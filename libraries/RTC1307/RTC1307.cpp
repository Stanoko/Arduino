/*
  RTC1307 RTC Lib for M5Stack
  by Stano Kovačič
 */

#include <inttypes.h>
#if (defined(__AVR__) || defined(ARDUINO_ARCH_AVR))
# include <avr/io.h>
#endif
#if ARDUINO >= 100
# include "Arduino.h"
#else
# include "WProgram.h"
#endif
#include "Wire.h"
#include "RTC1307.h"


#define I2C_ADDR 0x68


//-------------------- Constructor --------------------


RTC1307::RTC1307(void)
{
  return;
}


//-------------------- Public --------------------


void RTC1307::begin(void)
{
  Wire.begin(); //init I2C lib
}

bool RTC1307::isRtc()
{
  Wire.begin(); //init I2C lib
	Wire.beginTransmission(I2C_ADDR);
	return (Wire.endTransmission() == 0 ? true : false);
}

bool RTC1307::isRunning()
{
  uint8_t data;
	bool flag;
	Wire.beginTransmission(DS1307_ADDR);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(DS1307_ADDR, 1);
	data = Wire.read();
	flag = bitRead(data, 7);
	return (!flag);
}



void RTC1307::start(void)
{
  uint8_t sec;

  //get second and CH bit
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDR, 1);
  sec = Wire.read();
  Wire.read();

  //set second and clear CH bit
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.write(sec & 0x7F);
  Wire.endTransmission();

  return;
}


void RTC1307::stop(void)
{
  uint8_t sec, d;

  //get second and CH bit
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDR, 1);
  sec = Wire.read();
  d   = Wire.read();

  //set second and CH bit
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.write(sec | 0x80); //hold
  Wire.endTransmission();

  return;
}



void RTC1307::get(int *sec, int *min, int *hour, int *day, int *month, int *year)
{
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDR, 7);
  *sec   = bcd2bin(Wire.read() & 0x7F);
  *min   = bcd2bin(Wire.read());
  *hour  = bcd2bin(Wire.read());
           bcd2bin(Wire.read()); //day of week
  *day   = bcd2bin(Wire.read());
  *month = bcd2bin(Wire.read());
  *year  = bcd2bin(Wire.read()) + 2000;

  return;
}

String RTC1307::dateTimeFile()
{
  int sec, min, hour,day,month,year;
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDR, 7);
  sec   = bcd2bin(Wire.read() & 0x7F);
  min   = bcd2bin(Wire.read());
  hour  = bcd2bin(Wire.read());
           bcd2bin(Wire.read()); //day of week
  day   = bcd2bin(Wire.read());
  month = bcd2bin(Wire.read());
  year  = bcd2bin(Wire.read()) + 2000;

  return LeftPadZero(String(year,DEC),4)+LeftPadZero(String(month,DEC),2)+LeftPadZero(String(day,DEC),2)+"_"+LeftPadZero(String(hour,DEC),2)+LeftPadZero(String(min,DEC),2)+LeftPadZero(String(sec,DEC),2);

}

String RTC1307::dateYYYMMDD()
{
  int sec, min, hour,day,month,year;
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDR, 7);
  sec   = bcd2bin(Wire.read() & 0x7F);
  min   = bcd2bin(Wire.read());
  hour  = bcd2bin(Wire.read());
           bcd2bin(Wire.read()); //day of week
  day   = bcd2bin(Wire.read());
  month = bcd2bin(Wire.read());
  year  = bcd2bin(Wire.read()) + 2000;

  return LeftPadZero(String(year,DEC),4)+LeftPadZero(String(month,DEC),2)+LeftPadZero(String(day,DEC),2);
}

String RTC1307::date()
{
  int sec, min, hour,day,month,year;
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDR, 7);
  sec   = bcd2bin(Wire.read() & 0x7F);
  min   = bcd2bin(Wire.read());
  hour  = bcd2bin(Wire.read());
           bcd2bin(Wire.read()); //day of week
  day   = bcd2bin(Wire.read());
  month = bcd2bin(Wire.read());
  year  = bcd2bin(Wire.read()) + 2000;

  return LeftPadZero(String(day,DEC),2)+"."+LeftPadZero(String(month,DEC),2)+"."+LeftPadZero(String(year,DEC),4);
}


String RTC1307::time()
{
  int sec, min, hour;
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADDR, 7);
  sec   = bcd2bin(Wire.read() & 0x7F);
  min   = bcd2bin(Wire.read());
  hour  = bcd2bin(Wire.read());

  return LeftPadZero(String(hour,DEC),2)+":"+LeftPadZero(String(min,DEC),2)+":"+LeftPadZero(String(sec,DEC),2);
}

void RTC1307::set(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint16_t year)
{
  uint8_t ch, d;

  if(year >= 2000)
  {
    year -= 2000;
  }

  //get CH bit
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDR, 1);
  ch = Wire.read();
  d  = Wire.read();
  
  if(ch & 0x80) //CH set?
  {
    ch = 0x80;
  }
  else
  {
    ch = 0x00;
  }

  //set time and date
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(byte(0x00));
  Wire.write(bin2bcd(sec) | ch);
  Wire.write(bin2bcd(min));
  Wire.write(bin2bcd(hour));
  Wire.write(bin2bcd(0));
  Wire.write(bin2bcd(day));
  Wire.write(bin2bcd(month));
  Wire.write(bin2bcd(year));
  Wire.endTransmission();

  return;
}


void RTC1307::set(int sec, int min, int hour, int day, int month, int year)
{
  return set((uint8_t)sec, (uint8_t)min, (uint8_t)hour, (uint8_t)day, (uint8_t)month, (uint16_t)year);
}


//-------------------- Private --------------------
String RTC1307::LeftPadZero(String s,int len)
{
  String pom=s;
  if (pom.length()<len)
  {
     pom=Repeat("0",len-pom.length())+pom;
  }
  return pom;
}

String RTC1307::Repeat(String s,int n)
{
   String repeat;

  for (int i = 0; i < n; i++)
    repeat += s;

  return repeat;
}


uint8_t RTC1307::bin2bcd(uint8_t val)
{
  return val + 6 * (val / 10);
}


uint8_t RTC1307::bcd2bin(uint8_t val)
{
  return val - 6 * (val >> 4);
}
