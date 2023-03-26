#include<stdio.h>
#include<stdlib.h>

struct flags{
    unsigned char N: 1; // Negative flag (1 if result most significant bit was set)
    unsigned char O: 1; // Overflow flag (1 if last arithmetic operation resulted in overflow)
    unsigned char U: 1; // Unused flag (always set to 1)
    unsigned char B: 1; // Break flag (1 if a software BRK instruction was executed)
    unsigned char D: 1; // Decimal flag (1 if arithmetic operations are performed in decimal mode)
    unsigned char I: 1; // Interrupt flag (1 for disable of interrupts)
    unsigned char Z: 1; // Zero flag (1 if the result of last arithmetic operation resulted in zero)
    unsigned char C: 1; // Carry flag (1 if there was a carry in the result of the most the recent operation)
};

struct cpu{
    unsigned char A; // Accumulator
    unsigned char X; // Index register
    unsigned char Y; // Index register
    unsigned short PC; // Program counter
    unsigned char SP; // Stack pointer
    union {
        struct flags flag; // Bitmap
        char SR; // Status register (contains flags)
    };
};

void initialize_cpu(struct cpu* cpu_6502){
    cpu_6502->SR = 0b00100100;
}

// Will need big switch case to execute instructions
void execute_instruction(struct cpu* cpu_6502){

}

void main(int argc, char** argv){
    struct cpu cpu_6502;
    initialize_cpu(&cpu_6502);

    exit(EXIT_SUCCESS);
}