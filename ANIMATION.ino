/*

Position must range between 0 and 35

*/
void lookupPos(byte Position, byte *xVal, byte *yVal, struct CRGB color){ 
  byte value;
    
   value = EEPROM.read(Position);
   *yVal = value & B00001111;
   *xVal = value>>4;
  
}

void makeRainbow(struct CRGB *disp, unsigned int hueStart) {
	unsigned char i;
	//unsigned int hue = hueStart;

	for(i = 0; i < 10; i++){
            drawLine(0, i, i, 0, disp, Wheel((hueStart + (i * rainbowWidth)) % 0x0600)); 
      }
      for(i = 1; i<10; i++){
          drawLine(i,9,9,i, disp, Wheel((hueStart + ((i+9) * rainbowWidth)) % 0x0600));
      }
  
}

