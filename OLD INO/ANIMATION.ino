/*

Position must range between 0 and 35

*/
void lookupPos(byte Position, byte *xVal, byte *yVal, struct CRGB color){ 
  byte value;
    
   value = EEPROM.read(Position);
   *yVal = value & B00001111;
   *xVal = value>>4;
  
}


