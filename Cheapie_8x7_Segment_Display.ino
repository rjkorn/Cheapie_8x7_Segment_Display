  
//****************************************************************
//*  Name    : 74HC595 8x7 segment module                        *
//*  Author  : Robert Joseph Korn                                *
//*  Notice  : Copyright (c) 2015 Open Valley Consulting Corp    *
//*          : All Rights Reserved                               *
//*  Date    : 5/22/15                                           *
//*  Version : 1.0                                               *
//*  Notes   : Cobbled together from various forum posts and     *
//*          : modified for a cheapie Ebay 74HC595 display.      *
//*          :                                                   *
//****************************************************************

#define PWR  7   
#define GND  6  

#define LATCH  3   
#define CLOCK  4    
#define DATA   5   
#define MultiplexDelay  1  
#define LEFT   0  
#define RIGHT  1  
#define BLANK 11  

byte digits[8] =   { 0b01111111,  //digit 1 from right
                    0b10111111,  //digit 2 from right
                    0b11011111,  //digit 3 from right
                    0b11101111,  //digit 4 from right
                    0b11110111,  //digit 5 from right
                    0b11111011,  //digit 6 from right
                    0b11111101,  //digit 7 from right
                    0b11111110   //digit 8 from right                                         
                    };

byte segments[12] = {0b00111111,  // 0
                    0b00000110,  // 1
                    0b01011011,  // 2
                    0b01001111,  // 3
                    0b01100110,  // 4
                    0b01101101,  // 5
                    0b01111101,  // 6
                    0b00000111,  // 7
                    0b01111111,  // 8
                    0b01101111,  // 9  
                    0b10000000,  //dot                  
                    0b00000000   //blank
                    };     
 
//fucntion to send the serial data out to two 74HC595 serial to parallel shift register and activate the 7 segment.                  
void display8x7segment(byte datapin, byte clockpin, byte latchpin, byte digit, byte number)
{
    digitalWrite(latchpin, LOW);
    shiftOut(datapin, clockpin, MSBFIRST, digit); 
    shiftOut(datapin, clockpin, MSBFIRST, number); 
    digitalWrite(latchpin, HIGH);  
}

//function to display value on 8x7 segment display according to the justify state
void displayNumber8x7segment(byte justify, unsigned long value)
{

   byte decimal[8] = {0};   
   value = value % 100000000;  
   decimal[7] = value / 10000000;  
   value = value % 10000000;       
   decimal[6] = value / 1000000;
   value = value % 1000000;
   decimal[5] = value / 100000;
   value = value % 100000;
   decimal[4] = value / 10000;
   value = value % 10000;
   decimal[3] = value / 1000;
   value = value % 1000;
   decimal[2] = value / 100;
   value = value % 100;
   decimal[1] = value / 10;
   decimal[0] = value % 10;
   byte zero = 0;
   if (justify == RIGHT)
   {   
     for(byte e = 8; e > 0 ; e --)
     {
       if(zero == 0)
       {
         if(decimal[e-1] != 0)          
         {
           display8x7segment(DATA, CLOCK, LATCH, digits[e-1], segments[decimal[e-1]]);
           zero = 1;     
         }
       }
       else display8x7segment(DATA, CLOCK, LATCH, digits[e-1], segments[decimal[e-1]]);
     
     delay(MultiplexDelay); 
     }
   }
   else  //if justify == left
   { 
     byte d = 0;     
     for(byte e = 8; e > 0; e --)
     {       
       if(zero == 0)
       {
         if(decimal[e-1] != 0)         
         {
           display8x7segment(DATA, CLOCK, LATCH, digits[7], segments[decimal[e-1]]);
           zero = 1;
           d ++;     
           delay(MultiplexDelay); 
         }
       }
       else 
       {
         display8x7segment(DATA, CLOCK, LATCH, digits[7-d], segments[decimal[e-1]]);
         d ++;
         delay(MultiplexDelay); 
       }     
       
     }
     
   }
         display8x7segment(DATA, CLOCK, LATCH, 0b00000000, 0b00000000);
}
 
void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  digitalWrite(LATCH, HIGH);  
  pinMode(PWR, OUTPUT);
  digitalWrite(PWR, HIGH);  
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);  
}

void loop(){ 
      displayNumber8x7segment(RIGHT, millis()/1000);     
}


