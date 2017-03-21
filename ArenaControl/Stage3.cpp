/********************************************************************
 *
 * SoutheastCon 2017 Arena control - Stage3.cpp
 *
 * This is the code file for the stage #3 control elements. 
 *
 ********************************************************************/

#include <MsTimer2.h>

#include "Arduino.h"
#include "Stage3.h"

#include "SimplePinChange.h"

#include "Stage1.h"
extern Stage1 stage1;
#include "Controller.h"
extern Controller controller;

#define ENCODER_A_PIN     3     // This one is an interrupt pin
#define ENCODER_B_PIN     4     // This is a standard (not interrupt) pin

#define ENABLE_LED_PIN    7
#define RED_LED_PIN       8
#define GREEN_LED_PIN     9
#define BLUE_LED_PIN     10

#define TICKS_PER_REVOLUTION  96
#define PLUS_MINUS             6

#define CENTER_WHITE  0x0
#define RIGHT_RED     0x1
#define LEFT_BLUE     0x2

volatile long encoderValue = 0;
int oldState = 0;
int blink = 0;
int blinkEnabled = true;
int digits[10] = { 0 };
int digitCounter = 0;

boolean movementDetected(int *encoder);
void blinkQuadratureLEDs(void);
void updateEncoder(void);


Stage3::Stage3() 
{
}

void Stage3::start() 
{
  uint16_t turnPattern;
  
  /* Set both quadrature channels to input and turn on pullup resistors */
  pinMode(ENCODER_A_PIN, INPUT_PULLUP); 
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);

  /* Get the initial state of the two quadrature pins */
  oldState = 0;
  if (digitalRead(ENCODER_A_PIN)) oldState |= 1;
  if (digitalRead(ENCODER_B_PIN)) oldState |= 2;

  /* invoke interrupt routine on each edge of ENCODER_A_PIN */
  SimplePinChange.attach(ENCODER_A_PIN, updateEncoder);
  SimplePinChange.attach(ENCODER_B_PIN, updateEncoder);  
  
  /* Setup the quadrature encoder pin modes */
  pinMode(ENABLE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN,    OUTPUT);
  pinMode(GREEN_LED_PIN,  OUTPUT);
  pinMode(BLUE_LED_PIN,   OUTPUT);

  /* Start the blink at 5hz rate (200ms period, 100ms irq rate */
  MsTimer2::set(100, blinkQuadratureLEDs);
  MsTimer2::start();

  /* Initial state of the LEDs is blinking white */
  digitalWrite(RED_LED_PIN,    LOW);
  digitalWrite(GREEN_LED_PIN,  LOW);
  digitalWrite(BLUE_LED_PIN,   LOW);
  digitalWrite(ENABLE_LED_PIN, HIGH);

  turnPattern = stage1.turnPattern;
  Serial.print("pattern=");
  Serial.println(turnPattern);

#if 0  
  int loop;
  int wrongGoals[5];
  int correctGoals[5];
  
  int plusOrMinus = +1;
  int previousGoal = 8;
  for (loop=0; loop < 5; loop++) {
     wrongGoals[4-loop] = previousGoal - 12;
     previousGoal = correctGoals[4-loop] = ((turnPattern % 10) * 96 - 8) * plusOrMinus;
     turnPattern /= 10;
     plusOrMinus *= -1;
  }
  
  for (loop=0; loop < 5; loop++) {
     Serial.print(wrongGoals[loop]);
     Serial.print(" ... ");
     Serial.println(correctGoals[loop]);
  }
  Serial.println("");
#endif  
}


void Stage3::stop(uint32_t timestamp) 
{
  /* Stop the blink LEDs */
  MsTimer2::stop();
  detachInterrupt(1);
  delay(1);

  /* Turn off the leds and the blink so the knob is off at contest end */
  digitalWrite(RED_LED_PIN,    HIGH);
  digitalWrite(GREEN_LED_PIN,  HIGH);
  digitalWrite(BLUE_LED_PIN,   HIGH);
  digitalWrite(ENABLE_LED_PIN, LOW);
}


void Stage3::step(uint32_t timestamp) 
{
   int encoder;

   /* Check if motion was detected, and control the quadrature red/white/blue
    *    LEDs. and to return the current encoder value.  We can't use a 'live'
    *    encoder value below as we could have a race condition if the encoder
    *    changed while in the code below
    */
   if (!movementDetected(&encoder)) {
      return;
   }

   if (!blinkEnabled) {
      Serial.print(F("encoder="));
      Serial.println(encoder);
   }
}


void Stage3::report(void) 
{
  Serial.print(F("------ Stage 3 report ------\n"));
  Serial.print(F("STAGE SCORE: "));
  Serial.print(score());
  Serial.print(F("\n\n"));   
 
   if (controller.attached()) {
      controller.lcdp()->setCursor(0,3);
      controller.lcdp()->print("3: ");
      controller.lcdp()->print(String(score()));
      controller.lcdp()->print(" ");
      controller.lcdp()->print(String("32154"));
   }
 
}


int Stage3::score(void) 
{
  return 0;
}

uint32_t movementHistory = 0;
long prevEncoderValue = 0;

#define MOVEMENT_HISTORY_SIZE  4
#define MOVEMENT_MASK_WIDTH    2
#define MOVEMENT_HISTORY_MASK  ((1 << MOVEMENT_MASK_WIDTH) - 1)

boolean movementDetected(int *encoder) 
{
   int curMovement;
   int loop; 
   int curDirection;
   int rightMotion  = 0;
   int leftMotion   = 0;
     
   /* If no motion, then return with nothing else to do */
   *encoder = encoderValue;
   if (prevEncoderValue == *encoder) {
       return false;
    }

   /* if we are within the plus/minus of center, set the quadrature LEDs to
      white and wipe out all past history. This is needed as if we are moving
      right to white, then reverse to left, we have more right motion than
      left and can temporarily light the LEDs the wrong color
   */
   if (abs(curMovement - *encoder) <= PLUS_MINUS) {
      curDirection = CENTER_WHITE;
      movementHistory = 0;
   
   /* Else if not int the center, we keep a running history of the last 'N'
      movement directions to average out small +/- movements setting the color
    */   
   } else {
     
      /* convert the current position to either center, or left/right of center */
      if (prevEncoderValue < *encoder) {
         curDirection = RIGHT_RED;
      } else {
         curDirection = LEFT_BLUE;
      } 
 
      /* movement history contains the history of last 'n' movements */
      movementHistory <<= MOVEMENT_MASK_WIDTH;
      movementHistory |= curDirection;
      movementHistory &= ((1 << (MOVEMENT_MASK_WIDTH*MOVEMENT_HISTORY_SIZE)) - 1);
     
      /* count the left and right movements to determine the major move direction */
      uint32_t history = movementHistory; 
     
      /* count number of 1 (right) or 0 (left) bits to determine the average direction */
      for (loop=0; loop < MOVEMENT_HISTORY_SIZE; loop++) {
         if ((history & MOVEMENT_HISTORY_MASK) == RIGHT_RED) {
            rightMotion++;
         } else if ((history & MOVEMENT_HISTORY_MASK) == LEFT_BLUE) {
            leftMotion++; 
         }
         history >>= MOVEMENT_MASK_WIDTH;
      }

      /* if we are currently at center, light the LED white, else red or 
       *   blue based on the motion counts above
       */
      if (leftMotion > rightMotion) {
         curDirection = LEFT_BLUE;
      } else {
         curDirection = RIGHT_RED;
      }

Serial.print("history=");
Serial.print(movementHistory);
Serial.print(", dir=");
Serial.println(curDirection);
   }
   
   /* Tricky code to handle the 3x conditions of red, white and blue for
    *   the quadrature encoding.  Green is only on if we are in the center
    *   position (not on for left or right), while red is off only when
    *   not heading left, and blue is only on when not heading right.
    */
   digitalWrite(GREEN_LED_PIN, !(CENTER_WHITE == curDirection));
   digitalWrite(RED_LED_PIN,   !(LEFT_BLUE    != curDirection));
   digitalWrite(BLUE_LED_PIN,  !(RIGHT_RED    != curDirection));

   /* If we moved out of the center area, disable blinking of the LEDs */
   if ((CENTER_WHITE != curDirection) && (blinkEnabled)) {
      blinkEnabled = false;
   }

   /* update previous encoder value for the next iteration */
   prevEncoderValue = *encoder;

   /* return true indicating we did have motion */
   return true;
}  


/* Toggle the quadrature LEDs on and off This function is invoked via
 *    the a timeout counter callback at a rate of 10Hz (100ms) to generate
 *    a 5Hz blink rate.
 */
void blinkQuadratureLEDs() {
  static boolean onOff = HIGH;
  
  /* if blink is enabled, then toggle the enable line */
  if (blinkEnabled) {
     digitalWrite(ENABLE_LED_PIN, onOff);
     onOff = !onOff;
     
  /* else turn off the blink timer and set the enable pin to always on */
  } else {
     MsTimer2::stop();
     digitalWrite(ENABLE_LED_PIN, HIGH);
  }
}


/* This quadrature encoder interrupt handler is based heavily
 *   on an the implementation in the Encoder library at:
 *
 *     https://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * First an overview of the two lines in a quadrature encoder:
 *               _____       _____
 *   Pin1:  ____|     |_____|     |____
 *            _____       _____       _  
 *   Pin2:  _|     |_____|     |_____|
 *
 *        <--- negative     positive --->
 *
 * By examining the current two pins and the two previous
 *    pins, we can determine the position and amount of
 *    motion (assume we can respond fast enough to only 
 *    lose at most 1 transition).
 *
 *   new    new    old    old
 *   pin2   pin1   pin2   pin1   Result
 *   ----   ----   ----   ----   ------
 *    0      0      0      0     no movement
 *    0      0      0      1     +1
 *    0      0      1      0     -1
 *    0      0      1      1     +2
 *    0      1      0      0     -1
 *    0      1      0      1     no movement
 *    0      1      1      0     -2
 *    0      1      1      1     +1
 *    1      0      0      0     +1
 *    1      0      0      1     -2
 *    1      0      1      0     no movement
 *    1      0      1      1     -1
 *    1      1      0      0     +2
 *    1      1      0      1     -1
 *    1      1      1      0     +1
 *    1      1      1      1     no movement
 *   ----   ----   ----   ----   ------
 */

/* state change based on table above */
const int stateChange[16] = 
{  
   0, +1, -1, +2, -1,  0, -2, +1, 
  +1, -2,  0, -1, +2, -1, +1,  0 
};

void updateEncoder(void) 
{
  /* Build up the 4bit state of current and past pin values */
  uint8_t state = oldState & 3;
  if (digitalRead(ENCODER_A_PIN)) state |= 4;
  if (digitalRead(ENCODER_B_PIN)) state |= 8;

  /* Based on 4-bit state value, update encoder value */
  encoderValue += stateChange[state];  

  /* Save our current two pins for the next interrupt */
  oldState = (state >> 2);
}


