/*
 * Simple program to calculate and print the permutations of a 16
 *   relay configuration to create the component combinations for
 *   the Southeastcon 2017 hardware competition
 *
 * Author: Rodney Radford (with lots of help from Pete Soper)
 *
 * It is assumed the components are connected as below, yet any
 *    valid connection is valid as long as they follow the rule 
 *    that each can connect to only 4 pads, and they each pick
 *    a unique pad with which they cannot connect).
 *
 *  W = wire (skips 1)
 *      can connect to pads 2, 3, 4, 5
 *  R = resistor (skips 2)
 *      can connect to pads 1, 3, 4, 5
 *  C = capacitor (skips 3)
 *      can connect to pads 1, 2, 4, 5
 *  I = inductor (skips 4)
 *      can connect to pads 1, 2, 3, 5
 *  D - diode (skips 5)
 *      (also use 'd' to indicate diode in opposite polarity)
 */

#include <stdio.h>

// Number of valid combinations found
int count = 0;

/*
 * These two macros generate the table to control the 3x relays
 *    for each component. The first entry is always zero - no
 *    relays engaged, then the right leaf node is engaged, then
 *    only the top level relay is engaged (selecting the left
 *    leaf relay, but that relay is not engaged), and finally
 *    selecting the left relay and engaging it
 */
#define BIT(x) (1 << x)
#define BIT_PATTERN_GENERATOR(top,right,left)  { \
         0, \
         BIT(right), \
         BIT(top), \
         BIT(top) | BIT(left) }

/*
 * Table of 3-relay tree bit patterns for a specific component. 
 *    Each table has four entries for the 4 valid combinations
 *    of the 3x relays (8x possible patterns, but only 4x valid)
 */
int w_bit_patterns[] = BIT_PATTERN_GENERATOR(12,13,14);
int r_bit_patterns[] = BIT_PATTERN_GENERATOR( 9,10,11);
int c_bit_patterns[] = BIT_PATTERN_GENERATOR( 6, 7, 8);
int i_bit_patterns[] = BIT_PATTERN_GENERATOR( 3, 4, 5);
int d_bit_patterns[] = BIT_PATTERN_GENERATOR( 0, 1, 2);

/* The #16 relay is used to select between D (anode) and d
 *     (cathode) configuration of the diode. The center of
 *     the SPDT switch for this relay connects to the 'top'
 *     relay for the diode tree, while the NC side connects
 *     to 'D' while NO connects to 'd'
 */
#define D_d_relay 15

// Determine if the pad is reachable by this component - mapping it to
//   the 1-origin pad number, with 0 being an invalid (unreachable) pad
int w_valid[] = { 0, 2, 3, 4, 5 };
int r_valid[] = { 1, 0, 3, 4, 5 };
int c_valid[] = { 1, 2, 0, 4, 5 };
int i_valid[] = { 1, 2, 3, 0, 5 };
int d_valid[] = { 1, 2, 3, 4, 0 };

// This is necessary to map a 0-origin pad number into a relay bit pattern for
// each device. These element values index into the bit pattern arrays to
// select a relay energizing sequence (000, 010, 001, 101) for the three
// elements connecting the device to one of four pads. It's impossible to
// use a -1 value: they only indicate the unused fifth pattern for a given
// device.
//  origin    pad:   1   2  3  4  5
int w_pad_map[] = { -1,  0, 1, 2, 3 };
int r_pad_map[] = {  0, -1, 1, 2, 3 };
int c_pad_map[] = {  0,  1,-1, 2, 3 };
int i_pad_map[] = {  0,  1, 2,-1, 3 };
int d_pad_map[] = {  0,  1, 2, 3,-1 };

/*
 * Verify if the pad placement for each component is valid
 */
int valid_combination(int w, int r, int c, int i, int d)
{
    // If the relay matrix cannot reach, obviously invalid
    if ((d_valid[d] == 0) ||
        (i_valid[i] == 0) ||
        (r_valid[r] == 0) ||
        (c_valid[c] == 0) ||
        (w_valid[w] == 0)) 
       return 0;

    // Also invalid if any are connecting to the same pad
    // Simplest test - only ignore comparisons to self, even
    //   though I still compare redundant X==Y and Y==X
    //   W       R       C       I       D
    if (        w==r || w==c || w==i || w==d ||    // W
        r==w         || r==c || r==i || r==d ||    // R
        c==w || c==r         || c==i || c==d ||    // C
        i==w || i==r || i==c         || i==d ||    // I
        d==w || d==r || d==c || d==i)              // D
       return 0;

    // can reach, and no duplicates, so must be valid
    return 1;
}

/*
 * Unfortunately C does NOT have a binary print format, so we have
 *    to generate our own.  Admittedly these numbers could be printed
 *    in hex, but it is easier to visually see which relays should
 *    be energized in a binary format.
 */
char *B16_format(int n) {
   static char buffer[32];
   char *cptr = buffer;
   int i;

   *cptr++ = '0';
   *cptr++ = 'b';

   for (i=0; i < 16; i++)
      *cptr++ = (n & BIT(15-i)) ? '1' : '0';
   *cptr++ = '\0';

   return buffer;
}

/* 
 * If the combination is valid, print it out
 *
 * NOTE: This could be modified to print out an array of 16
 *       bit numbers to compile into the Arduino code to feed
 *       directly to the relays
 */
void print_combination(int w, int r, int c, int i, int d)
{
    char buffer[5+1], code[5+1];
    int  bit_pattern;

    // human readable version of the pattern
    buffer[5] = '\0';   code[5] = '\0';
    buffer[w] = 'W';    code[w] = '1';
    buffer[r] = 'R';    code[r] = '2';
    buffer[c] = 'C';    code[c] = '3';
    buffer[i] = 'I';    code[i] = '4';
    buffer[d] = 'D';    code[d] = '5';

    // relay bit pattern (16-bit value actually sent to relay board)
    // relay bit pattern (16-bit value actually sent to relay board)
    bit_pattern = w_bit_patterns[w_pad_map[w_valid[w]]] |
                  r_bit_patterns[r_pad_map[r_valid[r]]] |
                  c_bit_patterns[c_pad_map[c_valid[c]]] |
                  i_bit_patterns[i_pad_map[i_valid[i]]] |
                  d_bit_patterns[d_pad_map[d_valid[d]]];

    // Print out the 'D' bits pattern - add +1 for each turn as
    //    pads are zero-based, but turn count is one-based
    printf("     { %s, %s }, // %s - #%d\n", 
            B16_format(bit_pattern), code, buffer, count);
    count++;


    // Now bit flip the D_d_relay to select the 'd' bits pattern
    buffer[d] = 'd';
    bit_pattern ^= BIT(D_d_relay);
    printf("     { %s, %s }, // %s - #%d\n", 
            B16_format(bit_pattern), code, buffer, count);
    count++;
}


void print_header() {
   printf("/*\n");
   printf(" * Southeastcon 2017 Stage 1 relay tree control patterns\n");
   printf(" *\n");
   printf(" * Table consists of binary pattern to the 16 relays, followed\n");
   printf(" *    by the number of turns required at stage 3\n");
   printf(" *\n");
   printf(" * The table contains the bit patterns for the relay (active\n");
   printf(" *    low), and the number of turns for the pad. The number of\n");
   printf(" *    turns is encoded in a 5-digit integer, with MSB the number\n");
   printf(" *    of turns for pad 1 (12 o'clock, and LSB being the number of\n");
   printf(" *    turns for pad 5 (9-10 o'clock).\n");
   printf(" * This does not allow all possible combinations), but is a reasonable\n");
   printf(" *    enough size tree of values (88 combinations) that it is not \n");
   printf(" *    possible to guess), and uses only 16 relays (as compared to 25 \n");
   printf(" *    for a simplistic approach that allows all combinations).\n");
   printf(" *\n");
   printf(" * This table was computer generated - do not hand edit!!\n");
   printf(" */\n\n");

   printf("#define RELAY_TABLE_LENGTH (sizeof(relayTable) / sizeof(relayTable[0]))\n");
   printf("\n");
   printf("const uint16_t relayTable[][2] PROGMEM = {\n\n");
   printf("//     Relay bit pattern   turns       pad\n");
   printf("//     __d_W__R__C__I__D_  12345       12345   ID\n");

}

void print_trailer() {
   printf("};\n");
}


int main(int argc, char **argv) 
{
   int w, r, c, i, d;

   print_header();

   // Blindly iterate over all combinations, testing if valid
   //    and only printing (and counting) the valid ones
   for (w=0; w < 5; w++)
      for (r=0; r < 5; r++)
          for (c=0; c < 5; c++)
              for (i=0; i < 5; i++)
                  for (d=0; d < 5; d++)
                     if (valid_combination(w,r,c,i,d))
                        print_combination(w,r,c,i,d);
   
   print_trailer();
   return 0;
}


