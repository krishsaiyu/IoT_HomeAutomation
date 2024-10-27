/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */
#include <RTCZero.h>
#include <SPI.h>
#include <SD.h>
const int cs=4;
File myFile;
RTCZero rtc;

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 55;
const byte hours = 13;

/* Change these values to set the current initial date */
const byte day = 16;
const byte month = 02;
const byte year = 17;

void setup() {
   Serial.begin(9600);
   rtc.begin(); // initialize RTC

  // Set the time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);

  // Set the date
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
  
  // Open serial communications and wait for port to open:
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  print2digits(rtc.getDay());
  myFile.print("/");
  Serial.print("/");
  
  print2digits(rtc.getMonth());
  myFile.print("/");
  Serial.print("/");
  
  print2digits(rtc.getYear());
  myFile.print(" ");
  Serial.print(" ");
  
  // ...and time
  print2digits(rtc.getHours());
  myFile.print(":");
  Serial.print(":");
  
  print2digits(rtc.getMinutes());
  myFile.print(":");
  Serial.print(":");
  
  print2digits(rtc.getSeconds());

  myFile.print("  ");
  Serial.print("  ");
  
  //delay(1000);
  myFile = SD.open("test.txt", FILE_WRITE);
  /*int sensor1 = analogRead(A1) * (3300/1024); // in milliVolt
  int temperature1=(sensor1-580)/10; 
  myFile.print("  ");
 // myFile.print("Temperature:");
  //myFile.println();
  //Serial.println("Temperature");
  //Serial.println();
  myFile.print("Temperature1:  ");
  myFile.println(temperature1);
  Serial.print("Temperature1:  ");
  Serial.println(temperature1);
 */
  delay(1000);
  myFile.close();
}
  void print2digits(int number) {
  if (number < 10) {
    myFile.print("0");
    Serial.print("0");// print a 0 before if the number is < than 10
   }
   else
   {
  myFile.print(number);
  Serial.print(number);
}
  }
  





