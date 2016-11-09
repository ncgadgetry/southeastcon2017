# Southeastcon 2017 

IEEE Southeastcon 2017 (The Engineering Force Awakens) competition files.

# Contents:

* ArenaControl - the Arduino source code for the control electronics

# References

* [Contest Rules](http://sites.ieee.org/southeastcon2017/files/2016/10/MMXVII-October-9-release.pdf)

* [Schematics and PCB files](http://sites.ieee.org/southeastcon2017/files/2016/10/MMXVII-October-9-release.pdf)
A big thank you and appreciation for the work of Pete Soper for the five arena schematic and PCB boards.

   * Stage 1 Probe Pads - Kicad
   * Stage 1 I2C Board - Eagle
   * Stage 2 Arduino Shield - Eagle
   * Stage 3 Quadrature Encoder - Eagle
   * Controller/Display - Eagle
   
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

* [Official IEEE SoutheastCon 2017 site](http://sites.ieee.org/southeastcon2017)

