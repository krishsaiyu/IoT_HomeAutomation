#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, A0, 8);
int trig = 7;
int echo = 9;
SoftwareSerial mySerial(3, 6);
volatile int flow_frequency; // Measure flow sensor pulses
unsigned char flowsensor = 2; // Sensor Input
int val = 0; // variable for reading the pin status
int result = 0;
int counter = 0;
int currentState = 0;
int previousState = 0;
int counter1=0;
unsigned long currentTime;
int result1=0;
unsigned long cloopTime;
unsigned int l_hour; // Calculated litres/hour
unsigned amount;
int LCD=13;
unsigned capacity;
void flow () // Interrupt function
{
   flow_frequency++;
}
void setup() {
  pinMode(flowsensor, INPUT);
pinMode(LCD, OUTPUT);
digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
Serial.begin(9600);
attachInterrupt(0, flow, RISING); // Setup Interrupt
sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
pinMode(trig, OUTPUT);
pinMode(echo, INPUT); 
lcd.begin(16, 2);
delay(100);
}

void loop() {
  currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
    if (l_hour>1) 
    { 
currentState = 1;
}
else {
currentState = 0;
}
if(currentState==1)
{
counter = counter + 1;
}
delay(250);


   counter1=counter*60;
   result = l_hour/counter1;
   result1=result1+result;
   Serial.println(result1,DEC);
   Serial.print("Litres");
   delay(1000);
   
   
   //gsm module
    if (Serial.available()>0)
   switch(Serial.read())
  {
    
    case 's':
    SendMessage();
     break;
   }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());
}
 // ultrasonics
  long t = 0, h = 0, hp = 0;
  
  // Transmitting pulse
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Waiting for pulse
  t = pulseIn(echo, HIGH);
  
  // Calculating distance 
  h = t / 58;
 
  h = h - 6;  // offset correction
  h = 50 - h;  // water height, 0 - 50 cm
  
  hp = 2 * h;  // distance in %, 0-100 %
  
  // Sending to computer
  Serial.println(hp);
  delay(1000);
  capacity= 1000-hp;
lcd.clear();
lcd.setCursor(0,0);  
lcd.print("Water Management");
lcd.setCursor(0,1);  
lcd.println("System!!!");
delay(1000);
lcd.clear();
lcd.setCursor(0,0);  
lcd.print("Water available in tank in cms:");
lcd.setCursor(0,1);  
lcd.print(capacity);
lcd.print ("(cms)in tank");
delay(1000);
}
//for gsm module
void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"9597266107\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.print(result1);// The SMS text you want to send
  mySerial.println(" Litres of water consumed");
  amount=result1*3;
  mySerial.println("Total amount to be pay:");
  mySerial.print(amount);
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
 }
 
  
