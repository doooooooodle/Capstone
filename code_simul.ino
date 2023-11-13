/*
schema
: 터치센서 -> 전원 ON/OFF -> LED 색 표현 & 열선 가열
: 열선 온도 측정 -> LCD에 온도 표시

구현 요소: 터치센서, 온도센서, 열선, led, lcd
*/

/*
보드 업로드 전 설정할 항목
- 핀번호 설정 (디지털: * / 아날로그: A*)
- led 색 바꿀 기준온도 설정
- lcd 커서 좌표 설정
*/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define col 5 // LCD 커서 좌표설정
#define row 1

// 터치센서
const int touchsensor = <<pin_num>>;
// int touchstate = 1; // 테스트용 코드
int touchstate = 0;
boolean flag_touch = false;

// lcd
LiquidCrystal_I2C lcd(0x27,16,2);

// led
const int RED = <<pin_num>>;
const int GREEN = <<pin_num>>;
const int BLUE = <<pin_num>>;

// 온도센서
const int tempsensor = <<pin_num>>;
const int TEMP0 = <<기준온도>>;
int temp;
boolean temp_flag = false;

// 열선
const int heatPad = <<pin_num>>;


void setup() {
  Serial.begin(9600);
  pinMode(touchsensor, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(tempsensor, INPUT);
  pinMode(heatPad, OUTPUT);
}

void makeColor(int REDVALUE, int GREENVALUE, int BLUEVALUE) {
  analogWrite(RED, REDVALUE);
  analogWrite(GREEN, GREENVALUE);
  analogWrite(BLUE, BLUEVALUE);
}

int conv(double readVal) {
  double voltage = readVal*5/1024;
  double temp = voltage*100-50;
  return (int)temp;
}

void loop() {

  if(touchstate == HIGH) { // 터치센서 접촉

    if(flag_touch == false) { // 꺼져 있던 경우
      flag_touch = true;
      Serial.println("touchsensor ON");

      // lcd initialization
      lcd.init();
      lcd.backlight();
      lcd.noCursor();

      // led = blue
      makeColor(0, 0, 255);
      Serial.println("LED: BLUE");

      // 열선 가열 시작
      digitalWrite(heatPad, HIGH);
      temp_flag = true;

      // touchstate = 0; // 테스트용 코드
    }

    else { // 켜져 있던 경우
      flag_touch = false;
      Serial.println("touchsensor OFF");

      // lcd off
      lcd.clear();
      lcd.noCursor();
      lcd.noBacklight();

      // led off
      makeColor(0, 0, 0);

      // 열선 가열 종료
      digitalWrite(heatPad, LOW);
    }

  }

  if(flag_touch == true) {

    // 온도 측정 => lcd/serial 출력
    temp = conv(analogRead(tempsensor));
    lcd.setCursor(col, row);
    lcd.print(temp);
    lcd.setCursor(col+3, row);
    lcd.print("'C");
    lcd.noCursor();
    Serial.print("LCD: ");
    Serial.print(temp);
    Serial.println("'C");

    if(temp_flag == true && temp >= TEMP0) {
      makeColor(255, 0, 0); // led = red
      Serial.println("LED: RED");
      temp_flag = false;
    }

  }
  
  delay(100); // 터치센서 반응 느리면 delay 삭제
}

/* 마지막에 확인할 점
1. 온도변환식
2. 온도센서 손상 여부
*/