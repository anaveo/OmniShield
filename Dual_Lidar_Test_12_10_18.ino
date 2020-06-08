/*------------------------------------------------------------------------------

  LIDARLite Arduino Library
  v3/GetDistancePwm

  This example shows how to read distance from a LIDAR-Lite connected over the
  PWM interface.

  Connections:
  LIDAR-Lite 5 Vdc (red) to Arduino 5v
  LIDAR-Lite Ground (black) to Arduino GND
  LIDAR-Lite Mode control (yellow) to Arduino digital input (pin 3)
  LIDAR-Lite Mode control (yellow) to 1 kOhm resistor lead 1
  1 kOhm resistor lead 2 to Arduino digital output (pin 2)

  (Capacitor recommended to mitigate inrush current when device is enabled)
  680uF capacitor (+) to Arduino 5v
  680uF capacitor (-) to Arduino GND

  See the Operation Manual for wiring diagrams and more information:
  http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf

------------------------------------------------------------------------------*/

unsigned long pulseWidth1;
unsigned long pulseWidth2;

int oldReading1 = 0;
int oldReading2 = 0;

int distanceCoveredL1 = 0;

int lidarR1 = 2;
int lidarR2 = 3;
int lidarL1 = 4;
int lidarL2 = 5;

const int ledL1 = 9;
const int ledL2 = 10;

const int buzzerPin = 8;

int startTimeL1 = 0;
int startTimeL2 = 0;
int alarmTime = 1100;
unsigned long interval=5000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.

boolean sensor1Activate = false;
boolean sensor2Activate = false;

void setup()
{
  Serial.begin(115200); // Start serial communications

  pinMode(lidarR1, OUTPUT); // Set pin 2 as trigger pin
  digitalWrite(lidarR1, LOW); // Set trigger LOW for continuous read

  pinMode(lidarR2, INPUT); // Set pin 3 as monitor pin

  pinMode(lidarL1, OUTPUT); // Set pin 2 as trigger pin
  digitalWrite(lidarL1, LOW); // Set trigger LOW for continuous read

  pinMode(lidarL2, INPUT); // Set pin 3 as monitor pin

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

}
void readValues() {
  pulseWidth1 = pulseIn(lidarR2, HIGH); // Count how long the pulse is high in microseconds
  pulseWidth2 = pulseIn(lidarL2, HIGH); // Count how long the pulse is high in microseconds
  // If we get a reading that isn't zero, let's print it
  if(pulseWidth1 != 0 && pulseWidth2 != 0){
  
    pulseWidth1 = pulseWidth1 / 10; // 10usec = 1 cm of distance
    pulseWidth2 = pulseWidth2 / 10; // 10usec = 1 cm of distance

    delay(100);

    oldReading1 = pulseWidth1;
    oldReading2 = pulseWidth2;
  pulseWidth1 = pulseIn(lidarR2, HIGH); // Count how long the pulse is high in microseconds
  pulseWidth2 = pulseIn(lidarL2, HIGH); // Count how long the pulse is high in microseconds
  distanceCoveredL1 = (abs((pulseWidth1) - (oldReading1)));
    Serial.print("Left Lidar: "); Serial.print(pulseWidth2); Serial.print("  ");
    Serial.print("Right Lidar: "); Serial.print(oldReading2); Serial.print("  ");
    Serial.println("cm");
  }
}

void blindSpotLidarOne() {
  //LIDAR 1 CODE
  if (pulseWidth1 <= 100){
   if (sensor1Activate == false){
      startTimeL1 = millis();
      Serial.println("time started");
  }
   sensor1Activate = true;
  }
 else{
   sensor1Activate = false;
  }
if (sensor1Activate == true && (alarmTime <= millis() - startTimeL1)){
   digitalWrite(ledL1, HIGH);
  }
else{
   digitalWrite(ledL1, LOW);
  }
}

void blindSpotLidarTwo() {
  //LIDAR 2 CODE
 if (pulseWidth2 <= 100){
   if (sensor2Activate == false){
      startTimeL2 = millis();
      Serial.println("time started");
  }
   sensor2Activate = true;
  }
 else{
   sensor2Activate = false;
  }
if (sensor2Activate == true && (alarmTime <= millis() - startTimeL2)){
   digitalWrite(ledL2, HIGH);
  }
else{
   digitalWrite(ledL2, LOW);
  }
}

void dangerDetectLidarOne() {
  if (distanceCoveredL1 >= 500){
    tone(buzzerPin, 1000);
    delay(70);
    noTone(buzzerPin);
    delay(70);
    tone(buzzerPin, 1000);
    delay(50);
    noTone(buzzerPin);
    delay(70);
    tone(buzzerPin, 1000);
    delay(70);
    noTone(buzzerPin);
    delay(70);
    tone(buzzerPin, 1000);
    delay(70);
    noTone(buzzerPin);
    delay(70);
    tone(buzzerPin, 1000);
    delay(70);
    noTone(buzzerPin);
    delay(70);
  }
  else {
    noTone(buzzerPin);
  }
}

void dangerDetectLidarTwo() {  
  
}

void loop()
{
  //Collect values from sensors
  readValues();

  //Collect Time
  unsigned long currentMillis = millis(); // grab current time

  //Execute blind spot functions for Lidars
  blindSpotLidarOne();
  blindSpotLidarTwo();
  dangerDetectLidarOne();
  dangerDetectLidarTwo();

 
    

    

  
}
  
