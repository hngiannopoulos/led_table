#ifndef DRAW_h
#define DRAW_h

#include "Arduino.h"
#include "FastLED.h"
#include "font.h"

#define CORDINATE(X, Y) ((Y)%2)?(((10*((Y)+1))-1)-(X)):(10*(Y))+(X)  

class draw{

public:

draw(struct CRGB ledDisplay[]);
/*
    INPUTS- WheelPos (0-0x5ff)
 RETURNS a CRGB struct of a color of the rainbow
 
 */
struct CRGB Wheel(int WheelPos);

/*
  Uses this formula to draw a line from two points in CRGB struct color
 INPUTS: x0, y0, x1, y1, disp - pointer to display matrix, color- CRGB struct of the color to draw the line in.
 */
void drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, struct CRGB color);

/*
    Draw Sprite
 */
void drawSprite(struct CRGB* disp, byte xPos, byte yPos, word *sprite, struct CRGB color);

/*
    INPUTS: Character, xPos, yPos, disp, color
 Places character on screen at xPos, yPos
*/
void placeChar(char Character, byte xPos, byte yPos, struct CRGB color);



/*
    INPUTS: string[] - string to display. yPos- y position of the scrolling text on the display. 
 scanPosition - what portion of the buffer to print on screen. disp - pointer to display matrix
 color- color of the text. 
 OUTPUT: PositionInBuffer - 10 - 
 HOW TO CALL:  for(char i = 0; i<=scrollText(message, 1,i, leds, Wheel(50)); i++){
 */
byte scrollText(char string[], byte yPos, byte scanPosition, struct CRGB color);

int linearPWM(int percentage);

private:
  struct CRGB * disp;

};

#endif