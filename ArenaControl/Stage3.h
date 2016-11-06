/********************************************************************
 *
 * SoutheastCon 2017 Arena control - Stage3.h
 *
 * This is the header file for the stage #3 control elements. 
 *
 * Stage3 control is responsible for rotary encoder motion and
 * encoder LED control.
 *
 ********************************************************************/

#ifndef Stage3_h
#define Stage3_h

#include "Arduino.h"

class Stage3 
{
   public:
      Stage3();

      void start(uint32_t timestamp);
      void stop(uint32_t timestamp);
      void step(uint32_t timestamp);
      void report(void);
      int  score(void);

   private:
      uint32_t _startTimestamp;
};

#endif
