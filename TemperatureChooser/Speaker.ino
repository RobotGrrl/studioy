void playTone(uint16_t tone, uint16_t duration) {
  
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(spkr, HIGH);
    delayMicroseconds(tone);
    digitalWrite(spkr, LOW);
    delayMicroseconds(tone);
  }

}
