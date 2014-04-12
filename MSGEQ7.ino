void readEQ(byte *spectrumValue){
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  for (int i = 0; i < 6; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);
     digitalWrite(strobePin, HIGH);
     Serial.print(spectrumValue[i]);
     Serial.print(",");
     
    
  }
  Serial.println();

}

