
byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void printDate() {

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

}

void readRTC() {
  
  // read the RTC
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  
  Wire.requestFrom(DS1307_ADDRESS, 7);
  
  thetime.second = bcdToDec(Wire.read());
  thetime.minute = bcdToDec(Wire.read());
  thetime.hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  thetime.weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  thetime.monthDay = bcdToDec(Wire.read());
  thetime.month = bcdToDec(Wire.read());
  thetime.year = bcdToDec(Wire.read());
  
}


void displayTimeDuration() {

  Serial.print("Elapsed duration: ");

  elapsedtime.month = 

  Serial.print(thetime.month);
    Serial.print("/");
    Serial.print(thetime.weekDay);
    Serial.print("/");
    Serial.print(thetime.year);
    Serial.print(" ");
    Serial.print(thetime.hour);
    Serial.print(":");
    Serial.print(thetime.minute);
    Serial.print(":");
    Serial.println(thetime.second);
  
}





