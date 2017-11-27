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

