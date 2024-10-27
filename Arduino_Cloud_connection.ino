#include <WiFi101.h> 
#include <ArduinoJson.h> 
#include <ArduinoHttpClient.h> 
#include <SPI.h> 
// ARTIK Cloud REST endpoint 
char server[] = "api.artik.cloud";   
int port = 443; // We're using HTTPS 
// Device ID tokens 
String deviceToken = "4478c8f8a18f4b34aad0e9601f1888eb"; 
String deviceId = "08bd76e489c94bca9f54431dd96f68bb"; 
// Your wifi network 
char ssid[] ="Arun";      
char pass[] = "arun12345"; 
float temperature = 0.0; 
boolean onFire = false; 
char buf[200]; // buffer to store the JSON to be sent to the ARTIK cloud 
const int LED = 6; 
int ledState = 0; 
WiFiSSLClient wifi; 
HttpClient client = HttpClient(wifi, server, port); 
int status = WL_IDLE_STATUS; 
void setup()
{ 
pinMode(LED,OUTPUT); 
Serial.begin(9600); 
while ( status != WL_CONNECTED) { // Keep trying until connected 
Serial.print("Attempting to connect to Network named: "); 
Serial.println(ssid);                   
// Connect to WPA/WPA2 network: 
status = WiFi.begin(ssid, pass); 
} 
} 
void loop() { 
Serial.println("loop"); 
ledToggle(); 
Serial.println("making POST request"); 
String contentType = "application/json"; 
String AuthorizationData = "Bearer " + deviceToken; //Device Token 
int sensor = analogRead(A0) * (3300/1024); // in milliVolt
temperature=(sensor-580)/10;
onFire = false; 
int len = loadBuffer(temperature,onFire);   
Serial.println("Sending data "+String(temperature));  
// push the data to the ARTIK Cloud 
client.beginRequest(); 
client.post("/v1.1/messages"); //, contentType, buf 
client.sendHeader("Authorization", AuthorizationData); 
client.sendHeader("Content-Type", "application/json"); 
client.sendHeader("Content-Length", len); 
client.endRequest(); 
client.print(buf); 
// read the status code and body of the response 
int statusCode = client.responseStatusCode(); 
String response = client.responseBody(); 
Serial.print("Status code: "); 
Serial.println(statusCode); 
Serial.print("Response: "); 
Serial.println(response); 
Serial.println("Wait a bit"); 
delay(500); // delay 5 min 
} 
int loadBuffer(float temp, boolean onFire ) {   
StaticJsonBuffer<200> jsonBuffer; // reserve spot in memory 
JsonObject& root = jsonBuffer.createObject(); // create root objects 
root["sdid"] =  deviceId;   
root["type"] = "message"; 
JsonObject& dataPair = root.createNestedObject("data"); // create nested objects 
dataPair["Temperature"] = temp;   
dataPair["onFire"] = onFire; 
root.printTo(buf, sizeof(buf)); // JSON-print to buffer 
return (root.measureLength()); // also return length 
} 
void ledToggle(){ 
if (ledState == 0){ 
digitalWrite(LED,LOW); 
ledState = 1; 
} else { 
digitalWrite(LED,HIGH); 
ledState = 0; 
} 
} 
