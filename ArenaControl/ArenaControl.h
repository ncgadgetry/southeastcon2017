/*
 * Arduino connection map:
 *   D0:  (O) Rx (currently unused)
 *   D1:  (I) Tx (currently unused)
 *   D2:  (I) vibration sensor (IRQ)
 *   D3:  (I) quadrature channel B (IRQ)                                                                                                                                                                    
 *   D4:  (I) quadrature channel A
 *   D5:  (I) quadrature pushbutton (currently unused)
 *   D6:  (O) neopixel stick data
 *   D7:  (O) LED enable (active low)
 *   D8:  (O) LED red (cathode)
 *   D9:  (O) LED green (cathode)
 *   D10: (O) LED blue (cathode)
 *   D11: unused (routed to external I/O header)
 *   D12: unused (routed to external I/O header)
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
 * Anyone who identifies any issues now (before it is complete) will have 
 *    their name listed in the collaboration section below. Once the code
 *    is listed as officially complete (expected date is January 1), will
 *    have their name listed and will received $5 gift (handed out in 
 *    Charlotte at Southeastcon - you must be present). 
 * So you can help me find the issues now and get your name listed, or 
 *    wait and hope to get your name and cash.
 * Note that only the first person to find each issue is listed (and
 *    optionally gets the cash).
 * Any bug already listed in the above known issues list are not eligible.
 *
 * COLLABORATORS / HONORS (for helping identify new issues in the software):
 *    YOUR NAME COULD BE HERE FOR EVERYONE TO SEE!
 */
