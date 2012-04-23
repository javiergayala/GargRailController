

#include <SoftwareSerial.h>
#include <serLCD.h>

/*
   GargRail Controller
   v0.01b - 2012-02-15
   
   Changelog:
   - 0.01b: Added debounce via millis in interrupt functions
            Decreased analog read range from 0-1023 to 0-255 by dividing value by 4
   - 0.01a: Added LCD via SoftwareSerial Library
   - 0.01: Initial version
*/


const int speedBtn = 0; // Pin of the speed potentiometer
const int emgStopBtn = 2; // Pin of the emergency stop button
const int brakeRelease = 3; // Pin of the brake release button
const int greenLight = 10; // Pin of the green light
const int brakeLight = 11; // Pin of the red light
const int lcdRx = -1; // Dummy pin since we don't RX back from the LCD
const int lcdTx = 4; // Pin to transmit to LCD via SoftwareSerial
const float speedSamples = 5.0; // Number of samples to take from the speed pot
volatile boolean emgStop = false; // Whether or not an Emergency Stop has been initiated
volatile boolean showMsg = false; // Whether or not we need to add a message for special reasons
volatile boolean brakeOnOff = false; // Whether to show the message about the brake being released
volatile float avgSpeed; // The average speed as computed by the samples
volatile int speed = 0; // Speed of the train
volatile int dispSpeed = 0;
static unsigned long last_loop_time = 0;


serLCD LCD(lcdTx);

void setup() {
    LCD.setBrightness(30);
    LCD.clear();
    Serial.begin(9600);
    Serial.println(" ");
    Serial.println("GargRail Controller v0.0.1b");
    LCD.selectLine(1);
    LCD.print("GargRail v.0.01b");
    LCD.selectLine(2);
    LCD.print("Initializing...");
    pinMode(emgStopBtn, INPUT);
    pinMode(brakeRelease, INPUT);
    pinMode(greenLight, OUTPUT);
    pinMode(brakeLight, OUTPUT);
    digitalWrite(emgStopBtn, HIGH);
    digitalWrite(brakeRelease, HIGH);
    attachInterrupt(0, stopTrain, FALLING);
    attachInterrupt(1, startTrain, FALLING);
    digitalWrite(greenLight, HIGH);
    digitalWrite(brakeLight, LOW);
    delay(3000);
    LCD.clearLine(2);
    LCD.selectLine(2);
    LCD.print("All Systems GO!!");
    delay(2000);
    showSpeed(0);
}

void loop() {
  if (emgStop == true) {
    if (showMsg == true) {
      LCD.clearLine(2);
      LCD.selectLine(2);
      LCD.print("!!!EMG STOP!!!");
      Serial.println("Emergency Stop Active!!!");
    }
    speed = 0;
  }
  else {
    if (brakeOnOff == true) {
      LCD.clearLine(2);
      LCD.selectLine(2);
      LCD.print("Brake Released");
      Serial.println("Emergency Brake Released...");
      brakeOnOff = false;
    }
    unsigned long loop_time = millis();
    if (loop_time - last_loop_time > 500)
    {
        if (avgSpeed != map(analogRead(speedBtn)/4, 0, 255, 0, 100) || showMsg == true) {
          speed = map(analogRead(speedBtn)/4, 0, 255, 0, 100);
          Serial.println(analogRead(speedBtn)); 
          avgSpeed = 0;
          for (int i=0; i<speedSamples; i++) {
            speed = map(analogRead(speedBtn)/4, 0, 255, 0, 100);
            avgSpeed += speed;
          }
          avgSpeed /= speedSamples;
          showSpeed(avgSpeed);
        }
    }
  } 
  showMsg = false;
}

void showSpeed(int speed) {
    Serial.print("Speed: ");
    Serial.println(speed);
    LCD.clearLine(2);
    LCD.selectLine(2);
    LCD.print("   Speed: ");
    LCD.print(speed);
}

void stopTrain() {
  // Serial.print statements don't work in interrupt functions!
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200)
  {
    speed = 0;
    //Serial.println(speed);
    digitalWrite(greenLight, LOW);
    digitalWrite(brakeLight, HIGH);
    emgStop = true;
    showMsg = true;
  }
}

void startTrain() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200)
  {
    speed = map(analogRead(speedBtn)/4, 0, 255, 0, 100);
    //Serial.println(speed);
    emgStop = false;
    showMsg = true;
    brakeOnOff = true;
    digitalWrite(greenLight, HIGH);
    digitalWrite(brakeLight, LOW);
  }
}

/*
void selectLineOne() {
  LCD.print(0xFE, BYTE); // command flag
  LCD.print(128, BYTE); // position
}

void selectLineTwo() {
  LCD.print(0xFE, BYTE); // command flag
  LCD.print(192, BYTE); // position
}

void clearLCD() {
  LCD.print(0xFE, BYTE); // command flag
  LCD.print(0x01, BYTE); // clear command
  delay(50);
}

void backlightLCD() {
  LCD.print(0xFE, BYTE); // command flag
  LCD.print(157, BYTE); // Turn LCD on 100%
}
  */
