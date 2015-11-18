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
// lcd is on i2c
const int button_L = 8; // green aka on
const int button_R = 9; // pink aka off
const int button_D = 4; // detail button on the side
const int led_L = 11;
const int led_R = 7;
const int servo_pin = 10;
const int spkr = 6;

// servo
const int strike_up = 60;
const int strike_down = 120;

// states
const int AMBIENT_STATE = 0;
const int MESSAGE_STATE = 1;
const int MESSAGES_TEST = 2;
const int CYCLE_MODE = 3;
const int LIVE_MODE = 4;
const int CREDITS_MODE = 5;
int CURRENT_STATE = CREDITS_MODE;

// vars
int mode = 0;
long current_time = 0;
long last_update = 0;
long last_print_date = 0;

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

long last_message_update = 0;
int message_ind = 0;

long button_hold_start_time = 0;
boolean button_hold = false;


//const int strike_up = 120;
//const int strike_down = 60;


long last_tou_change = 0;
int tou_state = 0; // 0 = off peak, 1 = mid peak, 2 = high peak
int cur_msg = 0;
long last_msg_flip = 0;

long last_update_live_mode = 0;

boolean button_lock = false;
boolean msg_displayed = false;
boolean light_on = false;

int light_on_seconds = 0;
int light_on_time = 0;
int light_off_time = 0;
int total_light_on_time = 0;
int current_time_seconds = 0;
int elapsed_on_time = 0;

struct RTC {
  int second;
  int minute;
  int hour; // 24 hour time
  int weekDay; // 0-6 -> sunday - Saturday
  int monthDay;
  int month;
  int year;
};

RTC thetime = { 0, 0, 0, 0, 0, 0, 0 };


void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.print("hello");
  lcd.begin(16,2);
  
  //myservo.attach(servo_pin);
  
  pinMode(button_L, INPUT);
  pinMode(button_R, INPUT);
  pinMode(button_D, INPUT);
  pinMode(led_L, OUTPUT);
  pinMode(led_R, OUTPUT);
  pinMode(spkr, OUTPUT);
}

void loop() {

  current_time = millis();


  if(digitalRead(button_D) == HIGH) {
    Serial.println("PRESSED!");
  }



  // update the buttons
  updateOnButton();
  updateOffButton();

  // checking for both buttons held down - easter egg feature
  if(digitalRead(button_L) == HIGH && digitalRead(button_R) == HIGH) {

    if(!button_hold) button_hold_start_time = current_time;
    button_hold = true;

    if(current_time-button_hold_start_time >= 3000) {
      servoTurnLightOn();
      delay(100);
      servoTurnLightOff();
      delay(100);
    }
    
  }


  // live mode displays how long the light has been turned on
   if(CURRENT_STATE == LIVE_MODE) {
      liveMode();
  }
  
  
  // cycle mode cycles through time of usage states
  if(CURRENT_STATE == CYCLE_MODE) {
      
    if(button_lock == false) {
      cycleMode();
    }
  
  }


  // display the credits scene
  if(CURRENT_STATE == CREDITS_MODE) {
    creditsMode();
  }
  


  // simulate the servo movements
  if(Serial.available() > 0) {
    char c = Serial.read();

    if(c == 'a') {
      digitalWrite(led_L, HIGH);
      servoTurnLightOn();
      digitalWrite(led_L, LOW);
    } else if(c == 'b') {
      digitalWrite(led_R, HIGH);
      servoTurnLightOff();
      digitalWrite(led_R, LOW);
    }
    
  }




/*

  if(CURRENT_STATE == AMBIENT_STATE) {
    
    ambientState();

  } else if(CURRENT_STATE == MESSAGE_STATE) {

    
  } else if(CURRENT_STATE == MESSAGES_TEST) {

    if(current_time-last_message_update >= 4000 || last_message_update == 0) {
      message_ind++;
      last_message_update = current_time;

      Serial.print("message index = ");
      Serial.println(message_ind);

      if(message_ind > 99) {
        message_ind = 0;
      }
  
      if(message_ind > 6) {
        message_ind = 99;
      } 
        
      lcd.clear();
      lcd.home();
  
      // "0000000000000000"

      switch(message_ind) {
        case 0:
          lcd.print("Grid status:");
          lcd.setCursor(0, 1);
          lcd.print("PEAK CAPACITY");
        break;
        case 1:
          lcd.print("Turning on this");
          lcd.setCursor(0, 1);
          lcd.print("light for 30 min");
        break;
        case 2:
          lcd.print("will add 72 kJ");
          lcd.setCursor(0, 1);
          lcd.print("to the grid.");
        break;
        case 3:
          lcd.print("15m: Hold ON to");
          lcd.setCursor(0, 1);
          lcd.print("confirm.");
          lcd.setCursor(11, 1);
          lcd.print("5");
          lcd.setCursor(12, 1);
          lcd.print("s");
        break;
        case 4:
          lcd.print("30m: Hold ON to");
          lcd.setCursor(0, 1);
          lcd.print("confirm.");
          lcd.setCursor(11, 1);
          lcd.print("5");
          lcd.setCursor(12, 1);
          lcd.print("s");
        break;
        case 5:
          lcd.print("OK, Lumenbot");
          lcd.setCursor(0, 1);
          lcd.print("will activate!");
        break;
        case 6:
          lcd.print("Make good use of");
          lcd.setCursor(0, 1);
          lcd.print("the electricity!");
        break;
        case 99:
          lcd.print("Lumenbot won't");
          lcd.setCursor(0, 1);
          lcd.print("activate Zzz...");
        break;
      }

    }
    
  }

  */

  
  //servoTest();
  //Serial.print("~");
  //delay(100);

}





void statusDisplay() {

  lcd.clear();
  lcd.home();

  lcdSetColour(200, 50, 150);

  String s1 = "Temp: " + (String)DHT.temperature + " C";
  String s2 = "Humidity: " + (String)DHT.humidity + "%";

  lcd.print(s1);
  lcd.setCursor(0, 1);
  lcd.print(s2);
  
}


void ambientState() {

  if(current_time-last_update >= 1000) {
    readDHT22();
    statusDisplay();
    last_update = current_time;
  }

  if(current_time-last_print_date >= 1000) {
    printDate();
    timeOfUsage();
    last_print_date = current_time;
  }
  
}



