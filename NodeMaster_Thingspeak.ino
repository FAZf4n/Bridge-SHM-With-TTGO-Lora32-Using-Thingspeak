//Dibuat Oleh Fauzan Affan ITK untuk keperluan Tugas Akhir, Terimakasih untuk semua website khususnya randomnerdtutorial dan how2electronics
#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>
 
//define the pins used by the LoRa transceiver module
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  915E6
 
// Replace with your network credentials
String apiKey = "VZWZR7UPPVJ63JZ1"; // Enter your Write API key from ThingSpeak
const char *ssid = "fauzan"; // replace with your wifi ssid and wpa2 key
const char *password = "fauzan22";
const char* server = "api.thingspeak.com";
 
WiFiClient client;
 
 
// Initialize variables to get and save LoRa data
int rssi;
String loRaMessage;
String accelerationx;
String accelerationy;
String accelerationz;
String readingID;
 
 
// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "Ax")
  {
    return accelerationx;
  }
  else if(var == "Ay")
  {
    return accelerationy;
  }
  else if (var == "RRSI")
  {
    return String(rssi);
  }
  else if (var == "Az")
  {
    return String(accelerationz);
  }
  return String();
}
 
void setup() {
  Serial.begin(115200);
  int counter;
 
  //setup LoRa transceiver module
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
//  LoRa.receive();
  Serial.println("init ok");
 
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
 
// Read LoRa packet and get the sensor readings
void loop() 
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    Serial.print("Lora packet received: ");
    while (LoRa.available())    // Read packet
  {
    String LoRaData = LoRa.readString();
    Serial.print(LoRaData); 
    
    
    int pos1 = LoRaData.indexOf('/');   
    int pos2 = LoRaData.indexOf('&');  
    int pos3 = LoRaData.indexOf('*'); 
    readingID = LoRaData.substring(0, pos1);                   // Get readingID
    accelerationx = LoRaData.substring(pos1 +1, pos2);           // Get temperature
    accelerationy = LoRaData.substring(pos2+1, pos3);  // Get pressure
    accelerationz = LoRaData.substring(pos3+1, LoRaData.length());
  }
  
  rssi = LoRa.packetRssi();       // Get RSSI
  Serial.print(" with RSSI ");    
  Serial.println(rssi);
}
 
  
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
   {
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(readingID);
      postStr += "&field2=";
      postStr += String(accelerationx);
      postStr += "&field3=";
      postStr += String(accelerationy);
      postStr += "&field4=";
      postStr += String(accelerationz);
      postStr += "\r\n\r\n\r\n\r\n";
    
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
 
    }    
    //delay(30000);
}