#define GOL_BUFFER_SIZE 16 // must be a power of 2 
#define GOL_BUFFER_WRAP(i) ((i) & (GOL_BUFFER_SIZE - 1)) // index wrapping macro 

#define gameOfLifeTime 50        //in # of frames per generation
#define gameOfLifeNLFactor (char)(255/gameOfLifeTime)
#define gameOfLifeLFactor   (char)(100/gameOfLifeTime)
#define gameOfLifeR     0         //1 - Color on  0 -color off
#define gameOfLifeG     0         //1 - Color on  0 -color off
#define gameOfLifeB     1         //1 - Color on  0 -color off
#define gameOfLifeMaxGen 30       //Max numbers of generations before reset
#define gameOfLifeResetFade 50    //time of reset fade

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
  static byte genCount = 0;
  static byte resetFade = gameOfLifeResetFade - 1;

  byte xMod, yMod, alive, fadecount;
  CRGB fadeInColor, fadeOutColor, onColor;
  byte fadeInVal, fadeOutVal;
 

  if((Reset == 1) | (res == 1)){    //if reset flag is thrown
   
    resetFade++;
    if(resetFade != gameOfLifeResetFade){   //take time to fade colors

          fadeOutVal    = 250 - (resetFade * 5);
          fadeOutColor  = CRGB(fadeOutVal * gameOfLifeR, fadeOutVal * gameOfLifeG, fadeOutVal * gameOfLifeB);
    
          for(byte y = 0; y<10; y++){      //draw the current frame window 
            for(byte x = 0; x<10; x++){
                 if( (bitRead(current[y], GOL_BUFFER_WRAP(x))==1) ){    //keep on all the way if the colonly isnt changing
                      disp[cordinate(x,y)] = fadeOutColor;
                  }
            }
            Serial.println(fadeOutVal);
          }
      }

    else{
      for(byte i = 0; i< 15; i++){        
        current[i] = 0;      //ramdomly populate playing field
        next[i] = random(0xFFFF);
      }
      Reset = 0;        //set reset flag back to 0;
      emptycount = 0;   //if the reset flag was thrown internally by a stagnated game reset stagnant frame count 
      genCount = 0;
      resetFade =  0;
    }
    
    //Serial.println(resetFade);
    return;
  }

  //check each cell to update for next generation generation
  if(frameCount == 0){
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


  fadeInVal     = (frameCount * gameOfLifeNLFactor) ;   //uncomment for non-linear brightness
  fadeOutVal    = 250 - fadeInVal;
  // fadeInVal     = linearPWM(frameCount * gameOfLifeLFactor);                            //uncomment for linear brightness
  // fadeOutVal    = linearPWM((gameOfLifeTime - frameCount) * gameOfLifeLFactor);
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
   
  fadecount = 0;      //seagnation variable

  if(frameCount == gameOfLifeTime){   //copy over and check for game stagnation
    frameCount = 0;                   
    genCount++;

    for(byte i = 0; i<16; i++){      //copy new frame to current frame
      if(((unsigned int)(next[i]) == ((unsigned int)(current[i]))) && (i < 10))
          fadecount++;

      current[i] = next[i]; 
    } //end for loop

    if(fadecount > 5)    //if most of the frame has not moved
      emptycount++;     //add 1 to emptycount
    else
      emptycount = 0;         //reset emptycount to 0 to prevent false positives

    if((emptycount>5) || (genCount > gameOfLifeMaxGen))        //if game had been mostly stagnant for 15 frames in a row, throw internal reset
      Reset = 1;
  }
  else
    frameCount++;
  
  
 
  return;
}