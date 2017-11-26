#include <msp430.h>
#include "libTimer.h"
#include "sound.h"


static unsigned int period = 1000;
static signed int rate = 500;	
char state;
#define MIN_PERIOD 1000
#define MAX_PERIOD 10000



void sound_init(char n)
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
    state = n;
     sound_advance_frequency();/* start buzzing!!! */


}
/*
Handle sound states
 */
void sound_advance_frequency() 
{

  soundOne();
  sound_set_period(period);

}
/*
Sound One 
 */
void soundOne(){
  period += rate;
  if ( period > 1000  && period <= 2000)   {
    period += (rate <<7);
    // printf("\nPeriodTop= %d",period);
    }
  else if(period > 4000 && period <=6000){
    period += (rate <<5);
    
    // printf("\nPeriodTop= %d",period);
    
  }
  else if(period >4000 && period <= 8000){
    period += (rate << 9); 
  }
  else if(period >8000 && period <= MAX_PERIOD){
    period += (rate << 3); 
  }
}
void sound_set_period(short cycles)
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}


    
    
  

