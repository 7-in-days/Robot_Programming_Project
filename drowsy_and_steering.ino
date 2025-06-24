//최종본(버튼 없는거)


/*
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// I2C LCD (0x3F, 16×2)
LiquidCrystal_I2C lcd(0x3F, 16, 2);
Servo windowServo;

#define Buzzer_pin       8
#define Touch_sensor_pin 2   
#define Servo_pin        9


enum State { Normal, Drowsy_Alert, Drowsy_Alarm, Steer_Alert, Steer_Alarm };
State state = Normal;

unsigned long startTime = 0;            
unsigned long last_servo_move = 0;      


const unsigned long Drowsy_Delay    = 3000;  // 눈 감김 경고에서 알람 
const unsigned long Steer_Delay     = 5000;  // 스티어 경고에서 알람 
const unsigned long Servo_Interval  = 500;   // 서보 이동 간격


const int servo_max   = 180;
const int servo_min   = 135;
const int servo_step  = 4;

int current_angle = servo_min;  

void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();

  pinMode(Buzzer_pin, OUTPUT);
  pinMode(Touch_sensor_pin, INPUT);

  windowServo.attach(Servo_pin);
  windowServo.write(current_angle);

  Serial.begin(9600);
  lcd.print("State: Normal");
}

void loop() {
  unsigned long now = millis();

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'C' && state == Normal) {
      state     = Drowsy_Alert;
      startTime = now;
      lcd.clear();
      lcd.print("Warn: DROWSY");
    }
    else if (c == 'O' && (state == Drowsy_Alert || state == Drowsy_Alarm)) {
      state = Normal;
      lcd.clear();
      lcd.print("State: Normal");
      noTone(Buzzer_pin);
    }
  }

  if (state == Normal) {
    if (digitalRead(Touch_sensor_pin) == LOW) {
      state     = Steer_Alert;
      startTime = now;
      lcd.clear();
      lcd.print("Warn: STEER");
    }
  }
  else if (state == Steer_Alert || state == Steer_Alarm) {
    if (digitalRead(Touch_sensor_pin) == HIGH) {
      state = Normal;
      lcd.clear();
      lcd.print("State: Normal");
      noTone(Buzzer_pin);
    }
  }

  switch (state) {
    case Drowsy_Alert:
      if (now - startTime >= Drowsy_Delay) {
        state = Drowsy_Alarm;
        lcd.clear();
        lcd.print("Alarm: DROWSY");
        tone(Buzzer_pin, 1500);
      }
      break;

    case Steer_Alert:
      if (now - startTime >= Steer_Delay) {
        state = Steer_Alarm;
        lcd.clear();
        lcd.print("Alarm: STEER");
        tone(Buzzer_pin, 1500);
      }
      break;

    default:
      break;
  }

  if (now - last_servo_move >= Servo_Interval) {
    last_servo_move = now;

    if (state == Steer_Alarm || state == Drowsy_Alarm) {
      if (current_angle < servo_max) {
        current_angle = min(current_angle + servo_step, servo_max);
        windowServo.write(current_angle);
      }
    }
    else {
      if (current_angle > servo_min) {
        current_angle = max(current_angle - servo_step, servo_min);
        windowServo.write(current_angle);
      }
    }
  }

}
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// I2C LCD (0x3F, 16×2)
LiquidCrystal_I2C lcd(0x3F, 16, 2);
Servo windowServo;

#define Buzzer_pin       8
#define Touch_sensor_pin 2   
#define Servo_pin        9
#define Button_pin      12

int lastButtonReading;
int buttonState;
const unsigned long buttonDebounceDelay = 50;
bool soundEnabled = true;
unsigned long lastButtonDebounceTime = 0;

enum State { Normal, Drowsy_Alert, Drowsy_Alarm, Steer_Alert, Steer_Alarm };
State state = Normal;

unsigned long startTime = 0;
unsigned long last_servo_move = 0;

const unsigned long Drowsy_Delay   = 3000;  // 눈 감김 경고 → 알람 지연(ms)
const unsigned long Steer_Delay    = 5000;  // 스티어 경고 → 알람 지연(ms)
const unsigned long Servo_Interval = 500;   // 서보 이동 간격(ms)

const int servo_max  = 180;
const int servo_min  = 135;
const int servo_step = 4;

int current_angle = servo_min;

void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("State: Normal");

  pinMode(Button_pin, INPUT_PULLUP);
  lastButtonReading = digitalRead(Button_pin);
  buttonState = lastButtonReading;

  pinMode(Buzzer_pin, OUTPUT);
  pinMode(Touch_sensor_pin, INPUT);

  windowServo.attach(Servo_pin);
  windowServo.write(current_angle);

  Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();

  int buttonReading = digitalRead(Button_pin);
  if (buttonReading != lastButtonReading) {
    lastButtonDebounceTime = now;
  }
  if (now - lastButtonDebounceTime >= buttonDebounceDelay) {
    if (buttonReading != buttonState && buttonReading == LOW) {
      soundEnabled = !soundEnabled;
      if (!soundEnabled) {
        noTone(Buzzer_pin);
      }
    }
    buttonState = buttonReading;
  }
  lastButtonReading = buttonReading;

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'C' && state == Normal) {
      state = Drowsy_Alert;
      startTime = now;
      lcd.clear();
      lcd.print("Warn: DROWSY");
    }
    else if (c == 'O' && (state == Drowsy_Alert || state == Drowsy_Alarm)) {
      state = Normal;
      lcd.clear();
      lcd.print("State: Normal");
      noTone(Buzzer_pin);
    }
  }

  if (state == Normal) {
    if (digitalRead(Touch_sensor_pin) == LOW) {
      state = Steer_Alert;
      startTime = now;
      lcd.clear();
      lcd.print("Warn: STEER");
    }
  }
  else if (state == Steer_Alert || state == Steer_Alarm) {
    if (digitalRead(Touch_sensor_pin) == HIGH) {
      state = Normal;
      lcd.clear();
      lcd.print("State: Normal");
      noTone(Buzzer_pin);
    }
  }

  switch (state) {
    case Drowsy_Alert:
      if (now - startTime >= Drowsy_Delay) {
        state = Drowsy_Alarm;
        lcd.clear();
        lcd.print("Alarm: DROWSY");
        if (soundEnabled) tone(Buzzer_pin, 1500);
      }
      break;

    case Steer_Alert:
      if (now - startTime >= Steer_Delay) {
        state = Steer_Alarm;
        lcd.clear();
        lcd.print("Alarm: STEER");
        if (soundEnabled) tone(Buzzer_pin, 1500);
      }
      break;

    default:
      break;
  }

  if (now - last_servo_move >= Servo_Interval) {
    last_servo_move = now;

    if (state == Steer_Alarm || state == Drowsy_Alarm) {
      if (current_angle < servo_max) {
        current_angle = min(current_angle + servo_step, servo_max);
        windowServo.write(current_angle);
      }
    }
    else {
      if (current_angle > servo_min) {
        current_angle = max(current_angle - servo_step, servo_min);
        windowServo.write(current_angle);
      }
    }
  }
}



