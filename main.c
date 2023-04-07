#include<stdlib.h>
#include<pthread.h>
#include "cpu.h"
#include "bus.h"

void main(int argc, char** argv){
    struct bus BUS;
    struct cpu* CPU = &BUS.CPU;
    
    connect_bus(CPU, &BUS);
    reset(CPU);
    reset_bus(&BUS);

    // Load some program into the ram to test and then run it.

    // Create an extra thread (or more?) to populate the interrupt pools in the CPU. Need some sort of mutex for the interrupt pools. 



    while(true){
        execute_instruction(CPU);
    }

    exit(EXIT_SUCCESS);
}