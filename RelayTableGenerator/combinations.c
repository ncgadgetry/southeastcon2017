
#include <stdio.h>

/*
 * Simple program to calculate and print the permutations of a 16
 *   relay configuration to create the component combinations for
 *   the Southeastcon 2017 hardware competition
 *
 * Author: Rodney Radford
 *
 * It is assumed the components are connected as below, yet any
 *    valid connection is valid as long as they follow the rule 
 *    that each can connect to only 4 pads, and they each pick
 *    a unique pad with which they cannot connect).
 *
 *  D - diode (skips 5)
 *      (also use 'd' to indicate diode in opposite polarity)
 *      can connect to 1, 2, 3, 4
 *  I = inductor (skips 4)
 *      can connect to pads 1, 2, 3, 5
 *  C = resistor (skips 3)
 *      can connect to pads 1, 2, 4, 5
 *  R = capacitor (skips 2)
 *      can connect to pads 1, 3, 4, 5
 *  W = wire (skips 1)
 *      can connect to pads 2, 3, 4, 5
 */

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
#define BIT_PATTERN_GENERATOR(top,right,left)  { 0, BIT(right), BIT(top), BIT(top) | BIT(left) }

/*
 * Table of 3-relay tree bit patterns for a specific component. 
 *    Each table has four entries for the 4 valid combinations
 *    of the 3x relays (8x possible patterns, but only 4x valid)
 * Note that the bits below are 1-based to match the relays also 
 *    being one based. The bit print routine drops the LSB bit
 *    when printing, so the bits are then converted to 0-based
 *    as needed for the control to the I2C board. 
 */
int w_bit_patterns[] = BIT_PATTERN_GENERATOR( 1, 2, 3);
int i_bit_patterns[] = BIT_PATTERN_GENERATOR( 4, 5, 6);
int r_bit_patterns[] = BIT_PATTERN_GENERATOR( 7, 8, 9);
int c_bit_patterns[] = BIT_PATTERN_GENERATOR(10,11,12);
int d_bit_patterns[] = BIT_PATTERN_GENERATOR(13,14,15);

/* The #16 relay is used to select between D (anode) and d
 *     (cathode) configuration of the diode. The center of
 *     the SPDT switch for this relay connects to the 'top'
 *     relay for the diode tree, while the NC side connects
 *     to 'D' while NO connects to 'd'
 */
#define D_d_relay 16

/* 
 * Table of pads that can reach a particular component - zero based 
 *    numbering with -1 for invalid (ie: can't reach the pad)
 */
int d_valid[] = {  0,  1,  2,  3, -1 };
int i_valid[] = {  0,  1,  2, -1,  3 };
int r_valid[] = {  0,  1, -1,  2,  3 };
int c_valid[] = {  0, -1,  1,  2,  3 };
int w_valid[] = { -1,  0,  1,  2,  3 };


/*
 * Verify if the pad placement for each component is valid
 */
int valid_combination(int d, int i, int r, int c, int w)
{
    // If the relay matrix cannot reach, obviously invalid
    if ((d_valid[d] == -1) ||
        (i_valid[i] == -1) ||
        (r_valid[r] == -1) ||
        (c_valid[c] == -1) ||
        (w_valid[w] == -1)) 
       return 0;

    // Also invalid if any are connecting to the same pad
    // Simplest test - only ignore comparisons to self, even
    //   though I still compare redundant X==Y and Y==X
    //   D       I       C       R       W
    if (        d==i || d==c || d==r || d==w ||    // D
        i==d         || i==c || i==r || i==w ||    // I
        c==d || c==i         || c==r || c==w ||    // C
        r==d || r==i || r==c         || r==w ||    // R
        w==d || w==i || w==c || w==r)              // W
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
      *cptr++ = (n & BIT(16-i)) ? '1' : '0';
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
void print_combination(int d, int i, int r, int c, int w)
{
    char buffer[5+1], code[5+1];
    int  bit_pattern;

    // human readable version of the pattern
    buffer[5] = '\0';   code[5] = '\0';
    buffer[d] = 'D';    code[d] = '5';
    buffer[i] = 'I';    code[i] = '4';
    buffer[c] = 'C';    code[c] = '3';
    buffer[r] = 'R';    code[r] = '2';
    buffer[w] = 'W';    code[w] = '1';

    // relay bit pattern (16-bit value actually sent to relay board)
    bit_pattern = d_bit_patterns[d_valid[d]] |
                  i_bit_patterns[i_valid[i]] |
                  c_bit_patterns[c_valid[c]] |
                  r_bit_patterns[r_valid[r]] |
                  w_bit_patterns[w_valid[w]];

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
   printf(" *    turns is encoded in a 5-digit integer, with LSB the number\n");
   printf(" *    of turns for pad 0, and MSB being the number of turns for\n");
   printf(" *    pad 5.\n");
   printf(" * This does not allow all possible combinations), but is a reasonable\n");
   printf(" *    enough size tree of values (88 combinations) that it is not \n");
   printf(" *    possible to guess), and uses only 16 relays (as compared to 25 \n");
   printf(" *    for a simplistic approach that allows all combinations).\n");
   printf(" *\n");
   printf(" * This table was computer generated - do not hand edit!!\n");
   printf(" */\n\n");

   printf("#define RELAY_TABLE_LENGTH (sizeof(relayTable) / sizeof(relayTable[0]))\n");
   printf("\n");
   printf("const uint16_t relayTable[][6] = {\n\n");
   printf("//     Relay bit pattern   turns       pad     ID\n");
}

void print_trailer() {
   printf("};\n");
}


int main(int argc, char **argv) 
{
   int d, i, r, c, w;

   print_header();

   // Blindly iterate over all combinations, testing if valid
   //    and only printing (and counting) the valid ones
   for (d=0; d<5; d++)
      for (i=0; i < 5; i++)
          for (r=0; r < 5; r++)
              for (c=0; c < 5; c++)
                  for (w=0; w < 5; w++)
                     if (valid_combination(d,i,r,c,w))
                        print_combination(d,i,r,c,w);
   
   print_trailer();
   return 0;
}


