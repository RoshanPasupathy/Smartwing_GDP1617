/***************************************************************************
  This is a library for the BMP280 pressure sensor

 ***************************************************************************/
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include "BMP280.h"


/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/


BMP280::BMP280(int8_t cspin,int spi_Speed) 
  : _cs(cspin),SPI_Speed(spi_Speed) 
{ }


bool BMP280::begin() {
  SPI.begin();

  if (read8(BMP280_REGISTER_CHIPID) != 0x58)
    return false;

  readCoefficients();
  write8(BMP280_REGISTER_CONTROL, 0x25, _cs);
  return true;
}


/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C/SPI
*/
/**************************************************************************/
void BMP280::write8(byte reg, byte value, int pin)
{
	SPI.beginTransaction(SPISettings(SPI_Speed, MSBFIRST, SPI_MODE0));
	digitalWrite(pin, LOW);
	SPI.transfer(reg & ~0x80); // write, bit 7 low
	SPI.transfer(value);
	digitalWrite(pin, HIGH);
	SPI.endTransaction();              // release the SPI bus   	
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t BMP280::read8(byte reg)
{
	uint8_t value;
  
	SPI.beginTransaction(SPISettings(SPI_Speed, MSBFIRST, SPI_MODE0));
	digitalWrite(_cs, LOW);
	SPI.transfer(reg | 0x80); // read, bit 7 high
	value = SPI.transfer(0);
	digitalWrite(_cs, HIGH);
	SPI.endTransaction();              // release the SPI bus   
	
	return value;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/
uint16_t BMP280::read16(byte reg)
{
	uint16_t value;

    SPI.beginTransaction(SPISettings(SPI_Speed, MSBFIRST, SPI_MODE0));
	digitalWrite(_cs, LOW);
    SPI.transfer(reg | 0x80); // read, bit 7 high
    value = (SPI.transfer(0) << 8) | SPI.transfer(0);
    digitalWrite(_cs, HIGH);
    SPI.endTransaction();              // release the SPI bus
	
	return value;
}

uint16_t BMP280::read16_LE(byte reg) {
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
  
}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/
int16_t BMP280::readS16(byte reg)
{
  return (int16_t)read16(reg);

}

int16_t BMP280::readS16_LE(byte reg)
{
  return (int16_t)read16_LE(reg);

}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void BMP280::readCoefficients(void)
{
   dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
   dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
   dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

   dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
   dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
   dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
   dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
   dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
   dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
   dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
   dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
   dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float BMP280::readTemperature(void)
{
  int32_t var1, var2;
  
  int32_t adc_T = read16(BMP280_REGISTER_TEMPDATA);
  adc_T <<= 8;
  adc_T |= read8(BMP280_REGISTER_TEMPDATA+2);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)dig_T1 <<1))) * 
	   ((int32_t)dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)dig_T1)) * 
	     ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) * 
	   ((int32_t)dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T/100;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float BMP280::readPressure(void) {
  int64_t var1, var2, p;
  
  int32_t adc_P = read16(BMP280_REGISTER_PRESSUREDATA);
  adc_P <<= 8;
  adc_P |= read8(BMP280_REGISTER_PRESSUREDATA+2);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)dig_P6;
  var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
  var2 = var2 + (((int64_t)dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) +
    ((var1 * (int64_t)dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
  
  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
  return (float)p/256;
}
