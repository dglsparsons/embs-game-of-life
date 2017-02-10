#include "toplevel.h"

//Prototype
void print_bits(unsigned value, unsigned bits);

//Counter for printing grid
int new_line = 0;

int main() {
    hls::stream<uint32> to_hw, from_hw;

    //This is the input board, represented as 100 bits (10 x 10 board)
    uint32 input_state1 = 0b0000;
    uint32 input_state2 = 0b00000000000000000000000000000000;
    uint32 input_state3 = 0b00000000000001000000000100000000;
    uint32 input_state4 = 0b01000000000000000000000000000000;

    // Send the input board to the hardware
    to_hw.write(input_state1);
    to_hw.write(input_state2);
    to_hw.write(input_state3);
    to_hw.write(input_state4);

    // Print out the input board in a readable format.
    printf("\rInput:\n");
    print_bits(input_state1,4);
    print_bits(input_state2,32);
    print_bits(input_state3,32);
    print_bits(input_state4,32);

    //Run the hardware
    toplevel(to_hw, from_hw);

    // Read the output from the hardware
    uint32 output_state1 = from_hw.read();
    uint32 output_state2 = from_hw.read();
    uint32 output_state3 = from_hw.read();
    uint32 output_state4 = from_hw.read();

    //Print the returned board in a readable format. 
    printf("\rOutput:\n");
    print_bits(output_state1,4);
    print_bits(output_state2,32);
    print_bits(output_state3,32);
    print_bits(output_state4,32);
}

//Function for printing integer 'value'
//only prints 'bits' number of bits, starting from LSB
void print_bits(unsigned value, unsigned bits) {
    unsigned mask = 1 << (bits-1);
    while (mask)
    {
        printf("%d", (mask & value) != 0);
        if (new_line == 9) {
            printf("\n");
            new_line = 0;
        } else {
            new_line++;
        }
        mask >>= 1;
    }
}
