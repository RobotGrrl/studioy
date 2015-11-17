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
const int MESSAGES_TEST = 2;
const int CYCLE_MODE = 3;
const int LIVE_MODE = 4;
int CURRENT_STATE = LIVE_MODE;

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

const int strike_up = 60;
const int strike_down = 120;


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

  // checking for both buttons held down
  if(digitalRead(button_L) == HIGH && digitalRead(button_R) == HIGH) {

    if(!button_hold) button_hold_start_time = current_time;
    button_hold = true;

    if(current_time-button_hold_start_time >= 3000) {
      myservo.attach(servo_pin);
      myservo.write(strike_up);
      delay(1000);
      myservo.write(strike_down);
      delay(1000);
      myservo.detach();
    }
    
  }







 if(CURRENT_STATE == LIVE_MODE) {

    if(current_time-last_update_live_mode >= 1000) {

        // read the RTC
        Wire.beginTransmission(DS1307_ADDRESS);
        Wire.write(zero);
        Wire.endTransmission();
      
        Wire.requestFrom(DS1307_ADDRESS, 7);
      
        int second = bcdToDec(Wire.read());
        int minute = bcdToDec(Wire.read());
        int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
        int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
        int monthDay = bcdToDec(Wire.read());
        int month = bcdToDec(Wire.read());
        int year = bcdToDec(Wire.read());
      
        current_time_seconds = second + (60*minute) + (60*60*hour);

        if(light_on) {
          elapsed_on_time = abs( current_time_seconds - light_on_seconds );
        }
    
        String s1 = "Light on for: ";
        String s2;
    
        if(elapsed_on_time >= (60*30)) {
          s2 = (String)elapsed_on_time + "s OMG";
        } else if(elapsed_on_time >= (60*5)) {
          s2 = (String)elapsed_on_time + "s BZZZT";
        } else {
          s2 = (String)elapsed_on_time + "s";
        }
    
        lcd.setCursor(0, 0);
        lcd.print(s1);
        lcd.setCursor(0, 1);
        lcd.print(s2);
    
        last_update_live_mode = current_time;

    }

}








  

if(CURRENT_STATE == CYCLE_MODE) {
    
if(button_lock == false) {

  if(current_time-last_tou_change >= long(30*1000) || last_tou_change == 0) {
    tou_state++;
    if(tou_state > 2) tou_state = 0;
    last_tou_change = current_time;
  }


  if(current_time-last_msg_flip >= long(5*1000) || last_msg_flip == 0) {
    
     cur_msg++;
     if(cur_msg > 4) cur_msg = 0;
     last_msg_flip = current_time;

     if(cur_msg == 0) {

        lcd.clear();
        lcd.home();

        if(light_on) {

          // read the RTC
          Wire.beginTransmission(DS1307_ADDRESS);
          Wire.write(zero);
          Wire.endTransmission();
        
          Wire.requestFrom(DS1307_ADDRESS, 7);
        
          int second = bcdToDec(Wire.read());
          int minute = bcdToDec(Wire.read());
          int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
          int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
          int monthDay = bcdToDec(Wire.read());
          int month = bcdToDec(Wire.read());
          int year = bcdToDec(Wire.read());
        
          current_time_seconds = second + (60*minute) + (60*60*hour);

          elapsed_on_time = abs( current_time_seconds - light_on_seconds );

          String s1 = "Light on for: ";
          String s2;

          if(elapsed_on_time >= (60*30)) {
            s2 = " " + (String)elapsed_on_time + " seconds OMG";
          } else if(elapsed_on_time >= (60*5)) {
            s2 = " " + (String)elapsed_on_time + " seconds BZZZT";
          } else {
            s2 = " " + (String)elapsed_on_time + " seconds";
          }

          Serial.println(s2);

          lcd.print(s1);
          lcd.setCursor(0, 1);
          lcd.print(s2);
          
        } else {
          readDHT22();
          String s1 = "Temp: " + (String)DHT.temperature + " C";
          String s2 = "Humidity: " + (String)DHT.humidity + "%";
        
          lcd.print(s1);
          lcd.setCursor(0, 1);
          lcd.print(s2);
        }

  
        if(tou_state == 0) {
    
          lcd.setPWM(REG_RED, 10);
          lcd.setPWM(REG_GREEN, 255);
          lcd.setPWM(REG_BLUE, 10);
          
          
        } else if(tou_state == 1) {
    
          lcd.setPWM(REG_RED, 200);
          lcd.setPWM(REG_GREEN, 200);
          lcd.setPWM(REG_BLUE, 10);
    
    
          
        } else if(tou_state == 2) {
    
          lcd.setPWM(REG_RED, 255);
          lcd.setPWM(REG_GREEN, 10);
          lcd.setPWM(REG_BLUE, 10);
     
        }
      
     } else if(cur_msg == 1) {
      
       if(tou_state == 0) {
  
        lcd.setPWM(REG_RED, 10);
        lcd.setPWM(REG_GREEN, 255);
        lcd.setPWM(REG_BLUE, 10);
        
        lcd.clear();
        lcd.home();
  
        lcd.print("Grid status:");
        lcd.setCursor(0, 1);
        lcd.print("Off peak");
        
      } else if(tou_state == 1) {
  
        lcd.setPWM(REG_RED, 200);
        lcd.setPWM(REG_GREEN, 200);
        lcd.setPWM(REG_BLUE, 10);
  
        lcd.clear();
        lcd.home();
  
        lcd.print("Grid status:");
        lcd.setCursor(0, 1);
        lcd.print("Mid peak");
  
        
      } else if(tou_state == 2) {
  
        lcd.setPWM(REG_RED, 255);
        lcd.setPWM(REG_GREEN, 10);
        lcd.setPWM(REG_BLUE, 10);
  
        lcd.clear();
        lcd.home();
  
        lcd.print("Grid status:");
        lcd.setCursor(0, 1);
        lcd.print("On peak");
   
      }

     } else if(cur_msg == 2) {


        if(tou_state == 0) {

          lcd.setPWM(REG_RED, 10);
          lcd.setPWM(REG_GREEN, 255);
          lcd.setPWM(REG_BLUE, 10);
          
          lcd.clear();
          lcd.home();
    
          lcd.print("The grid has");
          lcd.setCursor(0, 1);
          lcd.print("capacity");
          
        } else if(tou_state == 1) {

          lcd.setPWM(REG_RED, 200);
          lcd.setPWM(REG_GREEN, 200);
          lcd.setPWM(REG_BLUE, 10);
        
          lcd.clear();
          lcd.home();
    
          lcd.print("The grid is");
          lcd.setCursor(0, 1);
          lcd.print("becoming full");
          
        } else if(tou_state == 2) {

          lcd.setPWM(REG_RED, 255);
          lcd.setPWM(REG_GREEN, 10);
          lcd.setPWM(REG_BLUE, 10);
          
          lcd.clear();
          lcd.home();
    
          lcd.print("The grid is at");
          lcd.setCursor(0, 1);
          lcd.print("PEAK CAPACITY");
          
        }

      
     } else if(cur_msg == 3) {


        if(tou_state == 0) {

          lcd.setPWM(REG_RED, 10);
          lcd.setPWM(REG_GREEN, 255);
          lcd.setPWM(REG_BLUE, 10);
          
          lcd.clear();
          lcd.home();
    
          lcd.print("Electricity rate:");
          lcd.setCursor(0, 1);
          lcd.print("8.3 cents/kWh");
          
        } else if(tou_state == 1) {

          lcd.setPWM(REG_RED, 200);
          lcd.setPWM(REG_GREEN, 200);
          lcd.setPWM(REG_BLUE, 10);
        
          lcd.clear();
          lcd.home();
    
          lcd.print("Electricity rate:");
          lcd.setCursor(0, 1);
          lcd.print("12.8 cents/kWh");
          
        } else if(tou_state == 2) {

          lcd.setPWM(REG_RED, 255);
          lcd.setPWM(REG_GREEN, 10);
          lcd.setPWM(REG_BLUE, 10);
          
          lcd.clear();
          lcd.home();
    
          lcd.print("Electricity rate:");
          lcd.setCursor(0, 1);
          lcd.print("17.5 cents/kWh");
          
        }


      
     }




  }



    
  }



  }
  






  if(Serial.available() > 0) {
    char c = Serial.read();

    if(c == 'a') {
      myservo.attach(servo_pin);
      myservo.write(strike_up);
      delay(500);
      myservo.detach();
    } else if(c == 'b') {
      myservo.attach(servo_pin);
      myservo.write(strike_down);
      delay(500);
      myservo.detach();
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



void timeOfUsage() {


  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());



  float off_peak_cost = 0.083;
  float mid_peak_cost = 0.128;
  float on_peak_cost = 0.175;
  
  float result_cost = 0.0;

  int hour_offset = -1; // dst

  if(month >= 11 || month < 5) {
    // winter

    if(weekDay == 0 || weekDay == 6) {
      // weekend
      // off peak
      result_cost = off_peak_cost;
    } else {
      // weekday
      
      if(hour >= (7+hour_offset) && hour < (11+hour_offset)) {
        // on peak
        result_cost = on_peak_cost;
      } else if(hour >= (11+hour_offset) && hour < (17+hour_offset)) {
        // mid peak
        result_cost = mid_peak_cost;
      } else if(hour >= (17+hour_offset) && hour < (19+hour_offset)) {
        // on peak
        result_cost = on_peak_cost;
      } else {
        // off peak
        result_cost = off_peak_cost;
      }
      
    }
    
  } else if(month >= 5 || month < 11) {
    // summer

    if(weekDay == 0 || weekDay == 6) {
      // weekend
      // off peak
      result_cost = off_peak_cost;
    } else {
      // weekday

      if(hour >= (7+hour_offset) && hour < (11+hour_offset)) {
        // mid peak
        result_cost = mid_peak_cost;
      } else if(hour >= (11+hour_offset) && hour < (17+hour_offset)) {
        // on peak
        result_cost = on_peak_cost;
      } else if(hour >= (17+hour_offset) && hour < (19+hour_offset)) {
        // mid peak
        result_cost = mid_peak_cost;
      } else {
        // off peak
        result_cost = off_peak_cost;
      }
      
    }
    
  }

  Serial.print("Time of usage cost $");
  Serial.print(result_cost);
  Serial.print("\n");
  
}



