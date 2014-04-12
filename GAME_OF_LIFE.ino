#define GOL_BUFFER_SIZE 16 // must be a power of 2 
#define GOL_BUFFER_WRAP(i) ((i) & (GOL_BUFFER_SIZE - 1)) // index wrapping macro 

/*
  INPUTS: disp- the pointer to the display grid. color- the color of the active cells on the field
 res - externall reset flag, needs to be called before function is called for the first time
 otherwise you need to wait for 16 frames for the game to reset
 
 */
void gameOfLife(struct CRGB * disp, struct CRGB color, byte res){
  static word current[15];  //current 16x16 playing fied
  static word next[15];     //next 16x16 playing field
  byte xMod, yMod, alive, fadecount;
  static byte emptycount;
  static byte Reset;        //internal reset flag
  static byte frameOffset = 0;    //offset for drawing frame
  static unsigned long timeKeep;

  if((Reset == 1) | (res == 1)){    //if reset flag is thrown
    for(byte i = 0; i< 15; i++){        
      current[i] = random(0xFFFF);      //ramdomly populate playing field
    }
    
    Reset = 0;        //set reset flag back to 0;
    emptycount = 0;   //if the reset flag was thrown internally by a stagnated game reset stagnant frame count 
    //timeKeep = millis()+600;
    Serial.println("Reset");
    return;
  }

// if(millis() > (timeKeep + 250)){

//check each cell to update for next generation generation
for(byte y = 0; y<16; y++){         //check each row
  for(byte x = 0; x <16; x++){    //Check each column
    xMod =(15-((x + 15) % 15));    //flip x 
    yMod = (y + 15) % 15;

    alive = 0;
    alive += bitRead(current[yMod+1], xMod);      //right
    alive += bitRead(current[yMod], xMod+1);      //left
    alive += bitRead(current[yMod-1], xMod);      //up
    alive += bitRead(current[yMod], xMod-1);      // down
    alive += bitRead(current[yMod+1], xMod+1);    //down + right
    alive += bitRead(current[yMod-1], xMod-1);    //up + right
    alive += bitRead(current[yMod+1], xMod-1);    //down + left
    alive += bitRead(current[yMod-1], xMod+1);    //up + right

    if (bitRead(current[yMod], xMod) == 1){                     //check to see if cell lives or dies
      if ((alive < 2) || (alive > 3))       
        bitWrite(next[yMod], xMod, 0);                //dies
      else
        bitWrite(next[yMod], xMod, 1);                //lives
    }
    else if(bitRead(current[yMod], xMod) == 0){
      if (alive == 3)
        bitWrite(next[yMod], xMod, 1);                  //stays alive
    }

  }
}    //finish generation */
   
Serial.println("checked generation");

 

  //reset counter to see when game has stagnated
  fadecount = 0;
  for(byte i = 0; i<15; i++){      //copy new frame to current frame
    if(((unsigned int)(next[i])) == ((unsigned int)(current[i]))){ 
      fadecount++;
    }
    current[i] = next[i]; 
  }

  if(fadecount > 7)    //if most of the frame has not moved
    emptycount++;     //add 1 to emptycount
  else
    emptycount = 0;         //reset emptycount to 0 to prevent false positives

  if(emptycount>25)        //if game had been mostly stagnant for 15 frames in a row, throw internal reset
    Reset = 1;
    
  
  //timeKeep = millis();
  for(byte y = 0; y<10; y++){      
    for(byte x = 0; x<10; x++){
      if(bitRead(current[GOL_BUFFER_WRAP(y + (frameOffset>>1))], GOL_BUFFER_WRAP(15 - x - (frameOffset>>1)))==1)          //shift some bits to see if a cell in the field is alive
        disp[cordinate(x,y)] =color;

      else 
        disp[cordinate(x,y)] = CRGB(0, 0 ,0 );

    }
  }
  frameOffset++;
  return;
}