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

// states
const int AMBIENT_STATE = 0;
const int MESSAGE_STATE = 1;
int CURRENT_STATE = AMBIENT_STATE;

int button_L_prev = 0;
int button_L_current = 0;
long button_L_down = 0;
long button_L_up = 0;

int button_R_prev = 0;
int button_R_current = 0;
long button_R_down = 0;
long button_R_up = 0;

// vars
int mode = 0;
long last_switch = 0;
long current_time = 0;
int breath = 0;
boolean breath_dir = true;
long last_incr = 0;
long last_update = 0;
boolean switch_a = false;
boolean switch_b = false;
boolean hold_a = false;
boolean debouncing_b = false;
long last_button_L_press = 0;
long last_button_R_press = 0;
long last_print_date = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.print("hello");
  lcd.begin(16,2);
  
  //myservo.attach(servo_pin);
  
  pinMode(button_L, INPUT);
  pinMode(button_R, INPUT);
  pinMode(led_L, INPUT);
  pinMode(led_R, INPUT);
  pinMode(spkr, INPUT);
}

void loop() {

  current_time = millis();
  updateOnButton();
  updateOffButton();

  if(CURRENT_STATE == AMBIENT_STATE) {
    //ambient_display();

   //if( onButtonPressed() )
    
  } else if(CURRENT_STATE == MESSAGE_STATE) {

    

    
  }
  

  if(current_time-last_print_date >= 1000) {
    printDate();
    last_print_date = current_time;
  }
  

  delay(100);


  


  /*  
  //printDate();
  //servoTest();

  if(current_time-last_switch >= 3000) {
    mode++;
    if(mode > 3) mode = 0;
    
    lcd.clear();
    lcd.home();
    
    if(mode == 0) {
      offPeak();
    } else if(mode == 1) {
      midPeak();
    } else if(mode == 2) {
      onPeak();
    } else if(mode == 3) {
      gridMalfunction();
    }

    last_switch = current_time;

  }

  switch(mode) {
    case 0:
      lcd.setPWM(REG_RED, 0);
      lcd.setPWM(REG_GREEN, breath);
      lcd.setPWM(REG_BLUE, 0);
    break;
    case 1:
      lcd.setPWM(REG_RED, breath);
      lcd.setPWM(REG_GREEN, breath);
      lcd.setPWM(REG_BLUE, 0);
    break;
    case 2:
      lcd.setPWM(REG_RED, breath);
      lcd.setPWM(REG_GREEN, 0);
      lcd.setPWM(REG_BLUE, 0);
    break;
    case 3:
      lcd.setPWM(REG_RED, 255);
      lcd.setPWM(REG_GREEN, 0);
      lcd.setPWM(REG_BLUE, 0);
    break;
  }

  if(current_time-last_incr >= 100) {
    if(breath_dir) {
      breath += 40;
      if(breath > 255) { breath_dir = !breath_dir; breath = 255; }
    } else {
      breath -= 40;
      if(breath < 0) { breath_dir = !breath_dir; breath = 0; }
    }
    last_incr = current_time;
  }
  */
 
  //Serial.print("~");
  //delay(100);


 
  

}

void offPeak() {
  mode = 0;
  lcd.print("Off-Peak Rate");
  lcd.setCursor(0, 1);
  lcd.print("8.3 cents/kWh");
}

void midPeak() {
  mode = 1;
  lcd.print("Mid-Peak Rate");
  lcd.setCursor(0, 1);
  lcd.print("12.8 cents/kWh");
}

void onPeak() {
  mode = 2;
  lcd.print("On-Peak Rate");
  lcd.setCursor(0, 1);
  lcd.print("17.5 cents/kWh");
}

void gridMalfunction() {
  mode = 3;
  lcd.print("GRID MALFUNCTION");
  lcd.setCursor(0, 1);
  lcd.print("BEEP BEEP BEEEEP");
}



void servoTest() {
 for(int i=0; i<180; i++) {
    myservo.write(i);
    delay(10);
  }
  delay(100);
  for(int i=180; i>0; i--) {
    myservo.write(i);
    delay(10);
  }
  delay(100);
}




void statusDisplay() {

  lcd.clear();
  lcd.home();

  lcd.setPWM(REG_RED, 200);
  lcd.setPWM(REG_GREEN, 50);
  lcd.setPWM(REG_BLUE, 150);

  String s1 = "Temp: " + (String)DHT.temperature + " C";
  String s2 = "Humidity: " + (String)DHT.humidity + "%";

  lcd.print(s1);
  lcd.setCursor(0, 1);
  lcd.print(s2);
  
}


void ambient_state() {

  if(current_time-last_update >= 1000) {
    readDHT22();
    statusDisplay();
    last_update = current_time;
  }
  
}


