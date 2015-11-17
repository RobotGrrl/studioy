
void updateOnButton() {

  button_L_prev = button_L_current;
  button_L_current = digitalRead(button_L);
  
  // pressing / debouncing
  if(button_L_prev == LOW && button_L_current == HIGH && current_time-button_L_up > 20) {
    button_L_down = current_time;
    button_L_state = 1;
    Serial.println("ON!");
    onButtonPressed();
  }

  if(button_L_prev == HIGH && button_L_current == LOW && current_time-button_L_down > 20) {
    button_L_state = 0;
    Serial.println("ON! released");
    button_lock = false;
    msg_displayed = false;
    onButtonReleased();
  }



  // wait 100ms before saying that the button is being held down
  if(button_L_prev == HIGH && button_L_current == HIGH && current_time-button_L_down > 20) {
    // holding
    long holding_L = current_time-button_L_down;
    button_L_state = 2;
    Serial.print("ON! "); Serial.println(holding_L);

    button_lock = true;

    if(CURRENT_STATE == CYCLE_MODE) {

    if(tou_state == 0) {

          if(!msg_displayed) {
            lcd.setPWM(REG_RED, 10);
            lcd.setPWM(REG_GREEN, 255);
            lcd.setPWM(REG_BLUE, 10);
  
            lcd.clear();
            lcd.home();
      
            lcd.print("Hold for");
            lcd.setCursor(0, 1);
            lcd.print("5 seconds");
          }

          

          lcd.setCursor(15, 1);
          if(holding_L >= 4000) {
            lcd.print("1");
          } else if(holding_L >= 3000) {
            lcd.print("2");
          } else if(holding_L >= 2000) {
            lcd.print("3");
          } else if(holding_L >= 1000) {
            lcd.print("4");
          } else if(holding_L >= 0) {
            lcd.print("5");
          }



           if(holding_L >= long(5*1000)) {

            lcd.clear();
            lcd.home();
      
            lcd.print("Lumenbot");
            lcd.setCursor(0, 1);
            lcd.print("activated");

              light_on = true;
            
             myservo.attach(servo_pin);
              myservo.write(strike_up);
              delay(500);
              myservo.detach();
            digitalWrite(led_L, LOW);
          }
          
          
        } else if(tou_state == 1) {

          if(!msg_displayed) {
            lcd.setPWM(REG_RED, 200);
            lcd.setPWM(REG_GREEN, 200);
            lcd.setPWM(REG_BLUE, 10);
          
            lcd.clear();
            lcd.home();
      
            lcd.print("Hold for");
            lcd.setCursor(0, 1);
            lcd.print("7 seconds");
            

          }


            lcd.setCursor(15, 1);
          if(holding_L >= 6000) {
              lcd.print("1");
            } else if(holding_L >= 5000) {
              lcd.print("2");
            } else if(holding_L >= 4000) {
              lcd.print("3");
            } else if(holding_L >= 3000) {
              lcd.print("4");
            } else if(holding_L >= 2000) {
              lcd.print("5");
            } else if(holding_L >= 1000) {
              lcd.print("6");
            } else if(holding_L >= 0) {
              lcd.print("7");
            }



          if(holding_L >= long(7*1000)) {

            lcd.clear();
            lcd.home();
      
            lcd.print("Lumenbot");
            lcd.setCursor(0, 1);
            lcd.print("activated");

            light_on = true;
            
             myservo.attach(servo_pin);
              myservo.write(strike_up);
              delay(500);
              myservo.detach();
             digitalWrite(led_L, LOW);
          }
          
        } else if(tou_state == 2) {

          if(!msg_displayed) {
            lcd.setPWM(REG_RED, 255);
            lcd.setPWM(REG_GREEN, 10);
            lcd.setPWM(REG_BLUE, 10);
            
            lcd.clear();
            lcd.home();
      
            lcd.print("Peak capacity-");
            lcd.setCursor(0, 1);
            lcd.print("On is disabled");
          }
          
        }

    if(!msg_displayed) msg_displayed = true;
    
  }

  }
  
}

void updateOffButton() {
  
  button_R_prev = button_R_current;
  button_R_current = digitalRead(button_R);

  // pressing / debouncing
  if(button_R_prev == LOW && button_R_current == HIGH && current_time-button_R_down > 20) {
    button_R_down = current_time;
    button_R_state = 1;
    Serial.print("OFF!");
    offButtonPressed();
  }




  if(button_R_prev == HIGH && button_R_current == LOW && current_time-button_R_down > 20) {
    button_R_state = 0;
    Serial.println("OFF! released");
    offButtonReleased();    


      if(CURRENT_STATE == CYCLE_MODE) {

      if(tou_state == 0) {

          lcd.setPWM(REG_RED, 10);
          lcd.setPWM(REG_GREEN, 255);
          lcd.setPWM(REG_BLUE, 10);

          light_on = false;
          
          lcd.clear();
          lcd.home();

          lcd.print("Thank you!");
          
        } else if(tou_state == 1) {

          lcd.setPWM(REG_RED, 200);
          lcd.setPWM(REG_GREEN, 200);
          lcd.setPWM(REG_BLUE, 10);

          light_on = false;
        
          lcd.clear();
          lcd.home();
    
          lcd.print("Thank you!");
          
        } else if(tou_state == 2) {

          lcd.setPWM(REG_RED, 255);
          lcd.setPWM(REG_GREEN, 10);
          lcd.setPWM(REG_BLUE, 10);

          light_on = false;
          
          lcd.clear();
          lcd.home();
    
          lcd.print("Try again when");
          lcd.setCursor(0, 1);
          lcd.print("not on peak load");
          
        }

      }
      


  


  // wait 100ms before saying that the button is being held down
  if(button_R_prev == HIGH && button_R_current == HIGH && current_time-button_R_down > 20) {
    // holding
    long holding_R = current_time-button_R_down;
    button_R_state = 2;
    Serial.print("OFF! "); Serial.println(holding_R);
  }

}
  
}

void onButtonPressed() {

  digitalWrite(led_L, HIGH);

  if(CURRENT_STATE == AMBIENT_STATE) {
    // change to message state
  }

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

  light_on_time = minute + (60*hour);

  if(CURRENT_STATE == LIVE_MODE) {

light_on_seconds = second + (60*minute) + (60*60*hour);
  light_on = true;
    
    myservo.attach(servo_pin);
              myservo.write(strike_up);
              delay(500);
              myservo.detach();
            digitalWrite(led_L, LOW);
  }
  
  
}

void offButtonPressed() {

  digitalWrite(led_R, HIGH);

  if(CURRENT_STATE == AMBIENT_STATE) {
    // brighten the lcd screen for 5 secs
  }
  
}


void onButtonReleased() {
  /*
  myservo.attach(servo_pin);
      myservo.write(strike_up);
      delay(500);
      myservo.detach();
      */
    digitalWrite(led_L, LOW);
    
}


void offButtonReleased() {
  
  myservo.attach(servo_pin);
      myservo.write(strike_down);
      delay(500);
      myservo.detach();
      
      digitalWrite(led_R, LOW);

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

  light_off_time = minute + (60*hour);

  total_light_on_time = abs(light_off_time - light_on_time);

  Serial.print("the light was on for a total of ");
  Serial.print(total_light_on_time);
  Serial.println(" mins");

  light_on  = false;

  if(CURRENT_STATE == LIVE_MODE) {
    
    myservo.attach(servo_pin);
              myservo.write(strike_down);
              delay(500);
              myservo.detach();
            digitalWrite(led_L, LOW);
  }

  // TODO:
  // display this to lcd screen
  // save data to eeprom
  // eventually implement timer functionality

  // other things needed in eeprom:
  // current light state (on or off)
  // number of times light turned on & off

}




