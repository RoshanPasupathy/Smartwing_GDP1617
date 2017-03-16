/*
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
#include <BMP280.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <SPI.h>



/***************************************************************
					      Declarations
**************************************************************/

//*********************  UDP Information  ********************//


const char ssid[] = "Fintonian";		//  your network SSID (name)
const char pass[] = "fintan94";			//  your network password

const unsigned int localPort = 2390;    //  local UDP port

const IPAddress ip(192, 168, 2, 101);        //  Remote IP
const unsigned int RemotePort = 55005;  //  Remote Port 

const int freq = 50;					//  Packet Frequency (Hz)
const int32_t ID = 101;					//  Packet ID
int32_t Packet_Num = 1;					//  Packet Number

unsigned long timer;					//  timer

WiFiUDP udp;

//*******************  Sensor Information  *******************//

const byte NUM_SENSORS = 4;				//  Declare number of sensors
float p[NUM_SENSORS];					//  Array to place data in

const int Enable = 0;                   //	Shift register enable pin  
const int SPI_Speed = 10000000;         //	Hz
boolean sensorsSetup = false;			//	Used to asses if sensors are initialised


BMP280 *bme[NUM_SENSORS];

//***************  Shift Register Information  **************//

const int shiftclock = 2;				//	Clock to control shift register
const int shiftdata = 15;				//	On rise this clock sends shift register data to pins
const int data = 4;

// Define state of the shift registers outputs when floating
const int Pin_Float_state = 5;



/****************************************************************
							Setup
***************************************************************/
void setup()
{
	Serial.begin(115200);

	while (!Serial) {}        // wait for serial port to connect

start:
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
		// ShiftForward moves all the bits one step forwards in the shift registers
		// here it is used to flush through some '1' bits so that no sensors are selected
		shiftforward();
	}


 //*********************  UDP Setup  ************************//

	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, pass);   // Connect to WiFi network

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");

	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	Serial.println("Starting UDP");
	udp.begin(localPort);

	Serial.print("Local port: ");
	Serial.println(udp.localPort());
	timer = micros();
	}


/******************************************************************
							Main Loop
*****************************************************************/

void loop()
{

	//******************  Regulate loop Frequency  ******************//

	//Serial.println(micros()-timer);
	while (micros()<(timer + (1000000 / freq))){}
	timer = micros();
	//*******************  Collect Sensor Data  *********************//


	if (!sensorsSetup)
	{
		sensorsSetup = SetupSensors();
		// Set numbers so that 'dummy CAN messages can be sent if no sensors attached'
	}
	else
	{
		//call forced mode in all sensors
		bme[1]->write8(BMP280_REGISTER_CONTROL, 0x25, Pin_Float_state);

		// Itarate over all sensors and place last reading in p array
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
			bme[num]->readTemperature();
			p[num] = bme[num]->readPressure();
			Serial.println(p[num]);
		}
		//****************  Start Creating a UDP Packet  ****************//

		udp.beginPacket(ip, RemotePort);

		/***************************************************************
		Create Header
		**************************************************************/
		//Serial.println("Creating Packet");
		// Packet ID
		udpWriteBytes((byte*)&ID);

		//  Packet Number
		udpWriteBytes((byte*)&Packet_Num);
		Packet_Num++;

		//TS (currently millseconds since arduino startup)
		udpWriteBytes((byte*)&timer);

		/***************************************************************
		Data
		**************************************************************/
		//BMP280 Data  
		for (int num = 0; num<NUM_SENSORS; num++){
			udpWriteBytes((byte*)&p[num]);
		}

		/***************************************************************
		Send UDP Packet
		**************************************************************/
		udp.endPacket();

	}

	

	//************************  Loop Ends ************************//
}


/***************************************************************
Functions
**************************************************************/

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

void udpWriteBytes(byte* array)
/*
* This function takes an input of an array of bytes and writes
* these to the currently open Udp Packet
*/
{
	for (int i = 0; i < sizeof(array); i++)
	{
		udp.write(array[i]);
	}
}


void shiftforward()
{
	digitalWrite(shiftclock, HIGH);
	digitalWrite(shiftclock, LOW);
	digitalWrite(shiftdata, HIGH);
	digitalWrite(shiftdata, LOW);
}

