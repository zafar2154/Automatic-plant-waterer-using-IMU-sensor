#include <TimeLib.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define moisPin1 26
#define mois1Threshold 100	//definisikan nilai thresholdnya yaitu 100
int mois1;
#define moisPin2 25
#define mois2Threshold 100	//definisikan nilai thresholdnya yaitu 100
int mois2;
#define moisPin3 33
#define mois3Threshold 100	//definisikan nilai thresholdnya yaitu 100
int mois3;

const int sensorMin = 0;    // Nilai minimum (sangat basah)
const int sensorMax = 4095;

#define valve1 18
#define valve2 19

Adafruit_MPU6050 mpu;
int pitch, jam, menit;
bool penyiramanSelesai = false;
unsigned long waktuMulaiPenyiraman = 0;
sensors_event_t event;


void setup() {

  setTime(7, 59, 50, 11, 6, 2024); // Format: setTime(hour, minute, second, day, month, year)

  Serial.begin(115200);
  pinMode (moisPin1, INPUT);
  pinMode (moisPin2, INPUT);
  pinMode (moisPin3, INPUT);

  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);

  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

void loop() {
    //Serial Print IMU
    mpu.getAccelerometerSensor()->getEvent(&event);
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] X: ");
    Serial.print(event.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(event.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(event.acceleration.z);
    Serial.println(" m/s^2");

    //membaca pin soil moisture
    mois1=analogRead(moisPin1);
    mois2=analogRead(moisPin2);
    mois3=analogRead(moisPin3);

    //mengubah ke persen
    int moisPercent1 = map(mois1, sensorMin, sensorMax, 100, 0);
    int moisPercent2 = map(mois2, sensorMin, sensorMax, 100, 0);
    int moisPercent3 = map(mois3, sensorMin, sensorMax, 100, 0);
    Serial.println("Mois 1 :" +String(moisPercent1) + "% ; Mois 2 : " +String(moisPercent2) + "% ; Mois 3 : " + String(moisPercent3)+"%");

    //mengatur relay sesuai sudut kemiringan IMU
    if (event.acceleration.x>=5) {
      digitalWrite(valve1, HIGH);
      digitalWrite(valve2, LOW);
      Serial.println("mengisi air");
      delay(1000);
    }
    else if(event.acceleration.x<=5) {
      digitalWrite(valve1, LOW);
      digitalWrite(valve2, HIGH);
      Serial.println("menyiram tanaman");
      delay(3000);
      digitalWrite(valve1, LOW);
      digitalWrite(valve2, LOW);
      if(moisPercent1>=70 && moisPercent2>=70 && moisPercent3>=70){
        while(true){
        }
        Serial.println("tanaman selesai disiram");
      } 
      delay(5000);
    }
  }