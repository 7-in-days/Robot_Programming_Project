/*

#define accel_touch_pin 2
#define brake_touch_pin 3
#define green_led_pin   9
#define red_led_pin    10
#define button_pin     13

int    lastButtonReading;
int    buttonState;            
bool   ledEnabled = false;     
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  

void setup() {
  Serial.begin(9600);

  pinMode(button_pin, INPUT_PULLUP);
  lastButtonReading = digitalRead(button_pin);
  buttonState = lastButtonReading;

  pinMode(accel_touch_pin, INPUT);
  pinMode(brake_touch_pin, INPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  

  digitalWrite(green_led_pin, LOW);
  digitalWrite(red_led_pin, LOW);
}

void loop() {
  unsigned long now = millis();

  int reading = digitalRead(button_pin);
  if (reading != lastButtonReading) {
    lastDebounceTime = now;
  }
  if (now - lastDebounceTime >= debounceDelay) {
    // 안정된 상태로 간주
    if (reading != buttonState) {
      buttonState = reading;
      // 눌림(LOW) 에지에서만 토글
      if (buttonState == LOW) {
        ledEnabled = !ledEnabled;
      }
    }
  }
  lastButtonReading = reading;

  bool accelPressed = (digitalRead(accel_touch_pin) == LOW);
  bool brakePressed = (digitalRead(brake_touch_pin) == LOW);

  if (accelPressed && !brakePressed) {
    Serial.println("1");
  }
  else if (brakePressed && !accelPressed) {
    Serial.println("0");
  }

  if (ledEnabled) {
    if (accelPressed && !brakePressed) {
      digitalWrite(green_led_pin, HIGH);
      digitalWrite(red_led_pin, LOW);
    }
    else if (brakePressed && !accelPressed) {
      digitalWrite(green_led_pin, LOW);
      digitalWrite(red_led_pin, HIGH);
    }
    else {
      digitalWrite(green_led_pin, LOW);
      digitalWrite(red_led_pin, LOW);
    }
  } else {
    digitalWrite(green_led_pin, LOW);
    digitalWrite(red_led_pin, LOW);
  }


}
*/
/*
#define accel_touch_pin 2
#define brake_touch_pin 3
#define green_led_pin   9
#define red_led_pin    10
#define button_pin     13


int lastButtonReading;
int buttonState;            
bool ledEnabled = false;     
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  

unsigned long lastPrintTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(button_pin, INPUT_PULLUP);
  lastButtonReading = digitalRead(button_pin);
  buttonState = lastButtonReading;

  pinMode(accel_touch_pin, INPUT_PULLUP);
  pinMode(brake_touch_pin, INPUT_PULLUP);
  pinMode(green_led_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  
  digitalWrite(green_led_pin, LOW);
  digitalWrite(red_led_pin, LOW);
}

void loop() {
  unsigned long now = millis();

  int reading = digitalRead(button_pin);
  if (reading != lastButtonReading) {
    lastDebounceTime = now;
  }
  if (now - lastDebounceTime >= debounceDelay) {
    if (reading != buttonState && reading == LOW) {
      buttonState = reading;
      ledEnabled = !ledEnabled;
    }
  }
  lastButtonReading = reading;

  bool accelPressed = (digitalRead(accel_touch_pin) == LOW);
  bool brakePressed = (digitalRead(brake_touch_pin) == LOW);

  if (now - lastPrintTime >= 500) {
    lastPrintTime = now;
    if (accelPressed && !brakePressed) {
      Serial.println("1");
    }
    else if (brakePressed && !accelPressed) {
      Serial.println("0");
    }
  }

  if (ledEnabled) {
    if (accelPressed && !brakePressed) {
      digitalWrite(green_led_pin, HIGH);
      digitalWrite(red_led_pin, LOW);
    }
    else if (brakePressed && !accelPressed) {
      digitalWrite(green_led_pin, LOW);
      digitalWrite(red_led_pin, HIGH);
    }
    else {
      digitalWrite(green_led_pin, LOW);
      digitalWrite(red_led_pin, LOW);
    }
  } else {
    digitalWrite(green_led_pin, LOW);
    digitalWrite(red_led_pin, LOW);
  }
}

*/

// 최종본

#define accel_touch_pin   3
#define brake_touch_pin   2
#define green_led_pin     9
#define red_led_pin      10
#define button_pin       13

// 버튼 디바운스 & 토글 변수
int lastButtonReading;
int buttonState;
bool ledEnabled = false;
unsigned long lastButtonDebounceTime = 0;
const unsigned long buttonDebounceDelay = 50;

// 터치센서 디바운스 변수
bool lastRawAccel = HIGH;
bool stableAccel = HIGH;
unsigned long lastAccelDebounceTime = 0;

bool lastRawBrake = HIGH;
bool stableBrake = HIGH;
unsigned long lastBrakeDebounceTime = 0;

const unsigned long touchDebounceDelay = 20;  // 터치센서 디바운스(ms)

// 연속 출력 타이밍 (원한다면 조절 가능)
unsigned long lastPrintTime = 0;
const unsigned long printInterval = 100;

void setup() {
  Serial.begin(9600);

  // 버튼 세팅
  pinMode(button_pin, INPUT_PULLUP);
  lastButtonReading = digitalRead(button_pin);
  buttonState = lastButtonReading;

  // 터치센서 풀업 (LOW가 눌림)
  pinMode(accel_touch_pin, INPUT_PULLUP);
  pinMode(brake_touch_pin, INPUT_PULLUP);

  pinMode(green_led_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  digitalWrite(green_led_pin, LOW);
  digitalWrite(red_led_pin, LOW);
}

void loop() {
  unsigned long now = millis();

  int buttonReading = digitalRead(button_pin);
  if (buttonReading != lastButtonReading) {
    lastButtonDebounceTime = now;
  }
  if (now - lastButtonDebounceTime >= buttonDebounceDelay) {
    if (buttonReading != buttonState && buttonReading == LOW) {
      ledEnabled = !ledEnabled;
    }
    buttonState = buttonReading;
  }
  lastButtonReading = buttonReading;

  bool rawAccel = digitalRead(accel_touch_pin);
  bool rawBrake = digitalRead(brake_touch_pin);

  if (rawAccel != lastRawAccel) {
    lastAccelDebounceTime = now;
  }
  if (now - lastAccelDebounceTime >= touchDebounceDelay) {
    stableAccel = rawAccel;
  }
  lastRawAccel = rawAccel;

  if (rawBrake != lastRawBrake) {
    lastBrakeDebounceTime = now;
  }
  if (now - lastBrakeDebounceTime >= touchDebounceDelay) {
    stableBrake = rawBrake;
  }
  lastRawBrake = rawBrake;


  if (now - lastPrintTime >= printInterval) {
    lastPrintTime = now;

    if (stableAccel == LOW && stableBrake == HIGH) {
      Serial.println("1");
    }
    else if (stableBrake == LOW && stableAccel == HIGH) {
      Serial.println("0");
    }
  }

  if (ledEnabled) {
    if (stableAccel == LOW && stableBrake == HIGH) {
      digitalWrite(green_led_pin, HIGH);
      digitalWrite(red_led_pin, LOW);
    }
    else if (stableBrake == LOW && stableAccel == HIGH) {
      digitalWrite(green_led_pin, LOW);
      digitalWrite(red_led_pin, HIGH);
    }
    else {
      digitalWrite(green_led_pin, LOW);
      digitalWrite(red_led_pin, LOW);
    }
  } else {
    digitalWrite(green_led_pin, LOW);
    digitalWrite(red_led_pin, LOW);
  }
}

