#include <msp430.h>
#include "sound.h"


char state=0;
void decisecond() 
{
  static char cnt = 0;		/* # deciseconds/frequecy change */
  if (++cnt > 2) {
    
    sound_advance_frequency();
   
    cnt = 0;
  }
}

/*void
__interrupt_vec(WDT_VECTOR) WDT(){	// 250 interrupts/sec 
  static char second_count = 0, decisecond_count = 0;
  //When Global variable state changes change the pattern of flashing of lights
  
  //   led_update();
  
  }*/
