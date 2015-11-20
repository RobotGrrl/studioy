void liveMode() {

if(current_time-last_update_live_mode < 1000) return;

  lcd.clear();
  lcd.home();

  if(light_on) {

    lcdSetColour(200, 200, 200);

    readRTC();
    current_time_seconds = thetime.second + (60*thetime.minute) + (60*60*thetime.hour);
    elapsed_on_time = abs( current_time_seconds - light_on_seconds );
  
    int bulb_watts = 600;
    float kilojoules =  ((float)bulb_watts * (float)elapsed_on_time) / 1000.0;

    /*
    if(elapsed_on_time%(5) == 0) {
      playTone(700, 80);
      delay(80);
      playTone(300, 80);
      delay(80);
    }
    */
    
    // $0.25 / kWh
    // convert s -> h ....... s/60/60
    // convert w -> kW ....... w/1000
    
    // off peak cost
    //float cost = 0.083 * ( (float)elapsed_on_time/60/60 ) * ( (float)bulb_watts/1000 );
    
    // apocalypse cost
    float cost = 10000.0 * ( (float)elapsed_on_time/60/60 ) * ( (float)bulb_watts/1000 );
  
    String s1 = "Light on: ";
    String s_meep;// = (String)elapsed_on_time + "s";
    String s2;
  
    int num_mins;
    int num_secs;
  
    if(elapsed_on_time >= 60) {

      if(elapsed_on_time%60 == 0) elapsed_on_mins++;

      int num_mins = elapsed_on_mins;
      int num_secs = elapsed_on_time-(60*num_mins);
      
      if(num_secs < 10) {
        s_meep = (String)num_mins + ":0" + (String)num_secs;
      } else {
        s_meep = (String)num_mins + ":" + (String)num_secs;
      }
      
    } else {
  
      if(elapsed_on_time < 10) {
        s_meep = "0:0" + (String)elapsed_on_time;
      } else {
        s_meep = "0:" + (String)elapsed_on_time;
      }
      
    }
  
    //Serial.println(s_meep);
    
    String the_kj = (String)kilojoules;
    for(int i=0; i<the_kj.length(); i++) {
      if( the_kj[i] == '.' ) {
         the_kj.remove(i+3); // remove everything except the last two digits 
      }
    }
    
    s2 = " " + the_kj + "kJ = $" + cost;
    
    lcd.setCursor(0, 0);
    lcd.print(s1);
    
    lcd.setCursor(10, 0);
    lcd.print(s_meep);
    
    lcd.setCursor(0, 1);
    lcd.print(s2);

  
  } else {

    lcdSetColour(80, 80, 100);

    String s1 = "Lumenbot not";
    String s2 = "activated";

    lcd.setCursor(calcCenter(s1), 0);
    lcd.print(s1);
    
    lcd.setCursor(calcCenter(s2), 1);
    lcd.print(s2);
    
  }

  last_update_live_mode = current_time;

}

