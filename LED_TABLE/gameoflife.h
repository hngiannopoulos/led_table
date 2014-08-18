#ifndef GAMEOFLIFE_h
#define GAMEOFLIFE_h

#include "Arduino.h"
#include "FastLED.h"

#define CORDINATE(X, Y) ((Y)%2)?(((10*((Y)+1))-1)-(X)):(10*(Y))+(X)  

#define GOL_BUFFER_SIZE 16 // must be a power of 2 
#define GOL_BUFFER_WRAP(i) ((i) & (GOL_BUFFER_SIZE - 1)) // index wrapping macro 

#define gameOfLifeTime 		50        //in # of frames per generation
#define gameOfLifeNLFactor 	(char)(255/gameOfLifeTime)
#define gameOfLifeLFactor   (char)(100/gameOfLifeTime)
#define gameOfLifeR     	0         //1 - Color on  0 -color off
#define gameOfLifeG     	0         //1 - Color on  0 -color off
#define gameOfLifeB     	1         //1 - Color on  0 -color off
#define gameOfLifeMaxGen 	30       //Max numbers of generations before reset
#define gameOfLifeResetFade 50    //time of reset fade



class gameoflife{
public:
	gameoflife(struct CRGB * display);
	void run(byte res);

private:
	struct CRGB * disp;

};

#endif