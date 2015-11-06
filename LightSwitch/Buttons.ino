
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
    onButtonReleased();
  }

  // wait 100ms before saying that the button is being held down
  if(button_L_prev == HIGH && button_L_current == HIGH && current_time-button_L_down > 20) {
    // holding
    long holding_L = current_time-button_L_down;
    button_L_state = 2;
    Serial.print("ON! "); Serial.println(holding_L);
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
  }

  // wait 100ms before saying that the button is being held down
  if(button_R_prev == HIGH && button_R_current == HIGH && current_time-button_R_down > 20) {
    // holding
    long holding_R = current_time-button_R_down;
    button_R_state = 2;
    Serial.print("OFF! "); Serial.println(holding_R);
  }
  
}

void onButtonPressed() {

  digitalWrite(led_L, HIGH);

  if(CURRENT_STATE == AMBIENT_STATE) {
    // change to message state
  }
  
}

void offButtonPressed() {

  digitalWrite(led_R, HIGH);

  if(CURRENT_STATE == AMBIENT_STATE) {
    // brighten the lcd screen for 5 secs
  }
  
}


void onButtonReleased() {
  myservo.attach(servo_pin);
      myservo.write(strike_up);
      delay(500);
      myservo.detach();
    digitalWrite(led_L, LOW);
}


void offButtonReleased() {
  
  myservo.attach(servo_pin);
      myservo.write(strike_down);
      delay(500);
      myservo.detach();
      digitalWrite(led_R, LOW);
}




