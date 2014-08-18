#include "draw.h"
#include "FastLED.h"
draw::draw(struct CRGB ledDisplay[]){
   disp = ledDisplay;         //copy display buffer 
}

/*
      INPUTS- WheelPos (0-0x5ff)
 RETURNS a CRGB struct of a color of the rainbow
 
 */
struct CRGB draw::Wheel(int WheelPos){
   switch (WheelPos >> 8) {
      
   /* We are between R and RG */
   case 0:
      return CRGB(0xFF, WheelPos & 0xFF, 0x00);
   
   /* We are between RG and G */
   case 1:
      return CRGB(~(WheelPos & 0xFF), 0xFF, 0x00);
   
   /* We are between G and GB */
   case 2:
      return CRGB(0x00, 0xFF, WheelPos & 0xFF);
      
   /* We are between GB and B */
   case 3:
      return CRGB(0x00, ~(WheelPos & 0xFF), 0xFF);
      
   /* We are between B and BR */
   case 4:
      return CRGB(WheelPos & 0xFF, 0x00, 0xFF);
      
   /* We are between BR and R */
   case 5:
      return CRGB(0xFF, 0x00, ~(WheelPos & 0xFF));
      
   /* Shouldn't get here */
   default:
      return CRGB(255, 255, 255);
   }
}

/*
      INPUTS - WheelPos(0-255); brightness(0-7), 0 is brighter 7 is dimmer
 RETURNS, same as other Wheel, but this function takes brightness into account
 
 */

/*struct CRGB Wheel(byte WheelPos, char brightness)
{
   if (WheelPos < 85) {
      return CRGB((unsigned char)((WheelPos * 3)>>(brightness)), (unsigned char)((255 - WheelPos * 3)>>(brightness)), 0);
   } 
   else if (WheelPos < 170) {
      WheelPos -= 85;
      return CRGB((unsigned char)((255 - WheelPos * 3)>>(brightness)), 0, (unsigned char)((WheelPos * 3)>>(brightness)));
   } 
   else {
      WheelPos -= 170; 
      return CRGB(0, (unsigned char)((WheelPos * 3)>>(brightness)), (unsigned char)((255 - WheelPos * 3)>>(brightness)));
   }
}
*/

/*
      RETURNS the number light in the string based on x and y CORDINATEs
 */


/*
   Uses this formula to draw a line from two points in CRGB struct color
 INPUTS: x0, y0, x1, y1, disp - pointer to display matrix, color- CRGB struct of the color to draw the line in.
 */

void draw::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, struct CRGB color) {
   int16_t temp = 0;
   int16_t steep = abs(y1 - y0) > abs(x1 - x0);
   if (steep) {
      temp = x0;   
      x0 = y0; 
      y0 = temp; //swap(x0, y0);
      temp = x1;   
      x1 = y1; 
      y1 = temp; // swap(x1, y1);
   }
   if (x0 > x1) {
      temp = x0; 
      x0 = x1; 
      x1 = temp; //swap(x0, x1);
      temp = y0;    
      y0 = y1; 
      y1 = temp;   //swap(y0, y1);
   }

   int16_t dx, dy;
   dx = x1 - x0;
   dy = abs(y1 - y0);

   int16_t err = dx / 2;
   int16_t ystep;

   if (y0 < y1) {
      ystep = 1;
   } 
   else {
      ystep = -1;
   }

   for (; x0<=x1; x0++) {
      if (steep) {
         disp[CORDINATE(y0,x0)] = color;         //drawPixel(y0, x0, color);
      } 
      else {
         disp[CORDINATE(x0,y0)] = color;               // drawPixel(x0, y0, color);
      }
      err -= dy;
      if (err < 0) {
         y0 += ystep;
         err += dx;
      }
   }
}



/*
      Draw Sprite
 */
void draw::drawSprite(struct CRGB* disp, byte xPos, byte yPos, word *sprite, struct CRGB color){
   byte scan = 0;
   for(byte y = yPos; y<10; y++){
      for(byte x = xPos; x<10; x++){
         disp[CORDINATE(x, y)] = (bitRead(sprite[scan], 15 - x) == 0)? CRGB(0,0,0) : color;
      }
      scan++;
   } 
}

/*
      INPUTS: Character, xPos, yPos, disp, color
 Places character on screen at xPos, yPos
 */

void draw::placeChar(char Character, byte xPos, byte yPos, struct CRGB color){
   byte horiz = 0;
   for(int x = xPos; x <= 4+xPos; x++){

      byte temp = pgm_read_byte_near(((font5x7[Character-48])+horiz));

      for(int y = yPos; y < 8+yPos; y++){
         if(((temp>>(7-(y-yPos))) & B00000001) != 0)
            disp[CORDINATE(x,y)] = CRGB(255,0,0);
      }
      horiz++;
   }
}




/*
      INPUTS: string[] - string to display. yPos- y position of the scrolling text on the display. 
 scanPosition - what portion of the buffer to print on screen. disp - pointer to display matrix
 color- color of the text. 
 OUTPUT: PositionInBuffer - 10 - 
 HOW TO CALL:   for(char i = 0; i<=scrollText(message, 1,i, leds, Wheel(50)); i++){
 
 
 */

void draw::setString(char string[]){
   int positionInBuffer = 0;
   int stringLength     = 0;
   char letter;
   scanPosition = 0;
   memset(messageBuffer, 0x00, MAX_BUFFER_LEN * sizeof(uint8_t));

   while((string[stringLength] != 0) && (string[stringLength] != 0x0D)){            //Determine length of the string
      stringLength++;
   }
   bufferLength = (stringLength*6)+20;
   //memset(messageBuffer, 0, stringLength*6);

   //The array is 6x as long as the length of the input string
   // ADD 10 Columns Blank in the beginning of the message
   for(byte i = 0; i<10; i++){
      messageBuffer[positionInBuffer++] = 0;
   }
   //copies data for each character into a message buffer
   for(byte character = 0; character < stringLength; character++){             //for each charater in the string
      letter = string[character];                                              //copy ASCII value from string


         for(byte verticalData = 0; verticalData <5; verticalData++){              //for each vertical byte of a letter
            byte temp = pgm_read_byte_near(((font5x7[letter-32])+verticalData));   //copy data from progmem
            //copy data from the font to its respective spot in message buffer
            messageBuffer[positionInBuffer++] = temp;                        
         }
         messageBuffer[positionInBuffer++] = 0x00;
   }
   //ADD 10 Columns at the end of the message
   for(byte i = 0; i<10; i++){
      messageBuffer[positionInBuffer++] = 0x00;
   }

}


byte draw::scrollText(struct CRGB color){
   int position = (scanPosition >> 1);
   //Print characterBuffer to the screen 
   memset(disp, 0, NUM_LEDS * sizeof(struct CRGB));
   for(byte x = position;   x < (10+position) ; x++){
      for(byte y = 0; y < (8); y++){
         if(((messageBuffer[x]>>(7-(y))) & B00000001) != 0)
            disp[CORDINATE(x-position,(y+2))] = color;
      }
   }
   if(position < bufferLength){
      scanPosition++;
   }
   else{
      scanPosition = 0;
   }
   //scanPosition = (scanPosition < bufferLength) ? scanPosition++ : 5;
}


// returns a pwm value (0..255) for a required percentage (0..100) 
// as to provide a linear fade as perceived by eye
int draw::linearPWM(int percentage){
   // coefficients
   double a = 9.7758463166360387E-01;
   double b = 5.5498961535023345E-02;
   return floor((a * exp(b*percentage)+.5))-1;
}



















