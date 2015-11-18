
void timeOfUsage() {

  readRTC();

  float off_peak_cost = 0.083;
  float mid_peak_cost = 0.128;
  float on_peak_cost = 0.175;
  
  float result_cost = 0.0;
  int hour_offset = -1; // dst

  if(thetime.month >= 11 || thetime.month < 5) {
    // winter

    if(thetime.weekDay == 0 || thetime.weekDay == 6) {
      // weekend
      // off peak
      result_cost = off_peak_cost;
      
    } else {
      // weekday
      
      if(thetime.hour >= (7+hour_offset) && thetime.hour < (11+hour_offset)) {
        // on peak
        result_cost = on_peak_cost;
      } else if(thetime.hour >= (11+hour_offset) && thetime.hour < (17+hour_offset)) {
        // mid peak
        result_cost = mid_peak_cost;
      } else if(thetime.hour >= (17+hour_offset) && thetime.hour < (19+hour_offset)) {
        // on peak
        result_cost = on_peak_cost;
      } else {
        // off peak
        result_cost = off_peak_cost;
      }
      
    }
    
  } else if(thetime.month >= 5 || thetime.month < 11) {
    // summer

    if(thetime.weekDay == 0 || thetime.weekDay == 6) {
      // weekend
      // off peak
      result_cost = off_peak_cost;
    } else {
      // weekday

      if(thetime.hour >= (7+hour_offset) && thetime.hour < (11+hour_offset)) {
        // mid peak
        result_cost = mid_peak_cost;
      } else if(thetime.hour >= (11+hour_offset) && thetime.hour < (17+hour_offset)) {
        // on peak
        result_cost = on_peak_cost;
      } else if(thetime.hour >= (17+hour_offset) && thetime.hour < (19+hour_offset)) {
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

