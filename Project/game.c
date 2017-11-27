/** \file game.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <stdio.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "sound.h"

#define GREEN_LED BIT6
int player1Score=0;
int player2Score=0;
char snumPlayer1[5];
char snumPlayer2[5];
char splay1[5];
AbRect rect10 = {abRectGetBounds, abRectCheck, {2,12}}; /**< 10x10 rectangle */

AbRArrow rightArrow = {abRArrowGetBounds, abRArrowCheck, 30};

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {(screenWidth/2)-1, (screenHeight/2)-10 }
};
Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {(screenWidth/2), screenHeight/2},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0
};



Layer layer0 = {
  (AbShape *)&circle3,
  {(screenWidth/2)+10,(screenHeight/2)+5},
  {0,0},{0,0},
  COLOR_ORANGE,
  &fieldLayer,
};

Layer layer1 = {
  (AbShape *)&rect10,
  {(screenWidth/2)-55,(screenHeight/2)+20},
  {0,0},{0,0},
  COLOR_ORANGE,
&layer0
};
Layer layer2 = {
  (AbShape *)&rect10,
  {(screenWidth/2)+55,(screenHeight/2)+20},
  {0,0},{0,0},
  COLOR_ORANGE,
&layer0
};
/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

/* initial value of {0,0} will be overwritten */
 
MovLayer ml0 = { &layer0, {2,1}, 0 }; 
MovLayer m11 = {&layer1, {1,1},0};
MovLayer m12 = {&layer2, {1,1},0};
void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}
void vect2AddNew(Vec2 *result, const Vec2 *v1, const Vec2 *v2){
  u_char axis;
  for(axis=0;axis < 2; axis ++){
    result->axes[axis] = v1->axes[axis] + v2->axes[axis];
    
  }
}
/*
Method to move new position of paddle down when button is pressed
 */
void moveLeftPaddleUP(MovLayer *ml){
   Vec2 newPos;
  Vec2 player1NewPos;
  Vec2 player2NewPos;
  u_char axis;
  Region shapeBoundary;
  Region shapeBoundaryMovSqrs1,shapeBoundaryMovSqrs2;
  //set a boundary
  if(newPos.axes[1]<272) {
  vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
 
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    
    // itoa(newPos.axes[1],splay1,10);
    // drawString5x7(30,50, splay1, COLOR_GREEN, COLOR_BLUE); 
  		newPos.axes[1] += 2;
		newPos.axes[0] = 10;
		ml->layer->posNext = newPos;
}
 else{
   //stop paddle
 } 
}
/*
Method to move new position of paddle down when button is pressed
 */
void moveLeftPaddleDOWN(MovLayer *m3){
   Vec2 newPos;
  Vec2 player1NewPos;
  Vec2 player2NewPos;
  u_char axis;
  Region shapeBoundary;
  Region shapeBoundaryMovSqrs1,shapeBoundaryMovSqrs2;
    if(newPos.axes[1]<272) {
  vec2Sub(&newPos, &m3->layer->posNext, &m3->velocity);
      abShapeGetBounds(m3->layer->abShape, &newPos, &shapeBoundary);
      // itoa(newPos.axes[1],splay1,10);
      // drawString5x7(30,50, splay1, COLOR_GREEN, COLOR_BLUE); 
  		newPos.axes[1] -= 2;
		newPos.axes[0] = 10;
  		  m3->layer->posNext = newPos;
  
    }else{
      //stop paddle
    }
}
/*
Method to move new position of paddle down when button is pressed
 */
void moveRightPaddleUP(MovLayer *ml){
   Vec2 newPos;
  Vec2 player1NewPos;
  Vec2 player2NewPos;
  u_char axis;
  Region shapeBoundary;
  Region shapeBoundaryMovSqrs1,shapeBoundaryMovSqrs2;
  //set a boundary
  if(newPos.axes[1]<272) {
  vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
 
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    
    // itoa(newPos.axes[1],splay1,10);
    // drawString5x7(30,50, splay1, COLOR_GREEN, COLOR_BLUE); 
  		newPos.axes[1] += 2;
		newPos.axes[0] = 120;
		ml->layer->posNext = newPos;
}
 else{
   //stop paddle
 } 
}
/*
Method to move new position of paddle down when button is pressed
 */
void moveRightPaddleDOWN(MovLayer *m3){
   Vec2 newPos;
  Vec2 player1NewPos;
  Vec2 player2NewPos;
  u_char axis;
  Region shapeBoundary;
  Region shapeBoundaryMovSqrs1,shapeBoundaryMovSqrs2;
    if(newPos.axes[1]<272) {
  vec2Sub(&newPos, &m3->layer->posNext, &m3->velocity);
      abShapeGetBounds(m3->layer->abShape, &newPos, &shapeBoundary);
      // itoa(newPos.axes[1],splay1,10);
      //  drawString5x7(30,50, splay1, COLOR_GREEN, COLOR_BLUE); 
  		newPos.axes[1] -= 2;
		newPos.axes[0] = 120;
  		  m3->layer->posNext = newPos;
  
    }else{
      //stop paddle
    }
}

//Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
//int axisx;
// int axisy;
void mlAdvance(MovLayer *ml,MovLayer *m2,MovLayer *m3, Region *fence,Region *player1,Region *player2)
{
  Vec2 newPos;
  Vec2 player1NewPos;
  Vec2 player2NewPos;
  u_char axis;
  Region shapeBoundary;
  Region shapeBoundaryMovSqrs1,shapeBoundaryMovSqrs2;
  for (; ml; ml = ml->next) {
    
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    vec2Add(&player1NewPos, &m2->layer->posNext, &m2->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
  abShapeGetBounds(m2->layer->abShape,&player1NewPos,&shapeBoundaryMovSqrs1);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
		newPos.axes[axis] += (2*velocity);
		  itoa(newPos.axes[axis],splay1,10);
   drawString5x7(30,50, splay1, COLOR_GREEN, COLOR_BLUE);
    	if(shapeBoundary.topLeft.axes[0]== 0){
  		  player1Score++;
		  itoa(player2Score,snumPlayer2,10);
  drawString5x7(2,0, "Score Player 1:", COLOR_GREEN, COLOR_BLUE);
  drawString5x7(95,0, snumPlayer2, COLOR_GREEN, COLOR_BLUE);
		  
 
		  //newPos.axes[axis]=0;
		  // velocity =0;
		  
 newPos.axes[0] = 60;
		  newPos.axes[1] = 80;
		  play ='0';
		   sound_init();
		  		}
		else if(shapeBoundary.botRight.axes[0]>120){
		  player2Score++;
		  itoa(player1Score,snumPlayer1,10);
   drawString5x7(2,152, "Score Player 2:", COLOR_GREEN, COLOR_BLUE);
   drawString5x7(95,152, snumPlayer1, COLOR_GREEN, COLOR_BLUE);

   newPos.axes[0] = 60; 
		  newPos.axes[1] = 80;
		  play = '1';
		   sound_init();
		}	
      }

      
      /*      else if((shapeBoundary.topLeft.axes[0] < player1->botRight.axes[0])){
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
		newPos.axes[axis] += (2*velocity);
		}*/
	
      /**< if outside of fence */
      /*else if(shapeBoundary.topLeft.axes[1] < player1->botRight.axes[1]){
		int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
		
		newPos.axes[axis] += (2*velocity);
		//axisx=newPos.axes[0];
		//axisy=newPos.axes[1];
		//printf('%d',newPos);
		}*/
     /**< for axis */
    }
       ml->layer->posNext = newPos;
  } /**< for ml */
}
/*void buttonStates(int i){
  if(){

  }
  

    

  }*/

u_int bgColor = COLOR_BLUE;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */

Region player01,player02;
/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;



  
  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(15);
  //sound_init();
  shapeInit();

  layerInit(&layer0);
  layerDraw(&layer0);
  layerInit(&layer1);
  layerDraw(&layer1);

  layerInit(&layer2);
  layerDraw(&layer2);
  
 
  layerGetBounds(&fieldLayer, &fieldFence);
  layerGetBounds(&layer1, &player01);
   layerGetBounds(&layer2,&player02);
  itoa(player2Score,snumPlayer2,10);
  drawString5x7(2,0, "Score Player 1:", COLOR_GREEN, COLOR_BLUE);
  drawString5x7(95,0, snumPlayer2, COLOR_GREEN, COLOR_BLUE);

  //sound_init();
  itoa(player1Score,snumPlayer1,10);
   drawString5x7(2,152, "Score Player 2:", COLOR_GREEN, COLOR_BLUE);
   drawString5x7(95,152, snumPlayer1, COLOR_GREEN, COLOR_BLUE);
 
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
 
  for(;;) {
    
 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);
      
 
 u_int switches = p2sw_read(), i;
    char str[5];
    //States to move paddles up and down
     
     if((switches & (1<<0))==0){
       moveLeftPaddleDOWN(&m11);
     }
     else if((switches & (1<<1))==0){
       moveLeftPaddleUP(&m11);
     }
     else if((switches & (1<<2))==0){
       moveRightPaddleDOWN(&m12);
     }
     else if((switches & (1<<3))==0){
       moveRightPaddleUP(&m12);
     }
    /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    //  printf('%d',axisx);
    movLayerDraw(&m11, &layer1); 
    movLayerDraw(&ml0, &layer0);
     movLayerDraw(&m12,&layer2);
    //hello
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;
  static char second_count = 0, decisecond_count = 0;
  //When Global variable state changes change the pattern of flashing of lights
  
  /**< Green LED on when cpu on */
  count ++;
  if (count == 15){
    /* for (i = 0; i < 4; i++){
      str[i] = (switches & (1<<i)) ? '-' : '0'+i;
    str[4] = 0;
     drawString5x7(20,20, str, COLOR_GREEN, COLOR_BLUE);
     }*/
     
    mlAdvance(&ml0,&m11, &m12,&fieldFence,&player01,&player02);
    
    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  } 
  
  //   led_update();
  
  
  if(++decisecond_count == 25){
    
    if(play =='1'){
      
    sound_advance_frequency();
    decisecond_count=0;
    }
  }
  

  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
