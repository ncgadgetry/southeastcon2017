/********************************************************************
 *
 * SoutheastCon 2017 Arena control - Stage1.h
 *
 * This is the header file for the stage1 control elements. 
 *
 * Stage1 control is responsible for choosing the location and
 * orientation of the components to be discovered in stage1. 
 *
 * These components consist of a resistor, inductor, capacitor, 
 * diode and straight wire. A relay tree is used to choose where 
 * the components are connected to a grid that must be detected 
 * by the contestant's robot.
 *
 * Once selected, this information is passed on to stage3 which
 * verifies whether the correct component placement was detected
 * by the robot.
 *
 ********************************************************************/

#ifndef Stage1_h
#define Stage1_h

#include "Arduino.h"

class Stage1 
{
   public:
      Stage1();

      void start(uint32_t timestamp);
      void stop(uint32_t timestamp);
      void step(uint32_t timestamp);
      void report(void);
      int  score(void);
            
      uint16_t pattern();

   private:
      uint32_t _startTimestamp;
};

#endif

