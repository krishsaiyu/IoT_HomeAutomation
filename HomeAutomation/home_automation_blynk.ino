#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "2687768ccece487a9e8b72811dd4dbb9";
#include<LiquidCrystal.h>
LiquidCrystal lcd( 7,6,5,4,3,2);//rs en ly rw is gnd pin config

// These constants won't change.  They're used to give names
// to the pins used:

const int PIR=8;
const int SMK=9;

const int light=10;
const int fan=11;
const int gate=12;
const int motor=13;
const int SW_WIFI=14;

String inputString = "";             // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete
unsigned char gsmdata[20];
unsigned char i,j,ser=0x37,st=0x01,data_cap=0x00,recdata=1;
int ctr=0,ctr2=0,ctr3=0,ctr4=0;
int PIRstate=0;
int SMKstate=0;

char inChar;
char ser_a;
char FlgPIR =1;
char FlgSMK =1;

void serialEvent()
{
  while (Serial.available())
  {
     inChar = (char)Serial.read();
     ser=inChar;
     if((ser=='<')&&(st==0x01)) 
     {
      lcd.setCursor(1,1);
      lcd.print("In serial INT");
      delay(10); 
      data_cap=0x01;
      st=0x00;     
     }
     if((data_cap==0x01)&&(i<7))
     {
      lcd.setCursor(1,1);
      lcd.print("In ser   dat");
      delay(10);
     gsmdata[i]=ser;
     i=i+1;
     }
  } 
}

void Motion_MSG()
{
lcd.setCursor(1,1);
lcd.print("MSG sending....");
Serial.write('A');
Serial.write('T');
Serial.write('+');
Serial.write('C');
Serial.write('M');
Serial.write('G');
Serial.write('S');
Serial.write('=');
Serial.write('"');
Serial.write('8');
Serial.write('1');
Serial.write('2');
Serial.write('4');
Serial.write('3');
Serial.write('0');
Serial.write('6');
Serial.write('7');
Serial.write('5');
Serial.write('2');
Serial.write('"');
Serial.write(13);
Serial.write(10);
delay(500);delay(1000);

Serial.write(0X20);
Serial.write('M');
Serial.write('O');
Serial.write('T');
Serial.write('I');
Serial.write('O');
Serial.write('N');
Serial.write(0X20);
Serial.write('D');
Serial.write('E');
Serial.write('T');
Serial.write('E');
Serial.write('C');
Serial.write('T');
Serial.write('E');
Serial.write('D');
delay(500);delay(1000);
Serial.write(13);
Serial.write(10);
Serial.write(26);
delay(500);delay(1000);
lcd.setCursor(1,1);
lcd.print("MSG. sent.     ");
delay(1000);delay(1000);delay(1000);
lcd.clear();
}

void Smoke_MSG()
{
lcd.setCursor(1,1);
lcd.print("MSG sending....");
Serial.write('A');
Serial.write('T');
Serial.write('+');
Serial.write('C');
Serial.write('M');
Serial.write('G');
Serial.write('S');
Serial.write('=');
Serial.write('"');
Serial.write('8');
Serial.write('1');
Serial.write('2');
Serial.write('4');
Serial.write('3');
Serial.write('0');
Serial.write('6');
Serial.write('9');
Serial.write('5');
Serial.write('2');
Serial.write('"');
Serial.write(13);
Serial.write(10);
delay(500);delay(1000);

Serial.write(0X20);
Serial.write('S');
Serial.write('M');
Serial.write('O');
Serial.write('K');
Serial.write('E');
Serial.write(0X20);
Serial.write('D');
Serial.write('E');
Serial.write('T');
Serial.write('E');
Serial.write('C');
Serial.write('T');
Serial.write('E');
Serial.write('D');
delay(500);delay(1000);
Serial.write(13);
Serial.write(10);
Serial.write(26);
delay(500);delay(1000);
lcd.setCursor(1,1);
lcd.print("MSG. sent.     ");
delay(1000);delay(1000);delay(1000);
lcd.clear();
}

void setup() 
{
FlgPIR =1;
FlgSMK =1;  
pinMode(PIR,INPUT); 
pinMode(SMK,INPUT_PULLUP);
 
pinMode(light,OUTPUT);
pinMode(fan,OUTPUT);
pinMode(gate,OUTPUT); 
pinMode(motor,OUTPUT);
pinMode(SW_WIFI,OUTPUT);
 
lcd.begin(16,2);//16 colums,2 rows
lcd.setCursor(5,0);//5 place,1 row
lcd.print("WELCOME");
lcd.setCursor(1,1);//5 place,1 row
lcd.print("Configuring......");

Serial.begin(9600);   
delay(500);

digitalWrite(SW_WIFI, HIGH);
delay(100);
Serial.write('A');
Serial.write('T');
Serial.write('+');
Serial.write('C');
Serial.write('I');
Serial.write('P');
Serial.write('M');
Serial.write('U');
Serial.write('X');
Serial.write('=');
Serial.write('1');
Serial.write(10);
Serial.write(13);
delay(2000);
Serial.write('A');
Serial.write('T');
Serial.write('+');
Serial.write('C');
Serial.write('I');
Serial.write('P');
Serial.write('S');
Serial.write('E');
Serial.write('R');
Serial.write('V');
Serial.write('E');
Serial.write('R');
Serial.write('=');
Serial.write('1');
Serial.write(',');
Serial.write('8');
Serial.write('0');
Serial.write(10);
Serial.write(13);
delay(1000);
digitalWrite(SW_WIFI, LOW);
delay(1000);

Serial.write('A');
Serial.write('T');
Serial.write(13);
delay(1000);
Serial.write('A');
Serial.write('T');
Serial.write('+');
Serial.write('C');
Serial.write('M');
Serial.write('G');
Serial.write('F');
Serial.write('=');
Serial.write('1');
Serial.write(13);
delay(1000);
lcd.clear();
}

void loop() 
{
  lcd.setCursor(6,0);//5 place,1 row
  lcd.print("HOME");
  lcd.setCursor(3,1);//5 place,1 row
  lcd.print("AUTOMATION");
   PIRstate = digitalRead(PIR);
   SMKstate = digitalRead(SMK);
   
   
 if ((PIRstate == 1)&&(FlgPIR == 1))
 {
  FlgPIR =0;
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Motion Detected");
  delay(2000);
  lcd.clear();
  Motion_MSG();
 }
  if (PIRstate == 0)
  {
  FlgPIR =1;
  }
  
 if ((SMKstate == 0)&&(FlgSMK == 1))
 {
  FlgSMK =0;
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Smoke Detected");
  delay(2000);
  lcd.clear();
  Smoke_MSG();
 }
 if (SMKstate == 1)
  {
  FlgSMK =1;
  }
 Blynk.run();
 
}


