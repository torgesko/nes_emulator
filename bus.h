#ifndef BUS_H
#include "cpu.h"
#define BUS_H
#define BUS_S 65536

struct bus{
    struct cpu CPU;
    char ram[BUS_S];
};

char write(struct bus* BUS, unsigned short addr, unsigned char data);
unsigned char read(struct bus* BUS, unsigned short addr, unsigned char bReadOnly);
void reset_bus(struct bus* BUS);

#endif