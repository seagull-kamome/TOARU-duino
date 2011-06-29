/*
  SD card file dump
 
 This example shows how to read a file from the SD card using the
 SD library and send it over the serial port.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created  22 December 2010
 
 This example code is in the public domain.
 	 
 */

#include <SD.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
SDCard<10> sd;
FATFS fs;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  
  SPIMaster::begin();
  sd.begin();
  // see if the card is present and can be initialized:
  uint8_t res;
  if (sd.mount(&fs)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  res = sd.open("datalog.txt");
  Serial.println(res, HEX);
  if (! sd.open("datalog.txt")) {
	  // if the file is available, write to it:
    while (sd.available()) {
      Serial.write(sd.read());
    }
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
}

void loop()
{
}

