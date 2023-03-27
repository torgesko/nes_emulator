#include<stdio.h>
#include<stdlib.h>
// Use threads?

#define ADDRESS_SPACE 65536

struct instruction {
    char n;
};

struct instruction instructions[256] = {
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'},
    {'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'}
};

struct flags {
    unsigned char N: 1; // Negative flag (1 if result most significant bit was set)
    unsigned char O: 1; // Overflow flag (1 if last arithmetic operation resulted in overflow)
    unsigned char U: 1; // Unused flag (always set to 1)
    unsigned char B: 1; // Break flag (1 if a software BRK instruction was executed)
    unsigned char D: 1; // Decimal flag (1 if arithmetic operations are performed in decimal mode)
    unsigned char I: 1; // Interrupt flag (1 for disable of interrupts)
    unsigned char Z: 1; // Zero flag (1 if the result of last arithmetic operation resulted in zero)
    unsigned char C: 1; // Carry flag (1 if there was a carry in the result of the most the recent operation)
};

struct cpu {
    unsigned char A; // Accumulator (uses all arithmetic and logical operations except for increments and decrements. The content of A can be stored  and retrieved either from memory or from the stack. Most complex operations will need to use the accumulator for arithmetic and efficient optimisation of its use is a key feature of time critical routines.)
    unsigned char X; // Index register (Typically holds counters or offsets for accessing memory. The value of the X register can be loaded and saved in memory, compared with values held in memory or incremented and decremented. The X register has one special function. It can be used to get a copy of the stack pointer or change its value.)
    unsigned char Y; // Index register (Same as X. It has no special functions) 
    unsigned short PC; // Program counter (points to the next instruction to be executed. PCL and PCH registers within 8 bit each)
    unsigned char SP; // Stack pointer (pushing bytes results in decrementing the stack pointer and vice versa. The CPU does not detect if the stack is overflowed by excessive pushing or pulling operations and will most likely result in the program crashing.)
    union {
        struct flags flag; // Bitmap
        char SR; // Status register (contains flags)
    };
};


/*
Basic architecture
The 6502 use little endian for byte order

The first 256 bytes (FF) page of memory is referred to as 'Zero Page' and is the focus of a number of special addressing modes that result in shorter (and quicker)
instructions or allow indirect access to memory. 

Second page ($0100 to $01FF) is reserved for the stack and cannot be relocated.

The only other reserved locations in the memory map are the very last 6 bytes of memory $FFFA to $FFFF which must be programmed 
with the addresses of the non-maskable interrupt handler ($FFFA/B), 
the power on reset location ($FFFC/D) and the BRK/interrupt request handler ($FFFE/F) respectively.
*/

void initialize_cpu(struct cpu* cpu_6502, char** address_space){
    if(cpu_6502 == NULL){
        exit(EXIT_FAILURE);
    }

    cpu_6502->SR = 0b00100100;
    cpu_6502->SP = 0x00; // This is an indirect address ($0100 to $01FF reserved for the stack, grows downward)

    // Important to also set the program counter

    *address_space = malloc(ADDRESS_SPACE);

    if(!address_space){
        exit(EXIT_FAILURE);
    } 
}



// Hvordan skal dette gjøres ?????????
void execute_instruction(struct cpu* cpu_6502, int index){

    char first_instruction;
    char cc = first_instruction & 0b00000011;
    
    swtich(cc){
        case 1:

            break;
        case 2:
        
            break;
        case 0:
        
            break;
        default:
            exit(EXIT_FAILURE);
    }
}


void main(int argc, char** argv){
    struct cpu cpu_6502;
    char* address_space;

    initialize_cpu(&cpu_6502, &address_space);

    exit(EXIT_SUCCESS);
}