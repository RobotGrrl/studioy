// printFloat prints out the float 'value' rounded to 'places' places after the decimal point
String printFloat(float value, int places) {

  String meep = "";
  
  // this is used to cast digits 
  int digit;
  float tens = 0.1;
  int tenscount = 0;
  int i;
  float tempfloat = value;

    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
  // if this rounding step isn't here, the value  54.321 prints as 54.3209

  // calculate rounding term d:   0.5/pow(10,places)  
  float d = 0.5;
  if (value < 0)
    d *= -1.0;
  // divide by ten for each decimal place
  for (i = 0; i < places; i++)
    d/= 10.0;    
  // this small addition, combined with truncation will round our values properly 
  tempfloat +=  d;

  // first get value tens to be the large power of ten less than value
  // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

  if (value < 0)
    tempfloat *= -1.0;
  while ((tens * 10.0) <= tempfloat) {
    tens *= 10.0;
    tenscount += 1;
  }


  // write out the negative if needed
  if (value < 0)
    meep += '-';

  if (tenscount == 0)
    meep += 0;

  for (i=0; i< tenscount; i++) {
    digit = (int) (tempfloat/tens);
    meep += digit;
    tempfloat = tempfloat - ((float)digit * tens);
    tens /= 10.0;
  }

  // if no places after decimal, stop now and return
  if (places <= 0)
    return meep;

  // otherwise, write the point and continue on
  meep += ".";

  // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
  for (i = 0; i < places; i++) {
    tempfloat *= 10.0; 
    digit = (int) tempfloat;
    meep += digit;
    // once written, subtract off that digit
    tempfloat = tempfloat - (float) digit; 
  }

    return meep;
  
}



void servoTurnLightOff() {
  myservo.attach(servo_pin);
  myservo.write(strike_down);
  delay(500);
  myservo.detach();
}


void servoTurnLightOn() {
  myservo.attach(servo_pin);
  myservo.write(strike_up);
  delay(500);
  myservo.detach();
}

void lcdSetColour(int r, int g, int b) {
  lcd.setPWM(REG_RED, r);
  lcd.setPWM(REG_GREEN, g);
  lcd.setPWM(REG_BLUE, b);
}



void servoTest() {
 for(int i=0; i<180; i++) {
    myservo.write(i);
    delay(10);
  }
  delay(100);
  for(int i=180; i>0; i--) {
    myservo.write(i);
    delay(10);
  }
  delay(100);
}


void playTone(int tone, int duration) {

  if(MUTE) return;
 
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(spkr, HIGH);
    delayMicroseconds(tone);
    digitalWrite(spkr, LOW);
    delayMicroseconds(tone);
  }
  
}

