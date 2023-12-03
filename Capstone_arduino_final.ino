#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define col 5
#define row 1
#define TEMP0 65

const int touchSensor = 2;
int touchState = 0;
int previousTouchState = 0;
int flag_onoff = 0;

LiquidCrystal_I2C lcd(0x27,16,2);

const int RED = A0;
const int GREEN = A1;
const int BLUE = A2;

const int tempSensor = A3;
bool flag_tempover = false;

const int heatPad = 5;


void setup() {
  Serial.begin(9600);
  pinMode(touchSensor, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(tempSensor, INPUT);
  pinMode(heatPad, OUTPUT);
  previousTouchState = digitalRead(touchSensor);
}

void turnOn() {
  flag_onoff = 1;
  Serial.println("touchsensor ON");

  lcd.init();
  lcd.backlight();
  lcd.noCursor();

  makeColor(0, 0, 255);
  Serial.println("LED: BLUE");

  digitalWrite(heatPad, HIGH);
  flag_tempover = true;
}

void turnOff() {
  flag_onoff = 0;
  Serial.println("touchsensor OFF");

  lcd.clear();
  lcd.noCursor();
  lcd.noBacklight();

  makeColor(0, 0, 0);

  digitalWrite(heatPad, LOW);
}

void act() {
  int temp = lm35conv(analogRead(tempSensor));
  lcd.setCursor(col, row);
  lcd.print(temp);
  lcd.setCursor(col+3, row);
  lcd.print("'C");
  lcd.noCursor();
  delay(100);
  Serial.print("LCD: ");
  Serial.print(temp);
  Serial.println("'C");

  if(flag_tempover == true) {
    if(temp >= TEMP0) {
      makeColor(255, 0, 0);
      Serial.println("LED: RED");
      flag_tempover = false;
    }
  }
}

void makeColor(int REDVALUE, int GREENVALUE, int BLUEVALUE) {
  analogWrite(RED, REDVALUE);
  analogWrite(GREEN, GREENVALUE);
  analogWrite(BLUE, BLUEVALUE);
}

int tmp36conv(double readVal) {
  double voltage = readVal*5/1024;
  double temp = voltage*100-50;
  return (int)temp;
}

int lm35conv(float readVal) {
  float temp = readVal * 500.0 / 1024.0;
  return (int)temp;
}

void loop() {

  touchState = digitalRead(touchSensor);

  if(previousTouchState != touchState) {
    if(touchState == 1) {
      if(flag_onoff == 0) turnOn();
      else if(flag_onoff == 1) turnOff();
    }
    delay(10);
  }

  previousTouchState = touchState;

  if(flag_onoff == 1) act();

}