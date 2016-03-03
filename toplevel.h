#ifndef __TOPLEVEL_H_
#define __TOPLEVEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <ap_int.h>
#include <hls_stream.h>

// typedefs
typedef ap_uint<32> uint32;
typedef ap_int<32> int32;
typedef ap_uint<100> uint100;

// function prototypes
void toplevel(hls::stream<uint32> &input, hls::stream<uint32> &output);
void add(uint100 cXX);
uint100 step(uint100 c11);

#endif
