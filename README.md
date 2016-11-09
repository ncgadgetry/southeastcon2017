# Southeastcon 2017 

IEEE Southeastcon 2017 (The Engineering Force Awakens) competition files.

# Contents:

* ArenaControl - the Arduino source code for the control electronics

# References

* [Schematics and PCB files](https://github.com/petesoper/arena-pcbs)

   A big thank you and appreciation for the work of Pete Soper for the five arena schematic and PCB boards.

   * Stage 1 Probe Pads - the 6x pad board behind stage 1
   * Stage 1 I2C Board - I2C board that controls the relay tree to select components
   * Stage 2 Arduino Shield - the Arduino shield with connections to each of the other boards
   * Stage 3 Quadrature Encoder - stage 3 quadrature control board
   * Controller/Display - LCD display and contest start/stop buttons
   
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

* [Contest Rules](http://sites.ieee.org/southeastcon2017/files/2016/10/MMXVII-October-9-release.pdf)

* [Official IEEE SoutheastCon 2017 site](http://sites.ieee.org/southeastcon2017)
