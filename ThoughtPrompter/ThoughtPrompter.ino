
int ledr = 11;
int ledp = 10;

int buttonr = 8;
int buttonp = 9;

long current_time = 0;
long lastr = 0;
long lastp = 0;

boolean isrec = false;
boolean isplay = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(ledr, OUTPUT);
  pinMode(ledp, OUTPUT);
  pinMode(buttonr, INPUT);
  pinMode(buttonp, INPUT);
  
  digitalWrite(ledr, HIGH);
  digitalWrite(ledp, HIGH);
  delay(100);
  digitalWrite(ledr, LOW);
  digitalWrite(ledp, LOW);
  delay(100);
  
}

void loop() {
  
  current_time = millis();
  
  /*
  if(digitalRead(buttonr) == HIGH && isrec == false) {
   isrec = true;
   Serial.print("r");
   digitalWrite(ledr, HIGH);
   digitalWrite(ledp, LOW);
  }
  
  if(digitalRead(buttonr) == LOW && isrec == true) {
    isrec = false;
    Serial.print("r");
    digitalWrite(ledr, LOW);
    digitalWrite(ledp, LOW);
    isplay = true; 
  }
  */
  
  
  if(digitalRead(buttonr) == HIGH && current_time-lastr >= 500) {
  
    Serial.print("r");
    
    isrec = !isrec;
    
    if(isrec) {
    digitalWrite(ledr, HIGH);
    digitalWrite(ledp, LOW);
    } else {
      digitalWrite(ledr, LOW);
    digitalWrite(ledp, LOW);
    }
    
    lastr = current_time;
    
  }
  
  
  if(digitalRead(buttonp) == HIGH && current_time-lastp >= 100) {
    
    //if(isplay) {
      Serial.print("p");
      digitalWrite(ledr, LOW);
      digitalWrite(ledp, HIGH);
    //} else {
    //  digitalWrite(ledr, LOW);
    //  digitalWrite(ledp, LOW);
    //}
    
          isplay = !isplay;
    lastp = current_time;
    
  }
  
  /*
  if(isplay == true && current_time-lastp >= 10*1000) {
    digitalWrite(ledr, LOW);
    digitalWrite(ledp, LOW);
    isplay = false;
  }
  */
  
}


