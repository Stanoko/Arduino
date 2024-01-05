/*
  SDHelper Lib funcions for SD Card M5Stack
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
#include <M5Stack.h>
#include "SDHelper.h"



//-------------------- Constructor --------------------


SDHelper::SDHelper(void)
{
  return;
}


//-------------------- Public --------------------


void SDHelper::begin(void)
{
 isCardIn=SD.begin(); //init 
}


bool SDHelper::isCard()
{
  return isCardIn;
}

String SDHelper::cardType() {
  uint8_t x= SD.cardType();
  if(x == CARD_MMC){
    return "MMC";
  } else if(x == CARD_SD){
    return "SDSC";
  } else if(x == CARD_SDHC){
    return "SDHC";
  } else {
    return "UNKNOWN";
  }
}
bool SDHelper::writeFile(const char * path, String message){
    message=message+"\n";
    bool ret=false;
    File file = SD.open(path, FILE_WRITE);
    if(file){
      if(file.print(message.c_str())){
          ret=true;
      }
    }
    file.close();
    return ret;
}
bool SDHelper::appendFile(const char * path, String message){
    message=message+"\n";
    bool ret=false;
    File file = SD.open(path, FILE_APPEND);
    if(file){
      if(file.print(message.c_str())){
          ret=true;
      }
    }
    file.close();
    return ret;
}


String SDHelper::fileToString(const char * path) {
    String output="";
    File file = SD.open(path);
    if(file){
      while(file.available()){
        output=output+ file.readString();
      }
    }
    file.close();
    return output;
}


bool SDHelper::createDir(const char * path){
  return SD.mkdir(path);
}

bool SDHelper::removeDir(const char * path){
   return SD.rmdir(path);
}

bool SDHelper::renameFile(const char * path1, const char * path2){
  return SD.rename(path1, path2);
}

bool SDHelper::deleteFile(const char * path){
  return SD.remove(path);
}


//-------------------- Private --------------------


