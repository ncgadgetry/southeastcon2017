# Southeastcon 2017 

This github contains the arena control software for the IEEE Southeastcon 2017 hardware competition to be held in Charlotte, NC on April 1, 2017.

# Contents:

* ArenaControl - the Arduino source code for the control electronics

   The arena source code is still a work in progress and is not yet complete. Once complete, I will announce an offer for
   free cash and fame to anyone who can find any remaining bugs in the code. That offer is NOT available at this time, but you are
   encouraged to read and understand the code. If you do find an issue now and want to pass it on to me, I will still add 
   add your name to the list of contributors, but no cash prize. Or you can wait and hope the issue has not been fixed or
   reported by someone else. 

# References

* [Schematics and PCB files](https://github.com/petesoper/arena-pcbs)

   A big thank you and appreciation to Pete Soper for his work on the five arena schematic and PCB boards:

   * Stage 1 Probe Pads - the 6x pad board behind stage 1
   * Stage 1 I2C Board - I2C board that controls the relay tree to select components
   * Stage 2 Arduino Shield - the Arduino shield with connections to each of the other boards
   * Stage 3 Quadrature Encoder - stage 3 quadrature control board
   * Controller/Display - LCD display and contest start/stop buttons
   
* [Contest Rules](http://sites.ieee.org/southeastcon2017/files/2016/10/MMXVII-October-9-release.pdf)

* [Official IEEE SoutheastCon 2017 site](http://sites.ieee.org/southeastcon2017)

* Arduino connection map:

   *   D0:  (O) Rx
   *   D1:  (I) Tx
   *   D2:  (I) vibration sensor (IRQ)
   *   D3:  (I) quadrature channel B (IRQ)
   *   D4:  (I) quadrature channel A
   *   D5:  (I) quadrature pushbutton switch
   *   D6:  (O) neopixel stick data
   *   D7:  (O) LED enable
   *   D8:  (O) LED red (cathode)
   *   D9:  (O) LED green (cathode)
   *   D10: (O) LED blue (cathode)
   *   D11: (I/O) auxiliary I/O
   *   D12: (I/O) auxiliary I/O
   *   D13: (O) magnetic coil (and UNO on-board LED)
   *   GND: neopixel, vibration, quadrature common
   *   5v:  neopixel, LED common (anode)

