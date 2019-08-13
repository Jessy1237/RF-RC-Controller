#include <SPI.h>

byte addressX = B00010000;
// Left side (Red LED in example) of digital Potentiometer is connected to Up/Down on RC Controller.

byte addressY = B00000000;
// Right side (Green LED in example) of digital Potentiometer is connected to Left/Right on RC Controller.
int CS = 10;


#define Up 3 // Yellow Wire (Pin 1 on DIN 9)
#define Down A3 // Red Wire (Pin 2 on DIN 9)
#define Left A2 // Brown Wire (Pin 3 on DIN 9)
#define Right A1 // Blue Wire (Pin 4 on DIN 9)

// Green Wire = Common (Pin 8 on DIN 9)

#define CENTER_X 2800
#define CENTER_Y 2800
#define STEP_Y 50
#define STEP_X 500

int x = CENTER_X;
int y = CENTER_Y;   
int prev_x, prev_y = 0;

void setup() {

  //Serial.begin(9600);

  pinMode(Up, INPUT_PULLUP);
  pinMode(Down, INPUT_PULLUP);
  pinMode(Left, INPUT_PULLUP);
  pinMode(Right, INPUT_PULLUP);

  pinMode (CS, OUTPUT);
  SPI.begin();

}

void loop() {

//IF both are off then reset to centre, or if in error state and both are on then reset to centre
  if ((digitalRead(Right) == LOW && digitalRead(Left) == LOW) || (digitalRead(Right) == HIGH && digitalRead(Left) == HIGH))
  {
    x = CENTER_X;
    digitalPotWriteX(x);
    //Serial.print("x is: ");
    //Serial.print(x);
    //Serial.print("\n");
  }
  else if (digitalRead(Right) == LOW) {
    if (x >= 5200) {
      x = 5200;
      digitalPotWriteX(x);
      //Serial.print("x held at: ");
      //Serial.print(x);
      //Serial.print("\n");
    }
    else {
      if (prev_x == 2) {
        x = CENTER_X;
        digitalPotWriteX(x);
      }
      x += STEP_X;
      digitalPotWriteX(x);

      prev_x = 1;
      //Serial.print("x changed to: ");
      //Serial.print(x);
      //Serial.print("\n");
    }

  }
  else if (digitalRead(Left) == LOW) {
    if (x <= 200) {
      x = 200;
      digitalPotWriteX(x);
      //Serial.print("x held at: ");
      //Serial.print(x);
      //Serial.print("\n");
    }
    else {
      if (prev_x == 1) {
        x = CENTER_X;
        digitalPotWriteX(x);
      }
      x -= STEP_X;
      digitalPotWriteX(x);

      prev_x = 2;
      //Serial.print("x changed to: ");
      //Serial.print(x);
      //Serial.print("\n");
    }
  }

  if ((digitalRead(Up) == LOW && digitalRead(Down) == LOW) || (digitalRead(Up) == HIGH && digitalRead(Down) == HIGH))
  {
    y = CENTER_Y;
    digitalPotWriteY(y);
    //Serial.print("y is: ");
    //Serial.print(y);
    //Serial.print("\n");

  }
  else if (digitalRead(Up) == LOW) {
    if (y >= 4200) {
      y = 4200;
      digitalPotWriteY(y);
      //Serial.print("y held at: ");
      //Serial.print(y);
      //Serial.print("\n");
    }
    else {
      if (prev_y == 2) {
        y = CENTER_Y;
        digitalPotWriteY(y);
      }
      y += STEP_Y;
      digitalPotWriteY(y);

      prev_y = 1;
      //Serial.print("y changed to: ");
      //Serial.print(y);
      //Serial.print("\n");
    }
  }
  else if (digitalRead(Down) == LOW) {
    if (y <= 1400) {
      y = 1400;
      digitalPotWriteY(y);
      //Serial.print("y held at: ");
      //Serial.print(y);
      //Serial.print("\n");
    }
    else {
      if (prev_y == 1) {
        y = CENTER_Y;
        digitalPotWriteY(y);
      }
      y -= STEP_Y;
      digitalPotWriteY(y);

      prev_y = 2;
      //Serial.print("y changed to: ");
      //Serial.print(y);
      //Serial.print("\n");
    }
  }

  delay(100);

}

int digitalPotWriteX(int valueX) {
  digitalWrite(CS, LOW);
  SPI.transfer(addressX);
  int data = (float)valueX/5000.0*128.0;
  SPI.transfer(data); //0 is 0ohm and 128 is 5kOhm. So we need to convert to our data values from the resistance values
  digitalWrite(CS, HIGH);
}

int digitalPotWriteY(int valueY) {
  digitalWrite(CS, LOW);
  SPI.transfer(addressY);
  int data = 128 - (float)valueY/5000.0*128.0; //Same as X but since we are using the Wiper to GND (instead of 5V to Wiper) of the pot resistor we need to inverse our resistance values
  SPI.transfer(data);
  digitalWrite(CS, HIGH);
}
