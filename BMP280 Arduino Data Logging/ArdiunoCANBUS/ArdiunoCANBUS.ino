// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <BMP280.h>

MCP_CAN CAN(10);                                      // Set CS to pin 10 for CAN

int freq = 10;                   //  Packet Frequency (Hz)
unsigned long timer;             //  timer

//*******************  Sensor Information  *******************//

const byte NUM_SENSORS = 4;       //  Declare number of sensors
float p;                          //  data store
byte data[NUM_SENSORS * 4];
byte* bytes;                      //  To Store bytes to send

//Define pins Sensor SDO pins are attached to
const int row_pin[NUM_SENSORS] = { 3, 4, 5, 6 };

BMP280 *bme[NUM_SENSORS];
/****************************************************************
Setup
***************************************************************/
void setup()
{
	Serial.begin(115200);

	//*******************  Sensor Setup  ***********************//

	Serial.println("BMP280 Setup");


	for (int num = 0; num<NUM_SENSORS; num++){
		pinMode(row_pin[num], OUTPUT);
		digitalWrite(row_pin[num], LOW);
	}

	//  Intialise each sensor, if it is found this also retrieves 
	//  the calibration files and stores them for each sensor

	for (int num = 0; num<NUM_SENSORS; num++){
		bme[num] = new BMP280(row_pin[num]);
		if (!(bme[num]->begin())) {
			//Serial.printf("Could not find sensor on pin %d", 
			//row_pin[num]);
			Serial.println(", check wiring!");
			while (1);
		}
	}

	//*********************  CAN Setup  ************************//
START_INIT:

	if (CAN_OK == CAN.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
	{
		Serial.println("CAN BUS Shield init ok!");
	}
	else
	{
		Serial.println("CAN BUS Shield init fail");
		Serial.println("Init CAN BUS Shield again");
		delay(100);
		goto START_INIT;
	}
	timer = millis();
}

void loop()
{
	while (millis()<(timer + (1000 / freq))){}
	timer = millis();

	for (int num = 0; num<NUM_SENSORS; num++)
	{
		bme[num]->readTemperature();
		p = bme[num]->readPressure();
		bytes = (byte*)&p;
		for (int i = 0; i < sizeof(bytes); i++)
		{
			data[((num)+(4 * i))] = data[i];
		}

	}

	CAN.sendMsgBuf(0x00, 0, sizeof(data), data);
	Serial.println("Message sent");
	Serial.println(sizeof(data));

}
