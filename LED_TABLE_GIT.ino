#include <EEPROM.h>
#include <avr/pgmspace.h>

#include <Wire.h>
#include "snake.h"
#include "draw.h"
#include "RTClib.h"
#include "TimerOne.h"
#include "FastLED.h"
#include "SPI.h"
#include "gameoflife.h"
#include "draw.h"


/*
*===========#DEFINES========================================================
*/
RTC_DS1307 rtc;

#define analogPin  0
#define strobePin 3
#define resetPin 4

#define brightness 200
#define NUM_LEDS 100 

#define debounceTime 200

#define rainbowTime 25      //shorter time is a faster movement
#define rainbowWidth 0x2f

#define FRAMERATE 40000 //about 40ms (in us)  == 25fps

#define INITIAL_PROGRAM_POS 2 


#define CORDINATE(X, Y) ((Y)%2)?(((10*((Y)+1))-1)-(X)):(10*(Y))+(X)  

//int16_t CORDINATE(byte x, byte y)
//{
//  return (y%2)?(((10*(y+1))-1)-x):(10*y)+x;  
//}


/*
*=========GLOBAL VARIABLES==================================================
*/

// TEST SPRITE
word sprite1[10] = {0x1200, 0x3F00, 0x6E80, 0xFFC0, 0xBF40, 0xA140, 0x1200,0x0000,0x0000, 0x0000};
  
char globalProgramPos = INITIAL_PROGRAM_POS;   //determines what function should be running
char frame_draw_flag = 0;                      //set to 1 if frame was drawn

CRGB leds[NUM_LEDS];                           //sets global variable for leds 

snake snake(leds);
draw draw(leds);
gameoflife GOL(leds);

unsigned long debug_time = 0;

void makeRainbow(struct CRGB *disp, unsigned int hueStart);

/*
*============INTERRUPTS======================================================
*/
void Interrupt(){
  static unsigned long last_interrupt = 0;
  unsigned long interrupt_time = millis();
  if(interrupt_time - last_interrupt > debounceTime){  
    globalProgramPos++;
  }
  last_interrupt = interrupt_time;
}

void int_draw_frame(){
    frame_draw_flag = 1;
    for(byte i = 0; i < 100; i++){
        SPI.transfer(leds[i].r);
        SPI.transfer(leds[i].g);
        SPI.transfer(leds[i].b);
    }
  }


/*
*=========SETUP======================================
*/
void setup() {
  Serial.begin(115200);
  
  randomSeed(analogRead(0));


  SPI.begin(); // wake up the SPI bus.
  SPI.setBitOrder(MSBFIRST);
  // nunchuk.init();

  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

  Wire.begin();
  rtc.begin();

  FastLED.setBrightness(brightness);
  FastLED.addLeds<WS2801, 11, 13, RGB, DATA_RATE_MHZ(1)>(leds, NUM_LEDS);
  memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  FastLED.show();

  GOL.run(1);
  snake.snakeInit();
  
  attachInterrupt(INT1, Interrupt, FALLING);

  Timer1.initialize(FRAMERATE);
  Timer1.attachInterrupt(int_draw_frame);

}


void loop() { 

  switch(globalProgramPos){
//-----------------------------------------------------------------------
    case 0:
        if(frame_draw_flag){
          GOL.run(0);
          frame_draw_flag = 0;               //reset frame draw flag
        }
        break;
//-------------------------------------------------------------        S
//------------------------------------------------------------------------
      case 1:
        if(frame_draw_flag){
            makeRainbow(leds,(millis()/rainbowTime)%0x0600);
            frame_draw_flag = 0;        //reset frame draw flag
        }
       break;
      
      case 2:
        if(frame_draw_flag){
          snake.run();
          frame_draw_flag = 0;
        }
      break;
      

      case 3:
        if(frame_draw_flag){
          leds[CORDINATE(0,0)] = CRGB::Red;
          leds[CORDINATE(0,1)] = CRGB::Blue;
          frame_draw_flag = 0;
        }
        break;

      default:
        globalProgramPos =0;
        break;
        
  }
        
}

void makeRainbow(struct CRGB *disp, unsigned int hueStart) {
  unsigned char i;
  //unsigned int hue = hueStart;

  for(i = 0; i < 10; i++){
        draw.drawLine(0, i, i, 0, draw.Wheel((hueStart + (i * rainbowWidth)) % 0x0600)); 
      }
  for(i = 1; i<10; i++){
      draw.drawLine(i,9,9,i, draw.Wheel((hueStart + ((i+9) * rainbowWidth)) % 0x0600));
  }
  
}



