#include "snake.h"
#include "FastLED.h"

snake::snake(struct CRGB ledDisplay[]){
   disp = ledDisplay;      //copy display buffer 

   memset(snakeArray,0xFF, 75 * sizeof(byte));
   snakeArray[0]       = 0x55;
   snakeArray[1]       = 0x56;
   snakeArray[2]       = 0x57;
   apple          = makeApple();
   snakeDirection = movingUp;
   snakeLength    = 3;
}

void snake::run(){

   byte framesPerMovement         =   constrain(6 - (snakeLength / 5), 0, 6);
   static byte framesSinceLastGen = framesPerMovement;
   static byte animationCount     = 0;
   static byte gameStarted;
   byte appleX                    = (apple>>4) & 0x0F;
   byte appleY                    = (apple & 0x0F);
   byte headX                     = (snakeArray[snakeLength-1] >> 4) & 0x0F;
   byte headY                     = (snakeArray[snakeLength-1] & 0x0F);
   byte gameState                 = 0; //1 - if game was lost; 0 - otherwise 
   byte quadrant;
   //Snake State Machine 

   if(framesSinceLastGen == framesPerMovement){
      framesSinceLastGen = 0;
      switch(snakeState){
         case resetAll:
               animationCount = 0;
               snakeInit();
               snakeState = lookingForDirection;
         break;
          
          case lookingForDirection:
                  quadrant = determineQuadrant(headX, headY, appleX, appleY);
                  //Serial.print("Direction = "); Serial.println(quadrant);
                  if(snakeDirection == movingUp){
                        if((quadrant == 0) || (quadrant == 1)) 
                           gameState = snakeMove(noTurn);
                        else if((quadrant == 2))
                           gameState = snakeMove(turnLeft);   
                        else if(quadrant == 3)
                           gameState = snakeMove(turnRight); 
                        else if(quadrant == 5)
                           gameState = snakeMove(turnRight); 
                        else if(quadrant == 7)
                           gameState = snakeMove(turnLeft);
                        else if((quadrant == 6) || (quadrant == 4))
                           gameState = snakeMove(noTurn);
                  }

                  else if(snakeDirection == movingDown){
                      if((quadrant == 2) || (quadrant == 3)) 
                           gameState = snakeMove(noTurn);
                        else if((quadrant == 0))
                           gameState = snakeMove(turnRight);   
                        else if(quadrant == 1)
                           gameState = snakeMove(turnLeft); 
                        else if(quadrant == 5)
                           gameState = snakeMove(turnLeft); 
                        else if(quadrant == 7)
                           gameState = snakeMove(turnRight);
                        else if((quadrant == 6) || (quadrant == 4))
                           gameState = snakeMove(noTurn);
                  }

                  else if(snakeDirection == movingLeft){
                     if((quadrant == 0) || (quadrant == 1)) 
                        gameState = snakeMove(turnRight);
                     else if((quadrant == 2) || (quadrant == 3))
                        gameState = snakeMove(turnLeft);   
                     else if(quadrant == 6)
                        gameState = snakeMove(turnLeft); 
                     else if(quadrant == 4)
                        gameState = snakeMove(turnRight); 
                     else
                        gameState = snakeMove(noTurn);    

                  }

                  else{ //snakeDirection == movingRight
                     if((quadrant == 0) || (quadrant == 1)) 
                        gameState = snakeMove(turnLeft);
                     else if((quadrant == 2) || (quadrant == 3))
                        gameState = snakeMove(turnRight);   
                     else if(quadrant == 6)
                        gameState = snakeMove(turnRight); 
                     else if(quadrant == 4)
                        gameState = snakeMove(turnLeft); 
                     else 
                        gameState = snakeMove(noTurn);    

                  }

                  if((gameState == 1))//lost game or won game
                     snakeState = deathAnimation;
                  // else if(quadrant == 8)
                  //    snakeState = newApple;
                  else
                     snakeState = lookingForDirection;
                  drawSnake();
            break;   //end lookingForDirection

            case deathAnimation:
                  if(animationCount <= snakeLength){
                      
                        byte X, Y;
                      // Serial.println(animationCount);
                        byte snakeVal = (byte)(( ((snakeLength - animationCount) * (float)225.0/snakeLength)) + 30);
                        X = (snakeArray[snakeLength - animationCount] >> 4) & 0x0F;    //unmask x cord
                        Y = (snakeArray[snakeLength - animationCount] & 0x0F);            //unmask y
                        disp[CORDINATE(X,Y)] = CRGB(snakeVal * snakeDeathR, snakeVal * snakeDeathG, snakeVal * snakeDeathB);
                         animationCount++;
                  }
                  else
                     snakeState = resetAll;
                     //animationCount = 0;

               break;

            }// end case
         }
         else
            framesSinceLastGen++;

         
          
   } 


void snake::drawSnake(){
   byte i = 0; 
   byte X, Y;
   memset(disp, 0, NUM_LEDS * sizeof(struct CRGB)); //clear display
   byte snakeVal = 100;
   for(i = 0; i < snakeLength; i++){
      snakeVal = (byte)(( (i * (float)225.0/snakeLength)) + 30);
      //snakeVal = 100 - snakeVal;
      X = (snakeArray[i] >> 4) & 0x0F;    //unmask x cord
      Y = (snakeArray[i] & 0x0F);            //unmask y
      disp[CORDINATE(X,Y)] = CRGB(snakeVal * snakeR, snakeVal * snakeG, snakeVal * snakeB);
   }

   X = (apple >> 4) & 0x0F;    //unmask x cord
   Y = (apple & 0x0F);            //unmask y
   disp[CORDINATE(X,Y)] = CRGB(255 * appleR, 255* appleG, 255 * appleB);
}


void snake::snakeInit(){
   memset(snakeArray,0xFF, 75 * sizeof(byte));
   snakeArray[0] = 0x55;
   snakeArray[1] = 0x56;
   snakeArray[2] = 0x57;
   apple = makeApple();
   snakeDirection   = movingUp;
   snakeLength = 3;
}


//
//INPUTS: MOVING: UP, DOWN, LEFT, RIGHT
//            TURNING: turnRight turnLeft noTurn
//
//RETURNS 1 if the snake hits itself
//            0 if the snake is ok

byte snake::snakeMove(byte turning){                
   byte snakeHead = snakeArray[snakeLength - 1 ];   //pull snake head coordinate
   byte findMove = 1;
   byte X, Y, i;
   byte tempSnakeDirection = snakeDirection;
   byte moveOk = 0;
   byte returnVal = 0;
         //some sort of fucking table 
         //modifies snake head 
      while(moveOk < 5){
         X = (snakeHead >> 4) & 0x0F;    //unmask x cord
         Y = (snakeHead & 0x0F);            //unmask y
         switch(snakeDirection){
            case(movingUp):   
                  if(turning == noTurn){
                     Y++;
                     tempSnakeDirection = movingUp;
                  }
                  else if(turning ==turnRight){
                     X--;
                     tempSnakeDirection = movingRight;
                  }
                  else if(turning == turnLeft){
                     X++;
                     tempSnakeDirection = movingLeft;
                  }
            break;

            case(movingDown):   
                  if(turning == noTurn){
                     Y--;
                     tempSnakeDirection = movingDown;
                  }
                  if(turning ==turnRight){
                     X++;
                     tempSnakeDirection = movingLeft;   
                  }
                  if(turning == turnLeft){
                     X--;
                     tempSnakeDirection   = movingRight;
                  }
            break;

            case(movingLeft):   
                  if(turning == noTurn){
                     X++;
                     tempSnakeDirection = movingLeft;
                  }
                  if(turning ==turnRight){
                     Y++;
                     tempSnakeDirection = movingUp;
                  }
                  if(turning == turnLeft){
                     Y--;
                     tempSnakeDirection = movingDown;
                  }
            break;

            case(movingRight):
                  if(turning == noTurn){
                     X--;
                     tempSnakeDirection = movingRight;
                  }
                  if(turning ==turnRight){
                     Y--;
                     tempSnakeDirection = movingDown;

                  }
                  if(turning == turnLeft){
                     Y++;
                     tempSnakeDirection = movingUp;
                  }
            break;
            
            default:
            //if you get here theres a lot of problems, but keep the head in the same spot.
            break;

         }   //end of switch
         
         
         //checks to see if snake head overlaps with any other part of the snake
         //returns 1 if you lost the game
         returnVal = 0;
         for(i = 0; i < snakeLength; i++){   
               if(snakeArray[i] == pointToBCD(X,Y) )                     //if the head of the snake overlaps itself return a 1, loose flag
                  returnVal = 1; 
         }
   
         if(returnVal == 1){         //if youre going to hit yourself by turning, try agin going straight
               moveOk++;    //add 1 to the try count
               Serial.println(moveOk);
               if((turning != noTurn) && (moveOk < 2)){    //if you're trying to turn 
                  turning = noTurn;
                   //Serial.println("Trying No Turn");
               }
               else {                           //if you're still going straight
                  turning = (moveOk % 2) ? turnLeft : turnRight;
                  //Serial.println("trying a turn");
               }

             
               returnVal = 1; 
         }
         
         else{
            moveOk = 10;      //
            snakeDirection = tempSnakeDirection;
         }
 
   }             //end of while(!moveOk)

   snakeArray[snakeLength] = pointToBCD(X,Y);      //assign new head with modified X and Y values   
   if(snakeArray[snakeLength] != apple){               //if new head isnt at the apple, move the snake down
         for(i = 0; i < snakeLength+1; i++)
               snakeArray[i] = snakeArray[i+1];
   }
   else{                                                                  //if you have hit the apple 
      snakeLength ++;                                                //add one to the lenght of the snake 
      apple = makeApple();                                        // and make new apple
   }
   return returnVal;
} 

// 0 | 1   on lines 4(V), 5
// 2 | 3   relative to the apple location
byte snake::determineQuadrant(byte headX, byte headY, byte appleX, byte appleY){                      
      	byte quadrant = 0;

         if((headX == appleX) && (headY == appleY))                     //if you actually hit the apple
            return 8;

      	if((headX == appleX))								//if you happen to be exactly on the vertical line and bellow apple
      		return (appleY > headY) ? 4 : 6;

         if((headY == appleY))                      //if you happen to be exactly on the vertical line and above apple;
            return (appleX > headX) ? 7 : 5;

         //determine regular quadrant
      	if(headY > appleY)			//you're bellow the apple   
      		quadrant += 2;			
      	if(headX > appleX)			// you're to the right of the apple
      		quadrant += 1;	

	    return quadrant;
}

byte snake::pointToBCD(char x, char y){
   if((x < 0) | (x > 9))      //if x is outside of the display area
      x = (x > 9) ? 0 : 9;    //loop around the display x = 10 convert to x = 0
   if((y < 0) | (y > 9))            //repeat with y
      y = (y > 9) ? 0 : 9;

   return (x << 4) | (y & 0x0F);
}

byte snake::makeApple(){
   char appleOk = 0;
   char tempApple;
   while(!appleOk){
      appleOk = 1;
      tempApple = pointToBCD(random(0x09) , random(0x09));
      for(byte i = 0; i < snakeLength; i++){
            if(tempApple == snakeArray[i])
                  appleOk = 0;    //if you hit overlap run again
      }
   }
   return tempApple;
}




