#ifndef sound_included
#define sound_included
//global variable state
extern char state;
extern char play;
void sound_init();
void sound_advance_frequency();
void soundOne();
void soundTwo();
void soundThree();
void soundFour();
void sound_set_period(short cycles);

#endif // included
