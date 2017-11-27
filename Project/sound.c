#include <msp430.h>
#include "libTimer.h"
#include "sound.h"
#include <lcdutils.h>
char s1[5];
static unsigned int period = 1000;
static signed int rate = 200;	
char state;
char play;
#define MIN_PERIOD 1000
#define MAX_PERIOD 1200



void sound_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */
    // state = '2';
   
     sound_advance_frequency();/* start buzzing!!! */


}
/*
Handle sound states
 */
void sound_advance_frequency() 
{
  if(play=='1'){
    rate =200;
  soundOne();
  sound_set_period(period);

  }
  else{
    rate = 0;
    period = 1000;
  }
}
/*
Sound One 
 */
void soundOne(){
  
  if ( period >= 1000  && period < 1200)   {
    period += rate;
    // printf("\nPeriodTop= %d",period);
    }
  
  else if(period >= MAX_PERIOD){
   play = '0';
   //itoa(period,s1,10);
   // drawString5x7(40,80, play , COLOR_GREEN, COLOR_BLUE);
  
   
  }
  
}
void sound_set_period(short cycles)
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}


    
    
  

