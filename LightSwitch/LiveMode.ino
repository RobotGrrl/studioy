void liveMode() {

if(current_time-last_update_live_mode < 1000) return;

  readRTC();
  
  current_time_seconds = thetime.second + (60*thetime.minute) + (60*60*thetime.hour);
  
  if(light_on) {
    elapsed_on_time = abs( current_time_seconds - light_on_seconds );
  }
  
  int bulb_watts = 60;
  float kilojoules =  ((float)bulb_watts * (float)elapsed_on_time) / 1000.0;
  
  // $0.25 / kWh
  // convert s -> h ....... s/60/60
  // convert w -> kW ....... w/1000
  
  // off peak cost
  //float cost = 0.083 * ( (float)elapsed_on_time/60/60 ) * ( (float)bulb_watts/1000 );
  
  // apocalypse cost
  float cost = 5.0 * ( (float)elapsed_on_time/60/60 ) * ( (float)bulb_watts/1000 );

  String s1 = "Light on: ";
  String s_meep = (String)elapsed_on_time + "s";
  String s2;
  
  String the_kj = (String)kilojoules;
  for(int i=0; i<the_kj.length(); i++) {
    if( the_kj[i] == '.' ) {
       the_kj.remove(i+3); // remove everything except the last two digits 
    }
  }
  
  s2 = the_kj + "kJ $" + cost;
  
  lcd.setCursor(0, 0);
  lcd.print(s1);
  
  lcd.setCursor(10, 0);
  lcd.print(s_meep);
  
  lcd.setCursor(0, 1);
  lcd.print(s2);
  
  last_update_live_mode = current_time;

}

