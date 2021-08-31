#include <virtuabotixRTC.h> 
#include <SD.h>

// ***** SD Module *****
const int cs = 10;     // CS Pin

// ***** Real Time Module *****
// CLK -> 7 , DAT -> 8, Reset -> 9
virtuabotixRTC myRTC(7, 8, 9);

// ***** RPM *****
const int ir_pin1 = 2; 
const int ir_pin2 = 3; 

volatile double time1 = 0;
volatile double d_time1 = 0;
volatile double rpm1;

volatile double time2 = 0;
volatile double d_time2 = 0;
volatile double rpm2;
int led = 6;

void setup() {
  pinMode(ir_pin1,INPUT);
  pinMode(led,OUTPUT);
  
  // seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(00, 00, 00, 7, 31, 8, 2021);  
  
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
  Serial.println("initialization failed!");
  while (1);
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File myFile = SD.open("testlog.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
  SD.remove("testlog.txt");
  }
  // close the file:
  myFile.close();
  Serial.println("done.");

 
  // Attach interrupt to digital pin 2 (intterupt pin 0), run interrupt service routine on trigger
  attachInterrupt(0, calculate1, FALLING);
  attachInterrupt(1, calculate2, FALLING);
  Serial.begin(9600);  // Begin serial port
} 

void loop()
{
  myRTC.updateTime();
  
  // If the card is inserted or not.
  if (!SD.begin(cs)) {
      Serial.println("Please Insert the card...");
  }
 
  // Open the file
  File dataFile = SD.open("testlog.txt", FILE_WRITE);
  
  // if the file is available, start writing:
  if (dataFile && SD.begin(cs)) {
    digitalWrite(led, HIGH);
    dataFile.print(myRTC.minutes);
    dataFile.print(":");
    dataFile.print(myRTC.seconds);
    dataFile.print(" --- ");   
    dataFile.print(rpm1);
    dataFile.print(" - ");
    dataFile.println(rpm2);
    dataFile.close();
  } 
  else {
    digitalWrite(led, LOW);
    Serial.println("error opening file");
  }
  delay(50);
}

void calculate1(){
  d_time1 = micros()-time1;
  time1 = time1 + d_time1;
  rpm1 = 1/d_time1*1000000*60;
}

void calculate2(){
  d_time2 = micros()-time2;
  time2 = time2 + d_time2;
  rpm2 = 1/d_time2*1000000*60;
}
