\/*
Diablo-UDP connection

This program sends a udp packet to the port outlined in
the declarations. This packet is in a format for
diablo to recieve.

The data sent in each packet is the pressure measured
at 4 pressure sensors.

Created 22/10/2015
By Fintan Healy

*/


//Define Classes to use
#include <mcp_can_dfs.h>
#include <BMP280.h>
#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>


/***************************************************************
Declarations
**************************************************************/

//*********************  CAN Information  ********************//

const int     SPI_CS_PIN = 16;


const int     freq = 100;           //  Packet Frequency (Hz)

unsigned long timer;                //  timer

MCP_CAN       CAN(SPI_CS_PIN);	  	// Set CS pin



//*******************  Sensor Information  *******************//

const int32_t serialNum = 101;		// CAN Serial Number
const int8_t  codeVer   = 1;		// For 2nd CAN Message
const int8_t  Config    = 1;		// For 2nd CAN Message
int8_t        supplyVoltage = 1;	// For 2nd CAN Message

const byte    NUM_SENSORS = 4;    	//  Declare number of sensors
float         p[NUM_SENSORS];       //  Array to place raw sensor data in
uint16_t       p_int[NUM_SENSORS];  //  Array to place 'CAN ready' data in
int16_t       words[4];				//  Array to create CAN Messages in
float         t[NUM_SENSORS];       //  Array to place sensor temperature data in

const int     Enable = 0;           //  shift register enable pin  
const int     SPI_Speed = 100000;   //  SPI speed in Hz

byte          message[8];

BMP280 *bme[NUM_SENSORS];			// Array of BMP280 object to communicate with sensors

//***************  Shift Register Information  **************//

const int     shiftclock = 2;  //clock to control shift register
const int     shiftdata = 15;  // on rise this clock sends shift register data to pins
const int     data = 4;

// Define state of the shift registers outputs when floating
const int     Pin_Float_state = 5;
boolean sensorsSetup = false;  // USed to asses if sensors are initialised

/****************************************************************
Setup
***************************************************************/
void setup()
{
	delay(3000);
	Serial.begin(115200);
	pinMode(shiftclock, OUTPUT);
	pinMode(shiftdata, OUTPUT);
	pinMode(data, OUTPUT);
	pinMode(Enable, OUTPUT);
	pinMode(Pin_Float_state, OUTPUT);
	digitalWrite(data, HIGH);
	digitalWrite(Enable, HIGH);
	digitalWrite(Pin_Float_state, HIGH);
	digitalWrite(shiftclock, LOW);
	digitalWrite(shiftdata, LOW);
	for (int i = 1; i<NUM_SENSORS; i++)
	{
		shiftforward();
	}
	
	//*********************  CAN Setup  ************************//
	Serial.println("CAN Setup");

	START_INIT:

	if (CAN_OK == CAN.begin(CAN_1000KBPS))                   // init can bus : baudrate = 500k
	{
		Serial.println("CAN BUS Shield init ok!");
	}
	else
	{
		Serial.println("CAN BUS Shield init fail");
		Serial.println("Init CAN BUS Shield again");
		delay(1000);
		goto START_INIT;
	}
  
	timer = micros();
}


/******************************************************************
Main Loop
*****************************************************************/

void loop()
{
	///*****************  Regulate loop Frequency  *****************///
	//Serial.println(micros()-timer);
	while (micros()<(timer + (1000000 / freq))){}
	timer = micros();
  
	if(!sensorsSetup)
	{
		sensorsSetup = SetupSensors();
      // Set numbers so that 'dummy CAN messages can be sent if no sensors attached'
	}
	else
	{
		///******************  Collect Sensor Data  ********************///	  	  
		//call forced mode in all sensors
		bme[1]->write8(BMP280_REGISTER_CONTROL, 0x25, Pin_Float_state);
	  
		// Itarate over all sensors and place latest reading in p array
		
		for (int num = 0; num<NUM_SENSORS; num++)
		{
			if (num == 0)
			{
				digitalWrite(data, LOW);
			}
			else if (num == 1)
			{
				digitalWrite(data, HIGH);
			}
			else{}
			shiftforward();
			t[num] = (float)bme[num]->readTemperature();
			p[num] = (float)(bme[num]->readPressure());
		}
	}
	 SendCanMsgs(sensorsSetup);
	///************************  Loop Ends ************************///	
}


/***************************************************************
Functions
**************************************************************/

void CANWriteBytes(byte* array,int start)
{
	for (int i = 0; i <sizeof(array); i++)
	{
		message[start+i] = array[i];
	}
}

boolean SetupSensors()
{
	  
	  digitalWrite(Enable, HIGH);
	  digitalWrite(Pin_Float_state, HIGH);
	  digitalWrite(shiftclock, LOW);
	  digitalWrite(shiftdata, LOW);
	  
	  // Flush sensors to ensure none are selected...
	  digitalWrite(data, HIGH);
	  for (int i = 1; i<NUM_SENSORS; i++)
	  {
			shiftforward();
	  }
	  
	  Serial.println("BMP280 Setup");
	  boolean answer = true;
	  for (int num = 0; num<NUM_SENSORS; num++)
	  {
			if (num == 0)
			{
				digitalWrite(data, LOW);
			}
			else if (num == 1)
			{
				digitalWrite(data, HIGH);
			}
			else{}
			shiftforward();
			bme[num] = new BMP280(Enable, SPI_Speed);
			if (!(bme[num]->begin())) 
			{
				  Serial.printf("Could not find sensor on pin %d",
					num);
				  Serial.println(", check wiring!");
				  answer = false;
				  delete(bme[num]);
			}
	  }
	  return answer;
}

void SendCanMsgs(boolean isSetup)
{
	//  Create CAN Meassage 1 (contains pressure data)
	//  Build the Message - all values stored as int12 .... order ( p-2,p-1,p-4,pitot(empty here),p-3,blank 4 bytes)
	//  Sensor 3 is Abs so do [p[3] minus each of the others to get there relative presure
	//  at other end converted to millbar using 'pressure =gain*input+offset'
	//  where: offset= 7.5 millibar     gain = -0.018310546875
	//  so reverse enginer this to get correct output  

	int const relative_sensors[NUM_SENSORS-1] = {0,1,3};
	float const diff_gain =  -0.018310546875;
	for (int num=0; num<NUM_SENSORS-1;num++)
	{
    if(isSetup)
    {
        p[relative_sensors[num]]=(-p[2]+p[relative_sensors[num]])/100;                                             // cals differential pressure and convert from pascals to millibar
        p_int[relative_sensors[num]]=((uint16_t)round((p[relative_sensors[num]]-7.5)/diff_gain));   // Convert to simulate output of mesl 16 ( int12)....     
        if(num==0)
        {
          p_int[2] = (uint16_t)round((p[2]/100)-812.5)/0.091552734375);
        }
    }
    else
    {
        p_int[relative_sensors[num]]=((uint16_t)round((7-7.5)/diff_gain));
        if(num==0)
        {
          p_int[2] = (uint16_t)round((0)/0.091552734375);
        }       
    }		
	}
	//  convert absolute pressure to simulate that of mesl 16... ( gain = 0.091552734375, offset = 812.5 )
	
	  
	//  Create CAN Message structure  
	uint16_t Msg = 0;
	words[0] = (p_int[1] & 4095) | (p_int[0]<<12);
	CANWriteBytes((byte*)&words[0], 0);
	words[1] = ((p_int[0] & 4095)>>4) | (p_int[3] << 8);
	CANWriteBytes((byte*)&words[1], 2);
	words[2] =  ((p_int[3]&4095)>>8);
	CANWriteBytes((byte*)&words[2], 4);
	words[3] = (p_int[2] & 4095);
	CANWriteBytes((byte*)&words[3], 6);
	  
	// Send Message 1
	CAN.sendMsgBuf(0x140, 0, 8, message);

	//Create CAN Message 2 ( contain sensor info

	// 8 bytes - 4 bytes probe serial, 1 byte code version, 1 byte config, 1 byte Supply voltage ( 6 bits only), 1 byte Temp ( 6 bits only)

	// calc temp and convert to mesl 16 standard (6 bytes) gain 2.25 offset 0
	  float Temp = 0;
	  for (int i=0;i<NUM_SENSORS;i++)
	  {
			Temp += t[i]/NUM_SENSORS;
	  }
	  int8_t Temp_byte = (uint8_t)round((Temp)/2.25) & (uint8_t)63 ;
	  
	  CANWriteBytes((byte*)&serialNum, 0);
	  CANWriteBytes((byte*)&codeVer, 4);
	  CANWriteBytes((byte*)&Config, 5);
	  supplyVoltage=supplyVoltage & (uint8_t)63;
	  CANWriteBytes((byte*)&supplyVoltage, 6);
	  CANWriteBytes((byte*)&Temp_byte, 7);

	  CAN.sendMsgBuf(0x141, 0, 8, message);  
}


void shiftforward()
{
	digitalWrite(shiftclock, HIGH);
	digitalWrite(shiftclock, LOW);
	digitalWrite(shiftdata, HIGH);
	digitalWrite(shiftdata, LOW);
}
