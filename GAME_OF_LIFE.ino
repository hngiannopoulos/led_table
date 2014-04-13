#define GOL_BUFFER_SIZE 16 // must be a power of 2 
#define GOL_BUFFER_WRAP(i) ((i) & (GOL_BUFFER_SIZE - 1)) // index wrapping macro 

/*
  INPUTS: disp- the pointer to the display grid. color- the color of the active cells on the field
 res - externall reset flag, needs to be called before function is called for the first time
 otherwise you need to wait for 16 frames for the game to reset
 
 */
void gameOfLife(struct CRGB * disp, byte res){

  static word current[15];  //current 16x16 playing fied
  static word next[15];     //next 16x16 playing field
  static byte emptycount;
  static byte Reset;        //internal reset flag
  static byte frameOffset = 0;    //offset for drawing frame
  static byte frameCount = 0;

  byte xMod, yMod, alive, fadecount;
  CRGB fadeInColor, fadeOutColor, onColor;
  byte fadeInVal, fadeOutVal;
  static byte genCount = 0;

  Serial.println(frameCount);
  if((Reset == 1) | (res == 1)){    //if reset flag is thrown

      for(byte i = 0; i< 15; i++){        
        current[i] = random(0xFFFF);      //ramdomly populate playing field
      }
      Reset = 0;        //set reset flag back to 0;
      emptycount = 0;   //if the reset flag was thrown internally by a stagnated game reset stagnant frame count 
      genCount = 0;
      return;
  }

  //check each cell to update for next generation generation
  if(frameCount == 0){
    Serial.print("Run");
    Serial.println(frameCount);
    for(byte y = 0; y<16; y++){         //check each row
      for(byte x = 0; x <16; x++){    //Check each column
        xMod =(15-((x + 15) % 15));    //flip x 
        yMod = (y + 15) % 15;

        alive = 0;
        alive += bitRead(current[yMod+1], xMod);      //right
        alive += bitRead(current[yMod+1], xMod+1);    //down + right
        alive += bitRead(current[yMod+1], xMod-1);    //down + left

        alive += bitRead(current[yMod-1], xMod-1);    //up + right
        alive += bitRead(current[yMod-1], xMod);      //up
        alive += bitRead(current[yMod-1], xMod+1);    //up + right

        alive += bitRead(current[yMod], xMod+1);      //left
        alive += bitRead(current[yMod], xMod-1);      // down

        if (bitRead(current[yMod], xMod) == 1){                     //check to see if cell lives or dies
          if ((alive < 2) || (alive > 3))       
            bitWrite(next[yMod], xMod, 0);                //dies
          else
            bitWrite(next[yMod], xMod, 1);                //lives
        }
        else{
          if (alive == 3)
            bitWrite(next[yMod], xMod, 1);                  //stays alive
        }

      }
    }    //finish generation */


  }

  //memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  fadeInVal     = frameCount * 5;
  fadeOutVal    = 250- fadeInVal;
  fadeInColor   = CRGB(fadeInVal * gameOfLifeR, fadeInVal * gameOfLifeG, fadeInVal * gameOfLifeB);
  fadeOutColor  = CRGB(fadeOutVal * gameOfLifeR, fadeOutVal * gameOfLifeG, fadeOutVal * gameOfLifeB);
  onColor       =  CRGB(255 * gameOfLifeR, 255 * gameOfLifeG, 255 * gameOfLifeB); 

  for(byte y = 0; y<10; y++){      //draw the current frame window 
    for(byte x = 0; x<10; x++){
      if( (bitRead(current[y], GOL_BUFFER_WRAP(x))==0) && (bitRead(next[y], GOL_BUFFER_WRAP(x))==0)){    //keep on all the way if the colonly isnt changing
           disp[cordinate(x,y)] = CRGB::Black; 
      }

      else if( (bitRead(current[y], GOL_BUFFER_WRAP(x))==1) && (bitRead(next[y], GOL_BUFFER_WRAP(x))==1) ){    //keep on all the way if the colonly isnt changing
        disp[cordinate(x,y)] = onColor; 
      }
      else if( (bitRead(current[y], GOL_BUFFER_WRAP(x)) == 1) && (bitRead(next[y], GOL_BUFFER_WRAP(x))==0) ){         //fade current out
            disp[cordinate(x,y)] = fadeOutColor;
      }

      else{         //fade next color in
              disp[cordinate(x,y)] = fadeInColor;
      }
     
     }
  }
   
   
  //fadecount = 0;
  if(frameCount == gameOfLifeTime){
      for(byte i = 0; i<16; i++){      //copy new frame to current frame
        current[i] = next[i]; 
      }
       frameCount = 0;
  }
  else{
    frameCount++;
  }
  
  frameOffset++;
  return;
}