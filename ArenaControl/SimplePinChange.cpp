/*
 * Simple support pin change interrupts tested with Atmega328 chips (e.g.
 * Arduino Uno, Nano, Mini) 
 *
 * Copyright (c) 2015 Peter James Soper
 * April 27, 2016
 */

#include "SimplePinChange.h"

// Callback for port change or NULL if none defined
static volatile voidFuncPtr callback[NUM_PORTS];

// Mask of spurious interrupts on a port lacking a callback (see vector 
// functions for values)
static uint8_t spurious_interrupt;

// Treat the interface as a global class instance
SimplePinChangeClass SimplePinChange;

// Return spurious interrupts 
uint8_t SimplePinChangeClass::getError() {
  uint8_t temp = spurious_interrupt;
  spurious_interrupt = 0;
  return temp;
}

// Define a callback function for a change of the given pin. Return true if
// the port handling this pin has already been subscribed. This is 
// informational. There's no problem with multiple pins, but the callback
// function has to determine which of the multiple pins changed in this case.
// An invalid pin number results in an immediate return of false

bool SimplePinChangeClass::attach(uint8_t pin, void (*handler)()) {
  volatile uint8_t *p = digitalPinToPCMSK(pin);

  if (p == NULL) {
    return false;
  }

  // Not clear how much common subexpression elimination the compiler does,
  // so just figure out this bit number once to save a few bytes.
  uint8_t pcicr_bit = digitalPinToPCICRbit(pin);

  bool subscription = callback[pcicr_bit];

  // Disables any pending interrupt to avoid a spurious one
  PCICR  &= ~bit(pcicr_bit);

  // Define the handler 

  callback[pcicr_bit] = handler;

  // Sets the pin change mask value for the port controlling the give pin.
  *p |= bit(digitalPinToPCMSKbit(pin)); 

  // Enable pin change interrupts for the port containing the given pin
  PCICR  |= bit(pcicr_bit);

  return subscription;
}

// Remove the association of a change of the given pin with an interrupt.
// Return true if this was the last pin for a port (the callback is nulled
// in this case).
// An invalid pin number results in an immediate return of false

bool SimplePinChangeClass::detach(uint8_t pin) {

  uint8_t port;
  int p = digitalPinToPCMSKbit(pin);

  if (digitalPinToPCMSK(pin)) {
    // Clear mask bit
    p &= ~bit (digitalPinToPCMSKbit(pin));

    // If the mask is clear now disable port interrupt and remove the handler
    if (!p) {

      // Disable pin change interrupts for the port containing the given pin
      PCICR  &= ~bit (digitalPinToPCICRbit(pin));

      // Undefine handler
      callback[digitalPinToPCICRbit(pin)] = NULL;
      return true;
    }
  }

  return false;
}

// Vector for pin 8-13 change interrupt
ISR(PCINT0_vect) {
  if (callback[0]) {
    callback[0]();
  } else {
    // This should be impossible!
    spurious_interrupt |= 1;
  }
}

// Vector for pin 14-21 (A0-A7) change interrupt
ISR(PCINT1_vect) {
  if (callback[1]) {
    callback[1]();
  } else {
    // This should be impossible!
    spurious_interrupt |= 2;
  }
}

// Vector for pin 0-7 change interrupt
ISR(PCINT2_vect) {
  if (callback[2]) {
    callback[2]();
  } else {
    // This should be impossible!
    spurious_interrupt |= 4;
  }
}
