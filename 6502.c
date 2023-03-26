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


/*
The first 256 bytes (FF) page of memory is referred to as 'Zero Page' and is the focus of a number of special addresing modes that result in shorter (and quicker)
instructions or allow indirect access to memory. Second page ($0100 to $01FF) is reserved for the stack and cannot be relocated.
*/

void initialize_cpu(struct cpu* cpu_6502){
    cpu_6502->SR = 0b00100100;
    cpu_6502->SP = 0x00; // This is an indirect address ($0100 to $01FF reserved for the stack)
}

// Will need big switch case to execute instructions
void execute_instruction(struct cpu* cpu_6502){

}

void main(int argc, char** argv){
    struct cpu cpu_6502;
    initialize_cpu(&cpu_6502);

    if(cpu_6502.flag.U == 1){ // Means that the bytes use big endian!
        printf("Yes!\n");
    }

    exit(EXIT_SUCCESS);
}