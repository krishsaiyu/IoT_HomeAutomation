// Download this library at IOTGecko.com -> Code Examples:
//#include 
#define esp_baudrate 115200 // enter baud rate of your wifi module
// initialize the library with the esp8266(wifi module) 
iotgecko gecko = iotgecko(esp_baudrate);
String id = [Email_ID]; // iotgecko login id
String pass = "9662";  // iotgecko login password
String ssid = [WIFI_Name]; // SSID/name of your wifi router or wifi hotspot
String pass_key = [WIFI_Password]; //Wifi Password
bool notConected = true;
bool login = false;
const int number_of_loads = 4; // number of loads/devices to control
int load_status[number_of_loads];
//Assign pins for loads
const int load1 = 9;
const int load2 = 10;
const int load3 = 11;
const int load4 = 12;
void setup()
{
  Serial.begin(115200);
  pinMode(load1,OUTPUT);
  pinMode(load2,OUTPUT);
  pinMode(load3,OUTPUT);
  pinMode(load4,OUTPUT);
  digitalWrite(load1,LOW);
  digitalWrite(load2,LOW);
  digitalWrite(load3,LOW);
  digitalWrite(load4,LOW);
  
  Serial.println("iot home automation");
  delay(2000);
  Serial.println("conncecting to wifi");
  Serial.print(ssid);
  Serial.print("\t");
  Serial.println(pass_key);
  
  while(notConected)
  {
    if(gecko.GeckoConnect(ssid,pass_key)) // connect to wifi with given SSID and password
    {
      Serial.println("connceted to wifi...");
      notConected = false;
    }
    else
    {
      Serial.println("can't connect to wifi"); 
    }
    delay(1000);
  }
  Serial.println("connecting to iotgecko.com");
  while(!login)
  {
    if(gecko.GeckoVerify(id,pass)) // login to iotgecko.com with given ID and password
    {
      Serial.println("connected succesfully");
      login = true;
    }
    else
    {
      Serial.println("fail to connect");
    }
  }
}
void loop()
{
  int iot_status = gecko.GetgParams(load_status,number_of_loads); // Get load operation commands from server
  if(iot_status == InvalidUserIdOrPassword)
  {
    Serial.println("Invalid UserId or Password");
    while(1);
  }
  else if(iot_status == InvalidData)
  {
    Serial.println("Invalid Data");
    while(1);
  }
  else if(iot_status == VALID)
  {
    if(load_status[0]>=0 && load_status[1]>=0 && load_status[2]>=0 && load_status[3]>=0)
    {
      if(load_status[0]==1)
      {
        digitalWrite(load1,HIGH);
        Serial.println("Load 1 is ON");
      }
      else
      {
        digitalWrite(load1,LOW);
        Serial.println("Load 1 is OFF");
      }
      if(load_status[1]==1)
      {
        digitalWrite(load2,HIGH);
        Serial.println("Load 2 is ON");
      }
      else
      {
        digitalWrite(load2,LOW);
        Serial.println("Load 2 is OFF");  
      }
      
      if(load_status[2]==1)
      {
        digitalWrite(load3,HIGH);
        Serial.println("Load 3 is ON");
      }
      else
      {
        digitalWrite(load3,LOW);
        Serial.println("Load 3 is OFF");
      }
      
      if(load_status[3]==1)
      {
        digitalWrite(load4,HIGH);
        Serial.println("Load 4 is ON");
      }
      else
      {
        digitalWrite(load4,LOW);
        Serial.println("Load 4 is OFF");
      }
    }
    else
    {
      Serial.println("connection lost");
      Serial.println("reconnecting...");
      while(!gecko.GeckoReconnect()) // reconnect to the iotgecko.com 
      {
        Serial.println("conncetion failed.....reconnecting");
        delay(2000);
      }
      Serial.println("connected succesfully");
    }
  }
  Serial.println();
  delay(1000);
} 

           
