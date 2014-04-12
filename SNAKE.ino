#define movingUp    0
#define movingDown  1
#define movingLeft  2
#define movingRight 3

#define turnRight   4 
#define turnLeft    5
#define noTurn      6 
#define endOfSnake  0xFF

//SnakeStates
#define resetAll 0
#define lookingForDirection 1
#define goingUpDown 2
#define goingLeftRight  3
#define almostThere 4
#define hitTarget 5
#define failed 6


//globals

byte snake[75] = {0xFF}; //sored in BCD (xy) where 0xFF is equivalent to the null therminator 
//The array is backwards, the head is at the highest value  snakeHead = snake[snakeLenght]
byte snakeDirection  = movingUp;
byte snakeLength = 1;
byte apple;
byte snakeState;



// void snakeAI(struct CRGB * disp){
//    byte appleX = (apple>>4) & 0x0F;
//    byte appleY = (apple & 0x0F);
//    byte headX = (snake[snakeLength] >> 4) 0x0F;
//    byte headY = (snake[snakeLength] & 0x0F);
   
   
//   switch(snakeState){
//     case resetAll:
//       snakeInit();
//       snakeState = lookingForDirection;
//      break;
     
//      case lookingForDirection:
//        byte quadrant = determineQuadrant(headX, headY, appleX, appleY);
//        if((quadrant == 4) || (quadrant == 5)){       //if youre in line with the thing already
//        }
       
//        if(quadrant == 
      
       
//   } 
// }




void snakeInit(){
  memset(snake,0xFF, sizeof(snake));
  snake[0] = 0x55;
  snake[1] = 0x56;
  apple = makeApple();
  snakeDirection  = movingUp;
  snakeLength = 1;
}


//
//INPUTS: MOVING: UP, DOWN, LEFT, RIGHT
//        TURNING: right left no turn
byte snakeMove(byte moving, byte turning){            //returs 1 if the snake hits itself
  byte snakeHead = snake[snakeLength];  //pull snake head coordinate
  byte X, Y, i;

  X = (snakeHead >> 4) & 0x0F;   //unmask x cord
  Y = (snakeHead & 0x0F);        //unmask y

  //some sort of fucking table 
  switch(moving){
    case(movingUp):  
    if(turning == noTurn)
      Y++;
    if(turning ==turnRight){
      X++;
      snakeDirection = movingRight;

    }
    if(turning == turnLeft){
      X--;
      snakeDirection = movingLeft;
    }
    break;

    case(movingDown):  
    if(turning == noTurn)
      Y--;
    if(turning ==turnRight){
      X--;
      snakeDirection = movingLeft;  

    }
    if(turning == turnLeft){
      X++;
      snakeDirection  = movingRight;

    }
    break;

    case(movingLeft):  
    if(turning == noTurn)
      X--;
    if(turning ==turnRight){
      Y++;
      snakeDirection = movingUp;

    }
    if(turning == turnLeft){
      Y--;
      snakeDirection = movingDown;
    }
    break;

    case(movingRight):
    if(turning == noTurn)
      X++;
    if(turning ==turnRight){
      Y--;
      snakeDirection = movingDown;

    }
    if(turning == turnLeft){
      Y++;
      snakeDirection = movingUp;
    }
    break;
    
  default:
    //if you get here theres a lot of problems, but keep the head in the same spot.
    break;
  }  //end of switch
  
  for(i = 0; i < snakeLength+1; i++){
      if(snake[i] == pointToBCD(X,Y))              //if the head of the snake overlaps itself return a 1, loose flag
        return 1; 
  }
  
  
  snake[snakeLength + 1] = pointToBCD(X,Y);    //assign new head with modified X and Y values  
  if(snake[snakeLength + 1] != apple){          //if new head isnt at the apple, move the snake down
      for(i = 0; i < snakeLength+1; i++)
          snake[i] = snake[i+1];
  }
  else{                                            //if you have hit the apple 
    snakeLength ++;                                //add one to the lenght of the snake 
    apple = makeApple();                           // and make new apple
  }
  return 0;

}  


byte determineQuadrant(byte headX, byte headY, byte appleX, byte appleY){               // 0 |1  on lines 4(V), 5
											// 2 | 3
	byte quadrant;
	if((headX == appleX))								//if you happen to be exactly on the vertical line line return 4;
		return 4;
	if(headY == appleY)									//if you're on the main horizontal line return 5
		return 5;
	
	if(headY < appleY)			//you're bellow the apple  
		quadrant += 2;			
		
	if(headX > appleX)			// you're to the right of the apple
		quadrant += 1;
		
	return quadrant;
}

byte pointToBCD(char x, char y){
  if((x < 0) | (x > 9))    //if x is outside of the display area
    x = (x > 9) ? 0 : 9;   //loop around the display x = 10 convert to x = 0
  if((y < 0) | (y > 9))        //repeat with y
    y = (y > 9) ? 0 : 9;

  return (x << 4) | (y & 0x0F);
}



byte makeApple(){
    return pointToBCD(random(0x08) + 1, random(0x08) +1);
}

void drawSnake(struct CRGB * disp, struct CRGB colorSnake, struct CRGB colorApple){
	byte i = 0; 
	byte X, Y;
	while(snake[i] != 0xFF){			//draw cordinates
		X = (snake[i] >> 4) & 0x0F;   //unmask x cord
		Y = (snake[i] & 0x0F);        //unmask y
		leds[cordinate(X,Y)] = colorSnake;
		i++;
	}
	X = (apple >> 4) & 0x0F;   //unmask x cord
	Y = (apple & 0x0F);        //unmask y
	leds[cordinate(X,Y)] = colorApple;
}


