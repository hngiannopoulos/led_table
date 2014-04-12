

void accelXY(byte * x, byte *y){
    word temp;
    nunchuk.update();
    temp = constrain(nunchuk.accelX,400, 600);
    *x = (byte)map(temp, 400, 600, 0, 9);
    
    temp = constrain(nunchuk.accelY,430, 650);
    *y = (byte)map(temp, 430, 650, 9, 0);
    
}
 
 void joyXY(byte * x, byte *y){
    byte temp;
    nunchuk.update();
    temp = constrain(nunchuk.analogX,200, 50);
    *x = (byte)map(temp, 200, 50, 0, 9);
    
    temp = constrain(nunchuk.analogY,200, 50);
    *y = (byte)map(temp, 200, 50, 9, 0);
 }
    
