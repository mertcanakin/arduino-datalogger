#include <SD.h>

const int cs = 10;     // CS Pin

const int lm35 = A0;           // LM35 pin
const int buttonPin = 4;       // the number of the input pin
const int rled = 3;            // Red led pin
const int gled = 2;            // Green led pin

int button_state = 0;          // HIGH or LOW?
int button_last_state = 0;     // Last state of the button
int button_counter = 0;        // Number of pushes

void setup()
{
  Serial.begin(9600);           
  analogReference(INTERNAL);    // Reference for LM35. 10 mV = 1 Celcius. 10/1.0742 = 9.31
  pinMode(buttonPin, INPUT);    // Input(button) and Outputs(LEDs)
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
}

void loop()
{

  float sensor_val = analogRead(lm35);   // Raw data from LM35
  float temp = sensor_val/9.31;          // Temperature result
  button_state = digitalRead(buttonPin); // Reading button state

// Compare button with its previous state
  if (button_state != button_last_state) {
    // If the state is changed, increment the counter
    if (button_state == HIGH) {
      button_counter++;
      Serial.print("Pushes: ");
      Serial.println(button_counter);
    }
    else {
      
    }
    delay(50);
  }
  // If the card is inserted or not.
  if (!SD.begin(cs)) {
      Serial.println("Please Insert the card...");
  }
  // If you pressed the button sd card will start logging:
  if(button_counter % 2 == 1){

    // Open the file
    File dataFile = SD.open("log.txt", FILE_WRITE);
    
    // if the file is available, start writing:
    if (dataFile && SD.begin(cs)) {
      digitalWrite(gled, HIGH);
      digitalWrite(rled, LOW);
      dataFile.println(temp);
      dataFile.close();
      Serial.println(temp);  // Print on serial port too.
    } 
    else {
      error_file();
      Serial.println("error opening file");
    }
    delay(50);
    }

    // If card is inserted but button state is changed:
    else{
       digitalWrite(rled, HIGH);
       digitalWrite(gled, LOW); 
    }
  
      button_last_state = button_state;   // Update last button state
}

// Error opening file indicator
int error_file(){
  digitalWrite(gled, LOW);
  digitalWrite(rled, HIGH);
  delay(250);
  digitalWrite(rled, LOW);
}
