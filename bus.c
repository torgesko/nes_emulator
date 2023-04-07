#include "bus.h"

// Write functions that write to and read from the bus 

char write(struct bus* BUS, unsigned short addr, unsigned char data){
    if(addr >= 0x0000 && addr <= 0xFFFF){
        BUS->ram[addr] = data;
        return 0;
    }

    return -1;
}

unsigned char read(struct bus* BUS, unsigned short addr, unsigned char bReadOnly){
    if(addr >= 0x0000 && addr <= 0xFFFF){
        return BUS->ram[addr];
    }

    return 0x00;
}
// Just in case
void reset_bus(struct bus* BUS){
    for(int i = 0; i < BUS_S; i++){
        BUS->ram[i] = 0x00;
    }
}