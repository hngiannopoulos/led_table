#include "Font.h"
#include "FastSPI_LED2.h"
#include <EEPROM.h>
#include <avr/pgmspace.h>

#include <Wire.h>
#include <ArduinoNunchuk.h>
#include "RTClib.h"
RTC_DS1307 rtc;

#define analogPin  0
#define strobePin 3
#define resetPin 4


/*
*===========PROGRAM VARIBLES========================================================
*/
#define brightness 200
#define gameOfLifeTime 175
#define debounceTime 200
#define rainbowTime 20
#define rainbowWidth 0x2F
//==================================================================================

// TEST SPRITE
word sprite1[10] = {
  0x1200, 0x3F00, 0x6E80, 0xFFC0, 0xBF40, 0xA140, 0x1200,0x0000,0x0000, 0x0000};
  
char globalProgramPos = 1; 

unsigned long int lastMillis, nowMillis;

#define NUM_LEDS 100

CRGB leds[NUM_LEDS];

ArduinoNunchuk nunchuk = ArduinoNunchuk();

void Interrupt(){
  static unsigned long last_interrupt = 0;
  unsigned long interrupt_time = millis();
  if(interrupt_time - last_interrupt > debounceTime){  
    globalProgramPos++;
  }
  last_interrupt = interrupt_time;
}


void setup() {

  Serial.begin(115200);
  LEDS.setBrightness(brightness);
  randomSeed(analogRead(0));
  nunchuk.init();

  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

  Wire.begin();
  rtc.begin();

  LEDS.addLeds<WS2801, 11, 13, RGB, DATA_RATE_MHZ(1)>(leds, NUM_LEDS);
  gameOfLife(leds, Wheel(millis()%254), 1);
  memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  LEDS.show();

  attachInterrupt(INT1, Interrupt, FALLING);


}

void loop() { 
  switch(globalProgramPos){
//-----------------------------------------------------------------------
    case 0:
         gameOfLife(leds, Wheel(millis()/15%0x5ff), 0);
         LEDS.show();
         while(millis() - lastMillis < gameOfLifeTime){
             if(globalProgramPos !=0){
               break;
             }
         }
         lastMillis = millis();
        break;
//-------------------------------------------------------------        
     case 1:
        char time[6];
        rtcString(time);
        //Serial.println(time);
        for(char i = 0; i<=scrollText(time, 1,i, leds, Wheel(50)); i++){
          delay(100);
          LEDS.show();
          memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
          if(globalProgramPos != 1)
            break;
        }
        break;
//------------------------------------------------------------------------
      case 2:
        makeRainbow(leds,(millis()/rainbowTime)%0x0600);
        LEDS.show();
         while(millis() - lastMillis < rainbowTime){
             if(globalProgramPos !=2){
               break;
             }
         }
         lastMillis = millis();
         snakeAI();
       
       break;
      
      
      default:
        globalProgramPos =0;
        break;
        
  }
        
}




