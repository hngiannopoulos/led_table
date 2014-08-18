#ifndef SNAKE_h
#define SNAKE_h

// the #include statment and code go here..
#include "Arduino.h"
#include "FastLED.h"


#define movingUp    0
#define movingDown  1
#define movingLeft  2
#define movingRight 3

#define NUM_LEDS 100
#define turnRight   4 
#define turnLeft    5
#define noTurn      6 
#define endOfSnake  0xFF

//SnakeStates
#define resetAll 0
#define lookingForDirection 1
#define deathAnimation 2

//#define framesPerMovement 7

#define snakeR     0         //1 - Color on  0 -color off
#define snakeG     1         //1 - Color on  0 -color off
#define snakeB     0         //1 - Color on  0 -color off

#define snakeDeathR     1         //1 - Color on  0 -color off
#define snakeDeathG     1         //1 - Color on  0 -color off
#define snakeDeathB     0         //1 - Color on  0 -color off

#define appleR     1         //1 - Color on  0 -color off
#define appleG     0         //1 - Color on  0 -color off
#define appleB     0         //1 - Color on  0 -color off

#define CORDINATE(X, Y) ((Y)%2)?(((10*((Y)+1))-1)-(X)):(10*(Y))+(X)  

class snake{
public:

  snake(struct CRGB * ledDisplay);
  void run();
  void snakeInit();

private:

  void drawSnake();
  byte snakeMove(byte turning);
  byte determineQuadrant(byte headX, byte headY, byte appleX, byte appleY);            
  byte pointToBCD(char x, char y);
   
  byte makeApple();
  //globals
  byte snakeArray[75]; //sored in BCD (xy) where 0xFF is equivalent to the null therminator 
  //The array is backwards, the head is at the highest value  snakeHead = snake[snakeLenght]
  byte snakeDirection;
  byte snakeLength;
  byte apple;
  byte snakeState;
  struct CRGB * disp;

};
#endif
