// Author: Mertcan AKIN
// Date: 24.11.2020
// Ultrasonic sensor and temperature sensor implementation
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure;

#define trig  7    // Trigger pin
#define echo  8    // Echo pin
#define lm35  A0   // Lm35 pin

long tof;                 // Time of flight of the ultrasonic wave
float distance;           // Distance will be measured
float distance_notemp;    // Distance with no temperature
float analog_val, temp;   // Variables to calculate temperature
float sound_speed;        // Speed of sound

void setup(){
  Serial.begin(115200);         // Start serial comm.
  pinMode(trig, OUTPUT);      // Pin parameters
  pinMode(echo, INPUT);
  
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
 
  if (!lox.begin()) {
    Serial.println(" ");
    while(1);
  }
 
}

void loop(){
  laser();

  digitalWrite(trig, LOW);    // Set trigger pin LOW
  delayMicroseconds(5);     
  digitalWrite(trig, HIGH);   // Trig the pin for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  tof = pulseIn(echo, HIGH);  // Read the echo pin. tof -> microseconds
  
  calc_temp();                // Calling temperature function

  sound_speed = 331.3 + (0.606*temp);   // Speed of sound formula m/s
  distance_notemp = (tof/2) / 29.1;     // 34000cm/1000000us = 1/29
  distance = tof * (sound_speed/10000)/2;  
  
  float error = 30 - distance_notemp;
  Serial.print(distance_notemp);
  Serial.print(" , ");
  Serial.print(error);
  Serial.print(" , ");
  Serial.println(measure.RangeMilliMeter/10);

  delay(100);
}

void calc_temp(){
  analog_val = analogRead(A0);              // Read raw values
  temp = ((analog_val/1023.0)*5000)/10.0;   // Signal mapping
}

void laser(){

  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
 
}
