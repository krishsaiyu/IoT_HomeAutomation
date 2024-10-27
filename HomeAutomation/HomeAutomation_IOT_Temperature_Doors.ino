#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <RTCZero.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 6
#define DHTTYPE DHT11
RTCZero rtc;

char ssid[] = "JioFi2_7B04D0";    //  your network SSID (name)
char pass[] = "5ynrrgxrm3";   // your network password
int keyIndex = 0;                // your network key Index number (needed only for WEP)

char server1[] = "mail.smtp2go.com";
int port1 = 2525;
int status = WL_IDLE_STATUS;
const int cs = 7;

File myFile;

int count = 0;

int num = 1;

int i = 0;

int count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0, count6 = 0, count7 = 0, count8 = 0, count9 = 0, count10 = 0;

const int GMT = 5; //change this to adapt it to your time zone

const int GMTmin = 29 ;

char server[] = "api.artik.cloud";

int port = 443; // We're using HTTPS

String deviceToken = "c31b998f045b4d9491a8d038d99478e5";

String deviceId = "0998923338be4f60a72714f78dfce317";

WiFiSSLClient wifi;

WiFiClient client1;

HttpClient client = HttpClient(wifi, server, port);

char buf[200]; // buffer to store the JSON to be sent to the ARTIK cloud

const int LED = 6;

int ledState = 0;

DHT dht(DHTPIN, DHTTYPE);

int sensor1, sensor2, sensor3, sensor4, sensor5, sensor6, sensor7; //Sensor input varibale

int temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7 ; //monitored parameters

int humid;   //monitored parameter for humidity

int state_door1, state_door2; //moniotored parameter for door status

int j = 0;

const int door1 = 1, door2 = 2, door3 = 3, buzzer = 0;

String alert, value, output;

int len;




void setup() {
  
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(door1, INPUT_PULLUP);

  pinMode(door2, INPUT_PULLUP);

  pinMode(door3, INPUT_PULLUP);
 Serial.println("WELCOME");

  //SD CARD-----------------------------------------------
  Serial.print("INITIALISING SD CARD.");

  if (!SD.begin(7))
  {
    Serial.println("SD CARD NOT DETECTED");
    return;
  }
 
  
  Serial.println("INITIALISATION DONE."); // open the file. note that only one file can be open at a time,so you have to close this one before opening another.
  myFile = SD.open("TrialRun.csv", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to TrialRun.csv");
    myFile.print("TEMPERATURE DATA LOG");
    myFile.print(",");
    
    myFile.print("SERIAL NUMBER");
    myFile.print(",");
    
    myFile.print("DATE");
    myFile.print(",");
    
    myFile.print("TIME");
    myFile.print(",");
    
    myFile.print("TEMPERATURE 1");
    myFile.print(",");
    
    myFile.print("TEMPERATURE 2");
    myFile.print(",");
    
    myFile.print("TEMPERATURE 3");
    myFile.print(",");
    
    myFile.print("TEMPERATURE 4");
    myFile.print(",");
    
    myFile.print("TEMPERATURE 5");
    myFile.print(",");
    
    myFile.print("TEMPERATURE 6");
    myFile.print(",");
    
    myFile.print("TEMPERATURE 7");
    myFile.print(",");
    
    myFile.println("HUMIDITY");
    // close the file:
    myFile.close();
    
    Serial.println("done.");
  }
  else
  {
    Serial.println("ERROR OPENING TrialRun.csv");
  }
  
  rtc.begin();
  unsigned long epoch;
  int numberOfTries = 1, maxTries = 6;
  do {
    epoch = WiFi.getTime();
  }
  while ((epoch == 0) || (1 > 6));
 
  if (numberOfTries > maxTries) {
    Serial.print("NTP unreachable!!");
    while (1);
  }
  else {
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    rtc.setEpoch(epoch);
    Serial.println();
  }

}



void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  status = 0;
  Serial.println("START OF LOOP");
  if ( status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  else
  {
    Serial.println("Internet status connected");
  }
  myFile = SD.open("TrialRun.csv", FILE_WRITE);
  if (!SD.begin(7))
  {
    Serial.println("ALERT!SD card has been removed");
    count++;
  }
  else
  {
    Serial.println("SD CARD STATUS : INSERTED");
    if (count > 2) {
      myFile.print("SD CARD WAS REMOVED AND REINSERTED");
    }
    count = 0;
   }
  myFile.print(",");
  myFile.print(num);
  num++;
  
  sensor1 = analogRead(A0) * (3300 / 1024); // in milliVolt
  temperature1 = (sensor1 - 580) / 10;
  
  sensor2 = analogRead(A1) * (3300 / 1024); // in milliVolt
  temperature2 = (sensor2 - 580) / 10;
  
  sensor3 = analogRead(A2) * (3300 / 1024); // in milliVolt
  temperature3 = (sensor3 - 580) / 10;
  
  sensor4 = analogRead(A3) * (3300 / 1024); // in milliVolt
  temperature4 = (sensor4 - 580) / 10;
  
  sensor5 = analogRead(A4) * (3300 / 1024); // in milliVolt
  temperature5 = (sensor5 - 580) / 10;
  
  sensor6 = analogRead(A5) * (3300 / 1024); // in milliVolt
  temperature6 = (sensor6 - 580) / 10;
  
  sensor7 = analogRead(A6) * (3300 / 1024); // in milliVolt
  temperature7 = (sensor7 - 580) / 10;
  
  state_door1 = digitalRead(1);
  state_door2 = digitalRead(2);
 // state_door3 = digitalRead(3);
  
  humid = dht.readHumidity();
  
  if (temperature1 >= 70 || temperature2 >= 70 || temperature3 >= 70 || temperature4 >= 70 || temperature5 >= 70 || temperature6 >= 70 || temperature7 >= 70)
  {
    tone(buzzer, 400);
  }
   noTone(buzzer);
   //email service
  if (temperature1 >= 70 || temperature2 >= 70 || temperature3 >= 70 || temperature4 >= 70 || temperature5 >= 70 || temperature6 >= 70 || temperature7 >= 70)
  {
    if (temperature1 >= 70)
    {
      alert = "temperature1 is high. Please check the condition";
      value = "Temperature value is:";
      output = temperature1;
      if (count1 == 0 || count1 == 300 || count1 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2);//..
        sendEmail();
      }
    }
   
    
    if (temperature2 >= 70)
    {
      alert = "temperature2 is high. Please check the condition";
      value = "Temperature value is:";
      output = temperature2;
      if (count2 == 0 || count2 == 300 || count2 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2);//..
        sendEmail();
      }
    }
   
   
    if (temperature3 >= 70)
    {
      alert = "temperature3 is high. Please check the condition";
      value = "Temperature value is:";
      output = temperature3;
      if (count3 == 0 || count3 == 300 || count3 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2);//..
        sendEmail();
      }
    }
   
    
    if (temperature4 >= 70)
    {
      alert = "temperature4 is high. Please check the condition";
      value = "Temperature value is:";
      output = temperature4;
      if (count4 == 0 || count4 == 300 || count4 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2); //..
        sendEmail();
      }
    }
    
    
    if (temperature5 >= 70)
    {
      alert = "temperature5 is high. Please check the condition";
      value = "Temperature value is:";
      output = temperature5;
      if (count5 == 0 || count5 == 300 || count5 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2); //..
        sendEmail();
      }
    }
    
    
    if (temperature6 >= 70)
    {
      alert = "temperature6 is high. Please check the condition";
      value = "Temperature value is:";
      output = temperature6;
      if (count6 == 0 || count6 == 300 || count6 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2); //..
        sendEmail();
      }
    }
   
    
    if (temperature7 >= 70)
    {
      alert = "temperature7 is high. Please check the condition";
      value = "Temperature value is:";
      output = temperature7;
      if (count7 == 0 || count7 == 300 || count7 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2);  //..
        sendEmail();
      }
    }
   
    
    if (state_door1 == 1 || state_door1 == HIGH)
    {
      alert = "Door1 is open. Please check the condition";
      value = " ";
      if (count8 == 0 || count8 == 300 || count8 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2); //..
        sendEmail();
      }
    }
 
    
    if (state_door2 == 1 || state_door2 == HIGH)
    {
      alert = "Door2 is open. Please check the condition";
      value = " ";
      if (count9 == 0 || count9 == 300 || count9 == 600)
      {
        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2); //..
        sendEmail();
      }
    }
 
    
//    if (state_door3 == 1 || state_door3 == HIGH)
//    {
//      alert = "Door3 is open. Please check the condition";
//      value = " ";
//      if (count10 == 0 || count10 == 300 || count10 == 600)
//      {
//        len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2); //..
//        sendEmail();
//      }
//    }
//  }
 
  
  count1++; count2++; count3++; count4++; count5++; count6++; count7++; count8++; count9++; count10++;

  
  if (temperature1 < 70 || temperature2 < 70 || temperature3 < 70 || temperature4 < 70 || temperature5 < 70 || temperature6 < 70 || temperature7 < 70)
  {
    if (temperature1 < 70)
       count1 = 0;
    
    if (temperature2 < 70)
       count2 == 0;
    
    if (temperature3 < 70)
       count3 = 0;
    
    if (temperature4 < 70)    
       count4 = 0;
    
    if (temperature5 < 70)    
       count5 = 0;
    
    if (temperature6 < 70)    
       count6 = 0;
    
    if (temperature7 < 70)    
       count7 = 0;
    
    if (state_door1 == 0 || state_door1 == LOW)    
       count8 = 0;
    
    if (state_door2 == 0 || state_door2 == LOW)    
       count9 = 0;
    
//    if (state_door3 == 0 || state_door3 == LOW)    
//       count10 = 0;
    
  }
  //-------------------------
  myFile.print(",");
  printDate();
  myFile.print(",");
  printTime();
  myFile.print(",");
 
  myFile.print(temperature1);
  Serial.print("Temperature1:  ");
  Serial.println(temperature1);
  myFile.print(",");
  
  myFile.print(temperature2);
  Serial.print("Temperature2:  ");
  Serial.println(temperature2);
  myFile.print(",");
  
  myFile.println(temperature3);
  Serial.print("Temperature3:  ");
  Serial.println(temperature3);
  myFile.print(",");
  
  myFile.println(temperature4);
  Serial.print("Temperatur4:  ");
  Serial.println(temperature4);
  myFile.print(",");
  
  myFile.println(temperature5);
  Serial.print("Temperature5:  ");
  Serial.println(temperature5);
  myFile.print(",");
  
  myFile.println(temperature6);
  Serial.print("Temperature6:  ");
  Serial.println(temperature6);
  myFile.print(",");
  
  myFile.println(temperature7);
  Serial.print("Temperature7:  ");
  Serial.println(temperature7);
  myFile.print(",");
  
  myFile.println(humid);
  Serial.print("Humidity:  ");
  Serial.println(humid);
  Serial.println();
  Serial.println();
  delay(2000);
  
  myFile.close();
  
  if ( status == WL_CONNECTED) {
    Serial.println(" VERIFYING INTERNET CONNECTION");
    Serial.println("loop");
    ledToggle();
    Serial.println("making POST request");
    String contentType = "application/json";
    String AuthorizationData = "Bearer " + deviceToken; //Device Token
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    if ( j == 0 || j == 96000)
    {
      Serial.println("SENDING DATA TO ARTIK CLOUD");

      //delay(500); // delay 5 min

      len = loadBuffer(temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, temperature7, humid, state_door1, state_door2); //..
      // read the status code and body of the response
      client.beginRequest();
      client.post("/v1.1/messages"); //, contentType, buf
      client.sendHeader("Authorization", AuthorizationData);
      client.sendHeader("Content-Type", "application/json");
      client.sendHeader("Content-Length", len);
      client.endRequest();
      client.print(buf);
      int statusCode = client.responseStatusCode();
      String response = client.responseBody();
      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
      Serial.println("Wait a bit");
      // read the status code and body of the response
      Serial.println("DATA SENT");
    }
    j++;
    if ( j == 96000)
    {
      j = 0;
    }
  }
}
int loadBuffer(int temp, int temp1, int temp2, int temp3, int temp4, int temp5, int temp6, int humidity, int door1_state, int door2_state)//...
{
  Serial.println("LOADBUFFER");
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  StaticJsonBuffer<200> jsonBuffer; // reserve spot in memory
  JsonObject& root = jsonBuffer.createObject(); // create root objects
  root["sdid"] =  deviceId;
  root["type"] = "message";
  JsonObject& dataPair = root.createNestedObject("data"); // create nested objects
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  for (i = 1; i <= 2; i++)
  {
    if (i % 2 == 0)
    {
      dataPair["Temperature1"] = temp;
      dataPair["Temperature2"] = temp1;
      dataPair["Temperature3"] = temp2;
      dataPair["Temperature4"] = temp3;
      dataPair["Temperature5"] = temp4;
      dataPair["Temperature6"] = temp5;
    }
    if (i % 2 != 0)
    {
      dataPair["Temperature7"] = temp6;
      dataPair["Humidity"] = humidity;
      dataPair["state_door1"] = door1_state;
      dataPair["state_door2"] = door2_state;
     // dataPair["state_door3"] = door3_state;
    }
  }
  i = 0;
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  root.printTo(buf, sizeof(buf)); // JSON-print to buffer
  return (root.measureLength()); // also return length
}
void ledToggle() {
  if (ledState == 0) {
    digitalWrite(LED, LOW);
    ledState = 1;
  } else {
    digitalWrite(LED, HIGH);
    ledState = 0;
  }
  //---------------------------------------------------
}
//SD CARD-----------------------------------------------
int print2digits(int number) {
  if (number < 10) {
    myFile.print("0");
    Serial.print("0");// print a 0 before if the number is < than 10
  }
  myFile.print(number);
  Serial.print(number);
}
void printTime()
{
  int a = rtc.getHours() + GMT;
  int b =  rtc.getMinutes() + GMTmin;
  int c = rtc.getSeconds();
  if (b > 59)
  {
    a = a + 1;
    b = b - 59;
  }
  myFile.print(a);
  Serial.print(a);
  myFile.print(":");
  Serial.print(":");
  myFile.print(b);
  Serial.print(b);
  myFile.print(":");
  Serial.print(":");
  Serial.print(c);
  myFile.print(c);
  Serial.println();
}

void printDate()
{
  Serial.print(rtc.getDay());
  myFile.print(rtc.getDay());
  Serial.print("/");
  myFile.print("/");
  Serial.print(rtc.getMonth());
  myFile.print(rtc.getMonth());
  myFile.print("/");
  Serial.print("/");
  Serial.print(rtc.getYear());
  myFile.print(rtc.getYear());
  Serial.print(" ");
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
//email service
byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;
  char tBuf[64];

  if (client1.connect(server1, port1) == 1) {
    Serial.println(F("connected"));
  } else {
    client1.stop();
    Serial.println(F("connection failed"));
    return 0;
  }

  if (!eRcv()) return 0;

  Serial.println(F("Sending hello"));
  // change to the IP of your Arduino
  strcpy_P(tBuf, PSTR("EHLO 192.168.0.2\r\n"));
  client1.write(tBuf);
  if (!eRcv()) return 0;

  Serial.println(F("Sending auth login"));
  strcpy_P(tBuf, PSTR("auth login\r\n"));
  client1.write(tBuf);
  if (!eRcv()) return 0;

  Serial.println(F("Sending User"));
  strcpy_P(tBuf, PSTR("bmtyazE5OTZAZ21haWwuY29t\r\n"));

  client1.write(tBuf);
  if (!eRcv()) return 0;

  Serial.println(F("Sending Password"));
  strcpy_P(tBuf, PSTR("TVFaNHFqOFY2Qnhu\r\n"));
  client1.write(tBuf);
  if (!eRcv()) return 0;

  // change to your email address (sender)
  Serial.println(F("Sending From"));
  strcpy_P(tBuf, PSTR("MAIL From: <nkrk1996@gmail.com>\r\n"));
  client1.write(tBuf);
  if (!eRcv()) return 0;

  // change to recipient address
  Serial.println(F("Sending To"));
  strcpy_P(tBuf, PSTR("RCPT To: <switchgeariot@gmail.com>\r\n"));
  client1.write(tBuf);
  if (!eRcv()) return 0;

  Serial.println(F("Sending DATA"));
  strcpy_P(tBuf, PSTR("DATA\r\n"));
  client1.write(tBuf);
  if (!eRcv()) return 0;

  Serial.println(F("Sending email"));

  // change to recipient address
  strcpy_P(tBuf, PSTR("To: Larsen & Toubro <switchgeariot@gmail.com>\r\n"));
  client1.write(tBuf);

  // change to your address
  strcpy_P(tBuf, PSTR("From: RAMESH <nkrk1996@gmail.com>\r\n"));
  client1.write(tBuf);

  client1.println("Subject: Alert mail....ATTENTION REQUIRED....\r\n");

  client1.println(alert);

  client1.println(".");

  if (!eRcv()) return 0;

  Serial.println(F("Sending QUIT"));
  strcpy_P(tBuf, PSTR("QUIT\r\n"));
  client1.write(tBuf);
  if (!eRcv()) return 0;

  client1.stop();

  Serial.println(F("disconnected"));

  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while (!client1.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
      client1.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client1.peek();

  while (client1.available())
  {
    thisByte = client1.read();
    Serial.write(thisByte);
  }

  if (respCode >= '4')
  {
    efail();
    return 0;
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;
  int loopCount = 0;

  client1.println("QUIT");

  while (!client1.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
      client1.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }

  while (client1.available())
  {
    thisByte = client1.read();
    Serial.write(thisByte);
  }

  client1.stop();

  Serial.println(F("disconnected"));
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print(F("BSSID: "));
  Serial.print(bssid[5], HEX);
  Serial.print(F(":"));
  Serial.print(bssid[4], HEX);
  Serial.print(F(":"));
  Serial.print(bssid[3], HEX);
  Serial.print(F(":"));
  Serial.print(bssid[2], HEX);
  Serial.print(F(":"));
  Serial.print(bssid[1], HEX);
  Serial.print(F(":"));
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F("signal strength (RSSI):"));
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print(F("Encryption Type:"));
  Serial.println(encryption, HEX);
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print(F("MAC address: "));
  Serial.print(mac[5], HEX);
  Serial.print(F(":"));
  Serial.print(mac[4], HEX);
  Serial.print(F(":"));
  Serial.print(mac[3], HEX);
  Serial.print(F(":"));
  Serial.print(mac[2], HEX);
  Serial.print(F(":"));
  Serial.print(mac[1], HEX);
  Serial.print(F(":"));
  Serial.println(mac[0], HEX);

  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print(F("NetMask: "));
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print(F("Gateway: "));
  Serial.println(gateway);
}


