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
#include <Update.h>
#ifndef _M5STACK_H_
#include <M5Stack.h>
#endif
#include "SimpleSwitchBinaries.h"





//-------------------- Constructor --------------------


SimpleSwitchBinaries::SimpleSwitchBinaries(void)
{
  return;
}


//-------------------- Public --------------------
void SimpleSwitchBinaries::updateFromSD(String returnBinary) {
  bool mam=loadFromSD("/update.bin");
  // whe finished remove the binary from sd card to indicate end of the process
	if (mam)
	{
      File myFileIn = SD.open("/update.bin", FILE_READ);
      File myFileOut = SD.open("/GasMonitor.bin", FILE_WRITE);
      while (myFileIn.available()) {
        myFileOut.write(myFileIn.read());
      }
      myFileIn.close();
      myFileOut.close();	  
	  SD.remove("/update.bin");   
	  M5.Power.reset();
	} else {
	  changeSketch(returnBinary.c_str());
	}
}

void SimpleSwitchBinaries::changeSketch(String binFile)
{
  loadFromSD(binFile);
  M5.Power.reset();

}

//-------------------- Private --------------------
void SimpleSwitchBinaries::performUpdate(Stream &updateSource, size_t updateSize) {
  if (Update.begin(updateSize)) {      
	size_t written = Update.writeStream(updateSource);
	if (written == updateSize) {
	  M5.Lcd.println("Written : " + String(written) + " successfully");
	}
	else {
	  M5.Lcd.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
	}
	if (Update.end()) {
	  M5.Lcd.println("OTA done!");
	  if (Update.isFinished()) {
		  M5.Lcd.println("Update successfully done.");
      M5.Lcd.println("Rebooting.");
	  }
	  else {
		  M5.Lcd.println("Update not finished?");
      M5.Lcd.println(" Something went wrong!");
	  }
	}
	else {
	  M5.Lcd.println("Error Occurred. Error #: " + String(Update.getError()));
	}
  }
  else
  {
	  M5.Lcd.println("Not enough space to begin OTA");
  }
}

bool SimpleSwitchBinaries::loadFromSD(String binFile) {
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextSize(1); 
  M5.Lcd.setCursor(0,0);  
  File updateBin = SD.open(binFile.c_str());
  if (updateBin) {
	  if(updateBin.isDirectory()){
		M5.Lcd.println("Error, "+binFile+" is not a file");
		updateBin.close();
		return false;
	  }

	  size_t updateSize = updateBin.size();

	  if (updateSize > 0) {
		M5.Lcd.println("Loading bin");
		performUpdate(updateBin, updateSize);
	  }
	  else {
		M5.Lcd.println("Error, file is empty");
		return false;
	  }

	  updateBin.close();
	  return true;
	}
	  else {
		  M5.Lcd.println("Could't load "+binFile+" from SD");
		  return false;
	}
}
