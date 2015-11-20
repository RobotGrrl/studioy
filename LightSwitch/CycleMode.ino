void cycleMode() {

  // cycle the time of usages
  if(current_time-last_tou_change >= long(30*1000) || last_tou_change == 0) {
    tou_state++;
    if(tou_state > 2) tou_state = 0;
    last_tou_change = current_time;
  }

  // cycle the messages
  if(current_time-last_msg_flip >= long(5*1000) || last_msg_flip == 0) {
    
     cur_msg++;
     if(cur_msg > 4) cur_msg = 0;
     last_msg_flip = current_time;

     // set colour according to time of usage
     if(tou_state == 0) {
       lcdSetColour(10, 255, 10);
     } else if(tou_state == 1) {
       lcdSetColour(200, 200, 10);
     } else if(tou_state == 2) {
       lcdSetColour(255, 10, 10);
     }
 
     lcd.clear();
     lcd.home();

     if(cur_msg == 0) {

        readDHT22();
        String s1 = "Temp: " + (String)DHT.temperature + " C";
        String s2 = "Humidity: " + (String)DHT.humidity + "%";
      
        lcd.print(s1);
        lcd.setCursor(0, 1);
        lcd.print(s2);
      
     } else if(cur_msg == 1) {

        lcd.print("Grid status:");
        lcd.setCursor(0, 1);
      
       if(tou_state == 0) {
          lcd.print("Off peak");
        } else if(tou_state == 1) {
          lcd.print("Mid peak");
        } else if(tou_state == 2) {
          lcd.print("On peak");
        }

      
     } else if(cur_msg == 2) {

        lcd.print("The grid is");
        lcd.setCursor(0, 1);

        if(tou_state == 0) {
          lcd.print("capacity ready");
        } else if(tou_state == 1) {
          lcd.print("becoming full");
        } else if(tou_state == 2) {
          lcd.print("PEAK CAPACITY");
        }

     } else if(cur_msg == 3) {

        lcd.print("Electricity rate:");
        lcd.setCursor(0, 1);

        if(tou_state == 0) {
          lcd.print("8.3 cents/kWh");
        } else if(tou_state == 1) {
          lcd.print("12.8 cents/kWh");
        } else if(tou_state == 2) {
          lcd.print("17.5 cents/kWh");          
        }

     }


  }

  
}

