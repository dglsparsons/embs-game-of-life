# EMBS Game Of Life
This is a solution to Conway's game of life for a 10 x 10 board using a Xilinx FPGA. 

# The solution is split into two files. 

toplevel.cpp defines a description of the hardware, and contains the logic for calculating the 
games next state. 

testbench.cpp shows how this logic may be used by a program on the FPGA. 
