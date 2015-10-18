#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

int mode = 0;
long last_switch = 0;
long current_time = 0;
int breath = 0;
boolean breath_dir = true;
long last_incr = 0;

void setup() {
  Serial.begin(9600);
  Serial.print("hello");
  lcd.begin(16,2);
}

void loop() {

  current_time = millis();

  
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
 
  Serial.print("~");
  delay(100);

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




