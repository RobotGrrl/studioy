void creditsMode() {

  if(current_time-last_msg_flip >= long(3*1000) || last_msg_flip == 0) {

    cur_msg++;
    if(cur_msg > 9) cur_msg = 0;
    last_msg_flip = current_time;

    lcdSetColour(66, 167, 255);
    
    lcd.clear();
    lcd.home();

    String s1;
    String s2;

    switch(cur_msg) {
      case 0:
        s1 = "Lumenbot brought";
        s2 = "to life";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 1:
        s1 = "during learning";
        s2 = "project at";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 2:
        s1 = "Studio[Y] MaRS";
        s2 = "in Toronto, ON";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 3:
        s1 = "Shocking";
        s2 = "energy team:";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 4:
        s1 = "Rebecca";
        s2 = "Wolff";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 5:
        s1 = "David";
        s2 = "Lawless";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 6:
        s1 = "Fatin";
        s2 = "Chowdhury";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 7:
        s1 = "Erin RobotGrrl";
        s2 = "Kennedy";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 8:
        s1 = "Lumenbot";
        s2 = "BEEP BEEP BEEP";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
      case 9:
        s1 = ";)";
        s2 = "";
        lcd.setCursor(calcCenter(s1), 0);
        lcd.print(s1);
        lcd.setCursor(calcCenter(s2), 1);
        lcd.print(s2);
      break;
    }
 
  }
  
}


int calcCenter(String s) {

  int l = s.length();
  int place = (16-l)/2;
  return place;
  
}


