//For low power mode
#include <avr/sleep.h>
#include <avr/power.h>

//OLED libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 5

Adafruit_SSD1306 display(OLED_RESET);

//In and Out
int pushbutton=4;

//Variables
const int ir_pin = 2; 

volatile double time = 0;
volatile double d_time = 0;
volatile double rpm;

void setup()   {                
  Serial.begin(9600);
  pinMode(ir_pin,INPUT);

  pinMode(pushbutton,INPUT);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done  
  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.setTextSize(2);
  display.setTextColor(WHITE);
  scrollENlogo();
  
  attachInterrupt(0, calculate, FALLING);

}


void loop() {
  if(digitalRead(pushbutton)==1){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Tachometer [ RPM ]");  
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,19);

    display.println(rpm); 
    delay(100); 
    display.display();
  }
  else{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Press the button");  
    delay(100); 
    display.display();
}


}

void calculate(){
  d_time = micros()-time;
  time = time + d_time;
  rpm = 1/d_time*1000000*60;
}

void scrollENlogo(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(7,0);
  display.clearDisplay();
  display.println("Automatic Control and Robotics Lab");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.display();
  delay(1);
 
  display.startscrolldiagright(0x00, 0x07);
  delay(5000);
  display.stopscroll();
  
}
