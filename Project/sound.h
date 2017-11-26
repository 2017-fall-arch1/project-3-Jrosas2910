#ifndef sound_included
#define sound_included
//global variable state
extern char state;
void sound_init(char n);
void sound_advance_frequency();
void soundOne();
void soundTwo();
void soundThree();
void soundFour();
void sound_set_period(short cycles);

#endif // included
