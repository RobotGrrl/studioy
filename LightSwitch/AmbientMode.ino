void ambientMode() {

  if(current_time-last_update >= 1000) {
    readDHT22();
    statusDisplay();
    last_update = current_time;
  }

  /*
  if(current_time-last_print_date >= 1000) {
    printDate();
    timeOfUsage();
    last_print_date = current_time;
  }
  */
  
}


void statusDisplay() {

  lcd.clear();
  lcd.home();

  lcdSetColour(200, 50, 150);

  String s1 = "Temp: " + (String)DHT.temperature + " C";
  String s2 = "Humidity: " + (String)DHT.humidity + "%";

  lcd.setCursor(0, 0);
  lcd.print(s1);
  lcd.setCursor(0, 1);
  lcd.print(s2);
  
}


