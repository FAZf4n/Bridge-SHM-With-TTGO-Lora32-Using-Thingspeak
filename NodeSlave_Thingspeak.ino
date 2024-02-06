//Dibuat Oleh Fauzan Affan ITK untuk keperluan Tugas Akhir, Terimakasih untuk semua website khususnya randomnerdtutorial dan how2electronics
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include <SSD1306.h> 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  915E6

Adafruit_MPU6050 mpu;

int accelerationx = 0;
int accelerationy = 0;
int accelerationz = 0;
int gyrox = 0;
int gyroz = 0;
int gyroy = 0;

//packet counter
int readingID = 0;
 
int counter = 0;
String LoRaMessage = "";
  
//Initialize LoRa module
void startLoRA(){

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
//  LoRa.receive();
  Serial.println("init ok");
}

void startMPU(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("");
  delay(100);
}
 
void getReadings(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accelerationx = a.acceleration.x;
  accelerationy = a.acceleration.y;
  accelerationz = a.acceleration.z;
  gyrox = g.gyro.x;
  gyroy = g.gyro.y;
  gyroz = g.gyro.z;

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");
  Serial.println("");

  delay(500);
}
 
void sendReadings() {
  LoRaMessage = String(readingID) + "/" + String(accelerationx) + "&" + String(accelerationy) + "*" + (accelerationz) ;
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  
  Serial.print("Sending packet: ");
  Serial.println(readingID);
  readingID++;
  Serial.println(LoRaMessage);
}
 
void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  startLoRA();
  startMPU();
}
void loop() {
  getReadings();
  sendReadings();
  delay(500);
}