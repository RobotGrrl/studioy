
void updateDButton() {

  button_D_prev = button_D_current;
  button_D_current = digitalRead(button_D);

  // pressing / debouncing
  if(button_D_prev == LOW && button_D_current == HIGH && current_time-button_D_up > 20) {
    button_D_down = current_time;
    button_D_state = 1;
    Serial.println("DETAIL!");
    dButtonPressed();
  }

  if(button_D_prev == HIGH && button_D_current == LOW && current_time-button_D_down > 20) {
    button_D_state = 0;
    Serial.println("DETAIL! released");
    button_lock = false;
    msg_displayed = false;
    dButtonReleased();
  }

  // wait 100ms before saying that the button is being held down
  if(button_D_prev == HIGH && button_D_current == HIGH && current_time-button_D_down > 20) {
    // holding
    long holding_D = current_time-button_D_down;
    button_D_state = 2;
    Serial.print("DETAIL! "); Serial.println(holding_D);
  }

}

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

      lcd.clear();
      lcd.home();
  
      if(tou_state == 0) {
  
        if(!msg_displayed) {
          lcdSetColour(10, 255, 10);
    
          lcd.setCursor(0, 0);
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
    
          lcd.setCursor(0, 0);
          lcd.print("Lumenbot");
          lcd.setCursor(0, 1);
          lcd.print("activated");
    
          light_on = true;
          
          servoTurnLightOn();
          digitalWrite(led_L, LOW);
        }
            
      } else if(tou_state == 1) {
    
        if(!msg_displayed) {
          lcdSetColour(200, 200, 10);
    
          lcd.setCursor(0, 0);
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
  
            lcd.setCursor(0, 0);
            lcd.print("Lumenbot");
            lcd.setCursor(0, 1);
            lcd.print("activated");
  
            light_on = true;
            
            servoTurnLightOn();
             digitalWrite(led_L, LOW);
          }
            
    } else if(tou_state == 2) {
  
      if(!msg_displayed) {
        lcdSetColour(255, 10, 10);
  
        lcd.setCursor(0, 0);
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
  
        light_on = false;
          
        lcd.clear();
        lcd.home();
  
        if(tou_state == 0) {
    
            lcdSetColour(10, 255, 10);
            
            lcd.setCursor(0, 0);
            lcd.print("Thank you!");
            
          } else if(tou_state == 1) {
    
            lcdSetColour(200, 200, 10);
    
            lcd.setCursor(0, 0);
            lcd.print("Thank you!");
            
          } else if(tou_state == 2) {
    
            lcdSetColour(255, 10, 10);
    
            lcd.setCursor(0, 0);
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

  readRTC();
  
  light_on_time = thetime.minute + (60*thetime.hour);

  light_start_s = convertToSeconds(thetime.hour, thetime.minute, thetime.second);
  
  if(CURRENT_STATE == LIVE_MODE) {
    light_on_seconds = thetime.second + (60*thetime.minute) + (60*60*thetime.hour);
    light_on = true;
    servoTurnLightOn();

    // output some data
    Serial.print("Light turned on: ");
    printTheTime();
  }
  
}

void offButtonPressed() {
  digitalWrite(led_R, HIGH);
}

void onButtonReleased() {
  digitalWrite(led_L, LOW);
}

void offButtonReleased() {
  
  servoTurnLightOff();
  digitalWrite(led_R, LOW);
  light_on  = false;

  readRTC();

  // this was probably in minutes, now it's seconds... have to clean this code
  light_off_time = convertToSeconds(thetime.hour, thetime.minute, thetime.second);
  
  if(CURRENT_STATE == LIVE_MODE) {
    servoTurnLightOff();
    digitalWrite(led_L, LOW);

    // output some data
    Serial.print("Light turned off: ");
    printTheTime();
  }

  // calculating how long the light has been on for
  light_end_s = convertToSeconds(thetime.hour, thetime.minute, thetime.second);
  light_duration = abs(light_end_s - light_start_s);

  Serial.print("Elapsed duration (");
  Serial.print(light_duration);
  Serial.print("s) ");
  printConvertToHMS(light_duration);
  Serial.print("\n");

}



void dButtonPressed() {
  // nothing to do really
}

void dButtonReleased() {
  // switch modes

  playTone(400, 80);
  delay(80);

  mode_press++;
  if(mode_press > 3) mode_press = 0;

  if(mode_press == 0) {
    last_update_live_mode = 0;
    CURRENT_STATE = LIVE_MODE;
  }

  if(mode_press == 1) {
    last_msg_flip = 0;
    CURRENT_STATE = CYCLE_MODE;
  }
  
  if(mode_press == 2) {
    last_update = 0;
    CURRENT_STATE = AMBIENT_MODE;
  }
  
  if(mode_press == 3) {
    cur_msg = 9; // just hacking how it advances quickly at the start
    last_msg_flip = 0;
    CURRENT_STATE = CREDITS_MODE;
  }
  
}

