#include <HID-Project.h>
#include <HID-Settings.h>

#define pinA 14
#define pinB 16
// Sw change layout
const int pinSw = 10;
const int ledA = 15;
const int ledB = 18;
const int ledC = 19;
//Rotary Encoder 
int aState = 0;
int aLastState = 0;
int aCounter = 0;
int rotaryState = 0;
int rotaryLastState = 0;
int rotaryCounter = 0;
// Matris keys
const int ROWS = 2;
const int COLS = 2;
const byte REGISTER_DELAY = 10;
const byte REPEAT_DELAY = 80;
int frameSkipCount = 0;
unsigned long lastFrame = 0;

char keys[ROWS][COLS] = {
  { 'A', 'B'},
  { 'C', 'D'}
};

byte rowPins[ROWS] = { 5, 7};
byte colPins[COLS] = { 6, 8};

int readKey() {
  int keyCode = -1;

  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT);
    digitalWrite(colPins[i], HIGH);
  }
  for (int row = 0; row < ROWS; row++) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], LOW);
    for (int col = 0; col < COLS; col++) {
      if (!digitalRead(colPins[col])) {
        keyCode = (col << 0) | (row << 1);
      }
    }
    pinMode(rowPins[row], INPUT);
    digitalWrite(rowPins[row], HIGH);
  }
  return keyCode;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(pinSw, INPUT_PULLUP);
  // Leds mode
  // ledA for layout 1
  pinMode(ledA, OUTPUT);
  // ledB for layout 2
  pinMode(ledB, OUTPUT);
  // ledC for layout 3
  pinMode(ledC, OUTPUT);

  // Rotary Encoder pinA & pinB 
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  rotaryLastState = digitalRead(pinA);

  Serial.begin(9600);
}

void volume() {
  rotaryState = digitalRead(pinA);
  if (rotaryState != rotaryLastState){
    if (digitalRead(pinB) != rotaryState) {
      Consumer.write(MEDIA_VOLUME_UP);
        ++rotaryCounter;
    } else {
      Consumer.write(MEDIA_VOLUME_DOWN);
        --rotaryCounter;
    }
      Serial.print("Val:");
      Serial.println(rotaryCounter);
  }
  rotaryLastState = rotaryState;
}
int layouts() {
  aState = digitalRead(pinSw);
   if (aState != aLastState) {
    if (aState == LOW) {
      ++aCounter;
//      Serial.println("Dang nhan");
//      Serial.print("So lan nhan la:");
//      Serial.println(aCounter);
    } else {
//      Serial.println("off");
    }
   }
   aLastState = aState;

   if (aCounter == 0){
    digitalWrite(ledA, HIGH);
    digitalWrite(ledB, HIGH);
   } else if (aCounter == 1) {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, HIGH);
    digitalWrite(ledC, HIGH);
   } else if (aCounter == 2) {
    digitalWrite(ledB, LOW);
    digitalWrite(ledA, HIGH);
    digitalWrite(ledC, HIGH);
   } else {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, LOW);
    digitalWrite(ledC, LOW);
   }
   if (aCounter > 2) {
    aCounter = 0;
   }
  return aCounter;
}

void loop() {
  // Volume rotary
   volume();
// Keyboard
  unsigned long timeNow = millis();
  // read every 1 millisecond
  if (timeNow != lastFrame) {
    frameSkipCount++;
    if (frameSkipCount == REGISTER_DELAY) {
      // Begin process
      int layout = layouts();
      int key = readKey();
      if (key != -1) {
        // if key is pressed
        int c = (key >> 0) & 1;
        int r = (key >> 1) & 1;
        if (layout == 0) {
          // layout no. 1
            Keyboard.print(keys[r][c]);
        } else if (layout == 1){
          // layout no. 2
          if (keys[r][c] == 'A') {
            Keyboard.press(KEY_RIGHT_CTRL);
            Keyboard.write('a');
            Keyboard.release(KEY_RIGHT_CTRL);
          } else if (keys[r][c] == 'B') {
            Keyboard.press(KEY_RIGHT_CTRL);
            Keyboard.write('v');
            Keyboard.release(KEY_RIGHT_CTRL);
          } else if (keys[r][c] == 'C') {
            Keyboard.press(KEY_RIGHT_CTRL);
            Keyboard.write('c');
            Keyboard.release(KEY_RIGHT_CTRL);
          } else if (keys[r][c] == 'D') {
            Keyboard.press(KEY_RIGHT_CTRL);
            Keyboard.write('n');
            Keyboard.release(KEY_RIGHT_CTRL);
          }
        } else if (layout == 2) {
          // layout no. 3
          if (keys[r][c] == 'A') {
            Keyboard.press(KEY_RIGHT_CTRL);
            Keyboard.write('a');
            Keyboard.release(KEY_RIGHT_CTRL);
          } else if (keys[r][c] == 'B') {
            Keyboard.print('2');
          } else if (keys[r][c] == 'C') {
            Keyboard.print('3');
          } else {
            Keyboard.print('4');
          }
        }
      
      //Keyboard.write(keys[r][c]);
      } else {
        frameSkipCount = 0;
      }
     if (frameSkipCount == REPEAT_DELAY) {
      frameSkipCount = 0;
     }
     lastFrame = timeNow;
    }
  } 
}
