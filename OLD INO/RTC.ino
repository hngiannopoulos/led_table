

void rtcString(char *string){
  DateTime now = rtc.now();
  byte stringPos = 0;
  byte to12hr;

  to12hr = (now.hour() <= 12) ? (now.hour()) : (now.hour() - 12);

  if((to12hr < 10))             //for first character
    string[stringPos++] = (to12hr / 10) + 48;   //add 10's place if applicable


  string[stringPos++]  = ((to12hr % 10)+48);        //for 1's place

  string[stringPos++] = ':';                        //add colon

  string[stringPos++] = (now.minute() / 10)+48;      //add 10's place for minute

  string[stringPos++] = (now.minute() % 10)+48;    //add

  string[stringPos++] = '\0'; 

}


