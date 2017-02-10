#include "toplevel.h"

//Input data storage
uint100 state, bit1, bit2, bit3;

//Define bit masks for selecting parts of the array.
#define LEFT ((uint100)0x8020080200802008020080200)
#define RIGHT ((uint100)0x0040100401004010040100401)
#define TOP ((uint100)0xFFC0000000000000000000000)
#define BOTTOM ((uint100)0x3FF)
#define NOT_LEFT ((uint100)~0x8020080200802008020080200)
#define NOT_RIGHT ((uint100)~0x0040100401004010040100401)

//Top-level function - this is called to process the hardware.
void toplevel(hls::stream<uint32> &input, hls::stream<uint32> &output) {
#pragma HLS RESOURCE variable=input core=AXI4Stream
#pragma HLS RESOURCE variable=output core=AXI4Stream
#pragma HLS INTERFACE ap_ctrl_none port=return

    //Read in state, and shift to correct position within a 100 bit int. 
    state = ((uint100)input.read()) << 96;
    state |= ((uint100)input.read()) << 64;
    state |= ((uint100)input.read()) << 32;
    state |= (uint100)input.read();

    //Calculate next generation of the board. 
    state = step(state);

    //Output new state
    output.write((uint32)(state >> 96) & 0xF);
    output.write((uint32)(state >> 64) & 0xFFFFFFFF);
    output.write((uint32)(state >> 32) & 0xFFFFFFFF);
    output.write((uint32)state & 0xFFFFFFFF);
}

// add is used to calculate the number of neighbours each cell has. This is
// done by accumulating the values into 3 x 100 bit integers, bit1, bit2 and
// bit3.  
// Here, bit 1 is the smallest, and bit 3 the largest. i.e 
// bit 3 | bit 2 | bit 1
// ---------------------
// 1     |   0   |   0   for a cell represents 4 neighbours. 
// 1     |   0   |   1   represents 5 neighbours. 
void add(uint100 cXX) {
    uint100 carry1 = bit1 & cXX;
    uint100 carry2 = bit2 & carry1;

    bit1 ^= cXX;
    bit2 ^= carry1;
    bit3 |= carry2;
}

// step calculates the next generation of the games state, and returns it as 
// a 100 bit integer.
uint100 step(uint100 c11) {
    // Shift neighbours into position, with wrap-around
    // Each of these numbers represents a cells neighbour. As this is done
    // using bitshifts, the entire boards neighbours are calculated at once. 
    uint100 c10 = (c11 >> 10 | ((c11 & BOTTOM) << 90));
    uint100 c12 = (c11 << 10 | ((c11 & TOP) >> 90));
    uint100 c00 = ((c10 & NOT_RIGHT) >> 1 | ((c10 & RIGHT) << 9));
    uint100 c01 = ((c11 & NOT_RIGHT) >> 1 | ((c11 & RIGHT) << 9));
    uint100 c02 = ((c12 & NOT_RIGHT) >> 1 | ((c12 & RIGHT) << 9));
    uint100 c20 = ((c10 & NOT_LEFT) << 1 | ((c10 & LEFT) >> 9));
    uint100 c21 = ((c11 & NOT_LEFT) << 1 | ((c11 & LEFT) >> 9));
    uint100 c22 = ((c12 & NOT_LEFT) << 1 | ((c12 & LEFT) >> 9));

    // Reset the bit registers that count the number of neighbours.
    bit1 = 0;
    bit2 = 0;
    bit3 = 0;

    // Accumulate live neighbour counts
    add(c00);
    add(c01);
    add(c02);
    add(c10);
    add(c12);
    add(c20);
    add(c21);
    add(c22);

    //  Return live cases
    return ((c11 | bit1) & bit2 & ~bit3);
}
