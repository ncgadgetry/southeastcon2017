/**
 *
 * Arduino connection map:
 *   D0:  (O) Rx
 *   D1:  (I) Tx
 *   D2:  (I) vibration sensor (IRQ)
 *   D3:  (I) quadrature channel B (IRQ)                                                                                                                                                                    
 *   D4:  (I) quadrature channel A
 *   D5:  unused
 *   D6:  (O) neopixel stick data
 *   D7:  (O) LED enable (active low)
 *   D8:  (O) LED red (cathode)
 *   D9:  (O) LED green (cathode)
 *   D10: (O) LED blue (cathode)
 *   D11: unused
 *   D12: unused
 *   D13: (O) magnetic coil (and UNO on-board LED)
 *   GND: neopixel, vibration, quadrature common
 *   5v:  neopixel, LED common (anode)
 *
 * KNOWN ISSUES:
 *   + stage 3 software not yet completed
 *   + reset stage 2 start period to 5 seconds (now 1 second)
 *   + reset match end time to 5 minutes (now 1 minute)
 *
 * I encourage everone to read through the code and help me identify any
 *    issues. This will make the arena electronics better for both me,
 *    your team, and all the other teams.
 * To encourage this, I will offer $5, and their name listed in the honor 
 *    section below, to the first student who can identify a new bug. If 
 *    more than one student finds the same bug, the first one gets the award. 
 *    Any bug already listed in the above known issues list are not eligible.
 *
 * STUDENT HONORS (for helping identify new issues in the software):
 *    YOUR NAME COULD BE HERE FOR EVERYONE TO SE!!
 */
 
#include <Adafruit_NeoPixel.h>

#include "Arduino.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include <Wire.h>
#include <MsTimer2.h>

Stage1 stage1;
Stage2 stage2;
Stage3 stage3;

/* Length of match runtime - should be 4 minutes, but using 1 minute for debugging */
#define MATCH_RUNTIME    (1L*60L*1000L)

int randomSeedValue = 0;
uint32_t startTimestamp = 0;
uint32_t endTimestamp   = 0;

void setup() 
{
   Serial.begin(9600);
   Wire.begin();

   // Choose a random index into the relay table. The reference to 
   //    analog channel 0 is a hack to attempt to randomize the random
   //    number generator by using an floating analog input as the
   //    randomizer
   randomSeedValue = analogRead(0);
   randomSeed(randomSeedValue);
     
   // Set the competition start and end time
   startTimestamp = millis();
   endTimestamp = startTimestamp + MATCH_RUNTIME;
   
   // Initialize processing for each stage
   stage1.start(startTimestamp);
   stage2.start(startTimestamp);
   stage3.start(startTimestamp);
   
   Serial.println(startTimestamp);
   Serial.println(endTimestamp);
}

void loop() 
{  
   uint32_t now = millis();
   int score = 0;

   // If the competition is still running, invoke each stage step (poor man's cooperative tasker)   
   if (now < endTimestamp) {
      stage1.step(now);
      stage2.step(now);
      stage3.step(now);

   // Else the competition is over, so stop everything and report the results
   } else {
   
      // Stop all the stage functions
      stage1.stop(now);
      stage2.stop(now);
      stage3.stop(now);
 
      // Add up and print the total score (not counting stage 4, which is manual)     
      score = stage1.score() + stage2.score() + stage3.score();
      Serial.print("------ RESULTS ------\n");
      Serial.print("FINAL SCORE: ");
      Serial.print(score);
      Serial.print("\nRANDOM SEED: ");
      Serial.print(randomSeedValue);      
      Serial.print("\n\n");
      
      // Print out more detail on each stage
      stage1.report();
      stage2.report();
      stage3.report();
      
      // Wait here forever
      for(;;);
   }
 }
 
