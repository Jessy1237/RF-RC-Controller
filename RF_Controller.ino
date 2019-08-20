#include <SPI.h>

byte addressX = B00010000;
// Left side (Red LED in example) of digital Potentiometer is connected to Up/Down on RC Controller.

byte addressY = B00000000;
// Right side (Green LED in example) of digital Potentiometer is connected to Left/Right on RC Controller.
int CS = 10;
int beginner = 8;
int moderate = 7;
int hard = 6;
int expert = 5;

#define DEBUG

#define Up 3 // Yellow Wire (Pin 1 on DIN 9)
#define Down A3 // Red Wire (Pin 2 on DIN 9)
#define Left A2 // Brown Wire (Pin 3 on DIN 9)
#define Right A1 // Blue Wire (Pin 4 on DIN 9)

// Green Wire = Common (Pin 8 on DIN 9)

#define CENTER_X 2600
#define CENTER_Y 2100


int STEP_Y = 0;
int MAX_Y = 0;
int MIN_Y = 0;
int STEP_X = 0;
int MAX_X = 0;
int MIN_X = 0;

int mode_done = 0;
int x = CENTER_X;
int y = CENTER_Y;
int prev_x, prev_y = 0;

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
#endif

  pinMode(Up, INPUT_PULLUP);
  pinMode(Down, INPUT_PULLUP);
  pinMode(Left, INPUT_PULLUP);
  pinMode(Right, INPUT_PULLUP);

  pinMode(beginner, INPUT_PULLUP);
  pinMode(moderate, INPUT_PULLUP);
  pinMode(hard, INPUT_PULLUP);
  pinMode(expert, INPUT_PULLUP);
  pinMode (CS, OUTPUT);
  SPI.begin();


}

void loop() {

  if (digitalRead(beginner) == LOW) {
    
#ifdef DEBUG
    Serial.print("Beginner Mode Activated \n");
#endif

    STEP_Y = 25;
    MAX_Y = 2625;
    MIN_Y = 1625;

    STEP_X = 250;
    MAX_X = 3400;
    MIN_X = 2150;

    mode_done = 1;
  }
  else if (digitalRead(moderate) == LOW) {
    
#ifdef DEBUG
    Serial.print("Moderate Mode Activated \n");
#endif

    STEP_Y = 25;
    MAX_Y = 3150;
    MIN_Y = 1150;

    STEP_X = 250;
    MAX_X = 4000;
    MIN_X = 1500;

    mode_done = 1;
  }
  else if (digitalRead(hard) == LOW) {
    
#ifdef DEBUG
    Serial.print("Hard Mode Activated \n");
#endif

    STEP_Y = 50;
    MAX_Y = 3675;
    MIN_Y = 675;

    STEP_X = 500;
    MAX_X = 4600;
    MIN_X = 850;

    mode_done = 1;
  }
  else if (digitalRead(expert) == LOW) {
    
#ifdef DEBUG
    Serial.print("Expert Mode Activated \n");
#endif

    STEP_Y = 50;
    MAX_Y = 4200;
    MIN_Y = 200;

    STEP_X = 500;
    MAX_X = 5200;
    MIN_X = 200;

    mode_done = 1;
  }
  else {
    
    #ifdef DEBUG
    Serial.print("No Mode Set! \n");
    #endif
    
    mode_done = 0;
  }

  if (mode_done == 1) {
    //IF both are off then reset to centre, or if in error state and both are on then reset to centre
    if ((digitalRead(Right) == LOW && digitalRead(Left) == LOW) || (digitalRead(Right) == HIGH && digitalRead(Left) == HIGH))
    {
      x = CENTER_X;
      digitalPotWriteX(x);

      #ifdef DEBUG
      Serial.print("x is: ");
      Serial.print(x);
      Serial.print("\n");
      #endif
    }
    else if (digitalRead(Right) == LOW) {

      if (prev_x == 2) {
        x = CENTER_X;
        digitalPotWriteX(x);
      }
      if (MAX_X <= STEP_X + x) {
        x = MAX_X;

        #ifdef DEBUG
        Serial.print("x held at: ");
        Serial.print(x);
        Serial.print("\n");
        #endif
      }
      else {
        x += STEP_X;

        #ifdef DEBUG
        Serial.print("x changed to: ");
        Serial.print(x);
        Serial.print("\n");
        #endif
      }
      digitalPotWriteX(x);

      prev_x = 1;
    }
    else if (digitalRead(Left) == LOW) {

      if (prev_x == 1) {
        x = CENTER_X;
        digitalPotWriteX(x);
      }
      if (x - STEP_X <= MIN_X) {
        x = MIN_X;

        #ifdef DEBUG
        Serial.print("x held at: ");
        Serial.print(x);
        Serial.print("\n");
        #endif
      }
      else {
        x -= STEP_X;
        
        #ifdef DEBUG
        Serial.print("x changed to: ");
        Serial.print(x);
        Serial.print("\n");
        #endif
      }
      digitalPotWriteX(x);

      prev_x = 2;

    }

    if ((digitalRead(Up) == LOW && digitalRead(Down) == LOW) || (digitalRead(Up) == HIGH && digitalRead(Down) == HIGH))
    {
      y = CENTER_Y;
      digitalPotWriteY(y);
      
      #ifdef DEBUG
      Serial.print("y is: ");
      Serial.print(y);
      Serial.print("\n");
      #endif

    }
    else if (digitalRead(Up) == LOW) {
        if (prev_y == 2) {
          y = CENTER_Y;
          digitalPotWriteY(y);
        }

        if (y + STEP_Y >= MAX_Y)
        {
          y = MAX_Y;
          
          #ifdef DEBUG
          Serial.print("y held at: ");
          Serial.print(y);
          Serial.print("\n");
          #endif
        }
        else
        {
          y += STEP_Y;

          #ifdef DEBUG
          Serial.print("y changed to: ");
          Serial.print(y);
          Serial.print("\n");
          #endif
        }
        digitalPotWriteY(y);

        prev_y = 1;


      }
      else if (digitalRead(Down) == LOW) {
        if (prev_y == 1) {
          y = CENTER_Y;
          digitalPotWriteY(y);
        }

        if (y - STEP_Y <= MIN_Y)
        {
          y = MIN_Y;

          #ifdef DEBUG
          Serial.print("y held at: ");
          Serial.print(y);
          Serial.print("\n");
          #endif
        }
        else
        {
          y -= STEP_Y;
          
          #ifdef DEBUG
          Serial.print("y changed to: ");
          Serial.print(y);
          Serial.print("\n");
          #endif
        }
        digitalPotWriteY(y);

        prev_y = 2;


      }

      delay(100);

    }

  }

  int digitalPotWriteX(int valueX) {
    digitalWrite(CS, LOW);
    SPI.transfer(addressX);
    int data = (float)valueX / MAX_X * 128.0;
    SPI.transfer(data); //0 is 0ohm and 128 is 5kOhm. So we need to convert to our data values from the resistance values
    digitalWrite(CS, HIGH);
  }

  int digitalPotWriteY(int valueY) {
    digitalWrite(CS, LOW);
    SPI.transfer(addressY);
    int data = (float)valueY / MAX_Y * 128.0; //Same as X but since we are using the Wiper to GND (instead of 5V to Wiper) of the pot resistor we need to inverse our resistance values
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
  }
