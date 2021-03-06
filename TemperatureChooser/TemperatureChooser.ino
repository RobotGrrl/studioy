#include <Wire.h>
#include <Servo.h>
#include "rgb_lcd.h"
#include <dht.h>

#define DHT22_PIN 5
#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527

rgb_lcd lcd;
Servo myservo;
dht DHT;

// pins
const int button_L = 8; // green aka on
const int button_R = 9; // pink aka off
const int led_L = 11;
const int led_R = 7;
const int servo_pin = 10;
const int spkr = 6;
// lcd on i2c

long current_time = 0;

int button_L_state = 0; // 0 = released, 1 = pressed, 2 = released
int button_L_prev = 0;
int button_L_current = 0;
long button_L_down = 0;
long button_L_up = 0;

int button_R_state = 0; // 0 = released, 1 = pressed, 2 = released
int button_R_prev = 0;
int button_R_current = 0;
long button_R_down = 0;
long button_R_up = 0;

float current_temp = 21.0;
float default_temp = 21.0;
long last_change_time = 0;
float temp_incr = 0.5;
float min_temp = 15.0;
float max_temp = 25.0;
float current_temp_F = 0;
boolean button_hold = false;
long button_hold_start_time = 0;

boolean MONITOR_STATE = false;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.print("hello");
  lcd.begin(16,2);

  //myservo.attach(servo_pin);

  pinMode(button_L, INPUT);
  pinMode(button_R, INPUT);
  pinMode(led_L, OUTPUT);
  pinMode(led_R, OUTPUT);
  pinMode(spkr, OUTPUT);
  
  updateDisplay();
}

void loop() {

  current_time = millis();
  updateOnButton();
  updateOffButton();

  if( ( digitalRead(button_L) == HIGH && digitalRead(button_R) == HIGH ) || MONITOR_STATE == true) {

    if(!button_hold) button_hold_start_time = current_time;
    button_hold = true;

    readDHT22();

    double meep = DHT.temperature;
    double meep_F = meep * 9 / 5;
    meep_F += 32;

    Serial.println(meep);
  
    if(!MONITOR_STATE) {
      digitalWrite(led_L, HIGH);
      digitalWrite(led_R, HIGH);
    }

    if(current_time-button_hold_start_time >= 3000) {
      myservo.attach(servo_pin);
      myservo.write(120);
      delay(1000);
      myservo.write(60);
      delay(1000);
      /*
      for(int i=80; i<=100; i++) {
        myservo.write(i);
        delay(10);
      }
      delay(1000);
      for(int i=100; i>80; i--) {
        myservo.write(i);
        delay(10);
      }
      delay(1000);
      */
      myservo.detach();
    }
    
    lcd.setCursor(0, 0);
    lcd.print("Current temp:");
    lcd.setCursor(0, 1);
    lcd.print(meep);
    lcd.setCursor(4, 1);
    lcd.print("C  ");
    lcd.setCursor(8, 1);
    lcd.print(meep_F);
    lcd.setCursor(12, 1);
    lcd.print("F  ");
  } else {

    if(button_hold) {
      button_hold = false;
      current_temp = default_temp;
      updateDisplay();
      digitalWrite(led_L, LOW);
      digitalWrite(led_R, LOW);
      last_change_time = current_time;  
    }
    
  }
  

  if(current_time-last_change_time >= long(10000)) { // reset after 10s
    current_temp = default_temp;
    last_change_time = current_time;
  }


  if(!button_hold) {

    if(current_temp > min_temp && current_temp <= 18.0) {
      lcd.setPWM(REG_RED, 10);
      lcd.setPWM(REG_GREEN, 10);
      lcd.setPWM(REG_BLUE, 255);
    } else if(current_temp > 18.0 && current_temp <= 21.0) {
      lcd.setPWM(REG_RED, 10);
      lcd.setPWM(REG_GREEN, 255);
      lcd.setPWM(REG_BLUE, 10);
    } else if(current_temp > 21.0 && current_temp <= 23.0) {
      lcd.setPWM(REG_RED, 200);
      lcd.setPWM(REG_GREEN, 200);
      lcd.setPWM(REG_BLUE, 10);
    } else if(current_temp > 23.0 && current_temp <= max_temp) {
      lcd.setPWM(REG_RED, 255);
      lcd.setPWM(REG_GREEN, 10);
      lcd.setPWM(REG_BLUE, 10);
    }

  } else {
    lcd.setPWM(REG_RED, 150);
    lcd.setPWM(REG_GREEN, 50);
    lcd.setPWM(REG_BLUE, 150);
  }

  //Serial.print("~");
  //delay(100);

}




void onButtonPressed() {

  digitalWrite(led_L, HIGH);
  current_temp += temp_incr;

  if(current_temp >= max_temp) {
    current_temp = max_temp;
    playTone(300, 100);
    delay(50);
    playTone(300, 100);
    delay(50);
  } else {
    playTone(300, 100);
    delay(50);
  }

  updateDisplay();
  digitalWrite(led_L, LOW);
  last_change_time = current_time;
  
}

void offButtonPressed() {

  digitalWrite(led_R, HIGH);
  current_temp -= temp_incr;

  if(current_temp <= min_temp) {
    current_temp = min_temp;
    playTone(700, 100);
    delay(50);
    playTone(700, 100);
    delay(50);
  } else {
    playTone(700, 100);
    delay(50);
  }

  updateDisplay();
  digitalWrite(led_R, LOW);
  last_change_time = current_time;
  
}


void updateDisplay() {

  current_temp_F = current_temp * 9 / 5;
  current_temp_F += 32;
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print(current_temp);
  lcd.setCursor(4, 1);
  lcd.print("C  ");
  lcd.setCursor(8, 1);
  lcd.print(current_temp_F);
  lcd.setCursor(12, 1);
  lcd.print("F  ");
  
}






