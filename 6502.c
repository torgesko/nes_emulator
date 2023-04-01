#include<stdio.h>
#include<stdlib.h>

#define ADDRESS_SPACE 65536

// Byte count for each segment of instruction
const char instructions[256] = {
    1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 0, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    3, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 0, 3, 0, 0,
    2, 2, 2, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0
};

struct cpu {
    unsigned char A; // Accumulator (uses all arithmetic and logical operations except for increments and decrements. The content of A can be stored  and retrieved either from memory or from the stack. Most complex operations will need to use the accumulator for arithmetic and efficient optimisation of its use is a key feature of time critical routines.)
    unsigned char X; // Index register (Typically holds counters or offsets for accessing memory. The value of the X register can be loaded and saved in memory, compared with values held in memory or incremented and decremented. The X register has one special function. It can be used to get a copy of the stack pointer or change its value.)
    unsigned char Y; // Index register (Same as X. It has no special functions) 
    union{
        unsigned short PC;
        struct {
            unsigned char PCL;
            unsigned char PCH;
        };
    };
    ; // Program counter (points to the next instruction to be executed. PCL and PCH registers within 8 bit each)
    unsigned char SP; // Stack pointer (pushing bytes results in decrementing the stack pointer and vice versa. The CPU does not detect if the stack is overflowed by excessive pushing or pulling operations and will most likely result in the program crashing.)
    union {
        struct {
            unsigned char N: 1; // Negative flag (1 if result most significant bit was set)
            unsigned char O: 1; // Overflow flag (1 if last arithmetic operation resulted in overflow)
            unsigned char U: 1; // Unused flag (always set to 1)
            unsigned char B: 1; // Break flag (1 if a software BRK instruction was executed)
            unsigned char D: 1; // Decimal flag (1 if arithmetic operations are performed in decimal mode)
            unsigned char I: 1; // Interrupt flag (1 for disable of interrupts)
            unsigned char Z: 1; // Zero flag (1 if the result of last arithmetic operation resulted in zero)
            unsigned char C: 1; // Carry flag (1 if there was a carry in the result of the most the recent operation)
        };
        unsigned char SR; // Status register (contains flags)
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
        fprintf(stderr, "");
        exit(EXIT_FAILURE);
    }

    cpu_6502->SR = 0b00100100;
    cpu_6502->SP = 0x00; // This is an indirect address ($0100 to $01FF reserved for the stack, grows downward)

    // Important to also set the program counter

    *address_space = malloc(ADDRESS_SPACE);

    if(!address_space){
        fprintf(stderr, "");
        exit(EXIT_FAILURE);
    } 
}

// Instruction med char args (this one is going to be long lol)
void execute_instruction(struct cpu* cpu_6502, char* address_space, unsigned char instruction, char* args){
    if(cpu_6502 == NULL){
        fprintf(stderr, "");
        exit(EXIT_FAILURE);
    }

    switch(instruction){
        case 0x00: // BRK (implied)
            cpu_6502->PC++;
            
            break;
        case 0x01: // ORA (IND, X)
            break;
        case 0x05: // ORA (ZP)
            break;
        case 0x06: // ASL (ZP)
            break;
        case 0x08: // PHP (implied)
            break;
        case 0x09: // ORA (IMM)
            break;
        case 0x0A: // ASL (accum)
            break;
        case 0x0E: // ORA (abs)
            break; // End 1th row
        case 0x10: // BPL (relative)
            break;
        case 0x11: // ORA (IND, Y)
            break;
        case 0x15: // ORA (ZP, X)
            break;
        case 0x16: // ASL (ZP, X)
            break;
        case 0x18: // CLC (implied)
            cpu_6502->C = 0;
            cpu_6502->PC++;
            // Pause one clock cycle
            break;
        case 0x19: // ORA (ABS, Y)
            break;
        case 0x1D: // ORA (ABS, X)
            break;
        case 0x1E: // ASL (ABS, X)
            break; // End 2th row
        case 0x20: // JSR (abs)
            break;
        case 0x21: // AND ((IND), X)
            break;
        case 0x24: // BIT (ZP)
            break;
        case 0x25: // AND (ZP)
            break;
        case 0x26: // ROL (ZP)
            break;
        case 0x28: // PLP (implied)
            break;
        case 0x29: // AND (IMM)
            break;
        case 0x2A: // ROL (accum)
            break;
        case 0x2C: // BIT (abs)
            break;
        case 0x2D: // AND (abs)
            break;
        case 0x2E: // ROL (abs)
            break; // End 3th row
        case 0x30: // BMI (relative)
            break;
        case 0x31: // AND ((IND), Y)
            break;
        case 0x35: // AND (ZP, X)
            break;
        case 0x36: // ROL (ZP, X)
            break;
        case 0x38: // SEC (implied)
            break;
        case 0x39: // AND (ABS, Y)
            break;
        case 0x3D: // AND (ABS, X)
            break;
        case 0x3E: // ROL (ABS, X)
            break; // End 4th row
        case 0x40: //ERI (implied)
            break;
        case 0x41: // EOR (IND, X)
            break;
        case 0x45: // EOR (ZP)
            break;
        case 0x46: // LSR (ZP)
            break;
        case 0x48: // PHA (implied)
            break;
        case 0x49: // EOR (IMM)
            break;
        case 0x4A: // LSR (accum)
            break;
        case 0x4C: // JMP (ABS)
            break;
        case 0x4D: // EOR (ABS)
            break;
        case 0x4E: // LSR (ABS)
            break; // End 5th row
        case 0x50: // BVC (relative)
            break;
        case 0x51: // EOR ((IND), Y)
            break;
        case 0x55: // EOR (ZP, X)
            break;
        case 0x56: // LSR (ZP, X)
            break;
        case 0x58: // CLI (implied)
            break;
        case 0x59: // EOR (ABS, Y)
            break;
        case 0x5D: // EOR (ABS, X)
            break;
        case 0x5E: // LSR (ABS, X)
            break; // End 6th row
        case 0x60: // RTS (implied)
            break;
        case 0x61: // ADC (IND, X)
            break;
        case 0x65: // ADC (ZP)
            break;
        case 0x66: // ROR (ZP)
            break;
        case 0x68: // PLA (implied)
            break;
        case 0x69: // ADC (imm)
            break;
        case 0x6A: // ROR (accum)
            break;
        case 0x6C: // JMP (indirect)
            break;
        case 0x6D: // ADC (ABS)
            break;
        case 0x6E: // ROR (ABS)
            break; // End 7th row
        case 0x70: // BVS (relative)
            break;
        case 0x71: // ADC (IND, X)
            break;
        case 0x75: // ADC (ZP, X)
            break;
        case 0x76: // ROR (ZP, X)
            break;
        case 0x78: // SEI (implied)
            break;
        case 0x79: // ADC (ABS, Y)
            break;
        case 0x7D: // ADC (ABS, X)
            break;
        case 0x7E: // ROR (ABS, X)
            break; // End 8th row
        case 0x81: // STA (IND, X)
            break;
        case 0x84: // STY (ZP)
            break;
        case 0x85: // STA (ZP)
            break;
        case 0x86: // STX (ZP)
            break;
        case 0x88: // Dey (implied)
            break;
        case 0x8A: // TXA (implied)
            break;
        case 0x8C: // STY (ABS)
            break;
        case 0x8D: // STA (ABS)
            break;
        case 0x8E: // STX (ABS)
            break; // End 9th row
        case 0x90: // BBC (relative)
            break;
        case 0x91: // STA ((IND), Y)
            break;
        case 0x94: // STY (ZP, X)
            break;
        case 0x95: // STA (ZP)
            break;
        case 0x96: // STX (ZP)
            break;
        case 0x98: // TYA (implied)
            break;
        case 0x99: // STA (ABS, Y)
            break;
        case 0x9A: // TXS (implied)
            break;
        case 0x9D: // STA (ABS, X)
            break; // End 10th row
        case 0xA0: // LDY (IMM)
            break;
        case 0xA1: // LDA (IND, X)
            break;
        case 0xA2: // LDX (IMM)
            break;
        case 0xA4: // LDY (ZP)
            break;
        case 0xA5: // LDA (ZP)
            break;
        case 0xA6: // LDX (ZP)
            break;
        case 0xA8: // TAY (implied)
            break;
        case 0xA9: // LDA (IMM)
            break;
        case 0xAA: // TAX (implied)
            break;
        case 0xAC: // LDY (ABS)
            break;
        case 0xAD: // LDA (ABS)
            break;
        case 0xAE: // LDX (ABS)
            break; // End 11th row
        case 0xB0: // BCS (relative)
            break;
        case 0xB1: // LDA ((IND), Y)
            break;
        case 0xB4: // LDY (ZP, X) 
            break;
        case 0xB5: // LDA (ZP, X)
            break;
        case 0xB6: // LDX (ZP, Y)
            break;
        case 0xB8: // CLV (implied)
            break;
        case 0xB9: // LDA (ABS, Y)
            break;
        case 0xBA: // TSX (implied)
            break;
        case 0xBC: // LDY (ABS, X)
            break;
        case 0xBD: // LDA (ABS, X)
            break;
        case 0xBE: // LDA (ABS, Y)
            break; // End 12th row
        case 0xC0: // CPY (IMM) 
            break;
        case 0xC1: // CMP ((IND), Y)
            break;
        case 0xC4: // CPY (ZP)
            break;
        case 0xC5: // CMP (ZP)
            break;
        case 0xC6: // DEC (ZP)
            break;
        case 0xC8: // INY (implied)
            break;
        case 0xC9: // CMP (IMM)
            break;
        case 0xCA: // DEX (implied)
            break;
        case 0xCC: // CPY (ABS)
            break;
        case 0xCD: // CMP (ABS)
            break;
        case 0xCE: // DEC (ABS)
            break; // End 13th row
        case 0xD0: // BNE (relative)
            break;
        case 0xD1: // CMP ((IND), Y)
            break;
        case 0xD5: // CMP (ZP, X)
            break;
        case 0xD6: // DEC (ZP, X)
            break;
        case 0xD8: // CLD (implied)
            break;
        case 0xD9: // CMP (ABS, Y)
            break;
        case 0xDD: // CMP (ABS, X)
            break;
        case 0xDE: // CMP (ABS, X)
            break; // End 14th row
        case 0xE0: // CPX (IMM)
            break;
        case 0xE1: // SBC (IND, X)
            break;
        case 0xE4: // CPX (ZP)
            break;
        case 0xE5: // SBC (ZP)
            break;
        case 0xE6: // INC (ZP)
            break;
        case 0xE8: // INX (implied)
            break;
        case 0xE9: // SBC (IMM)
            break;
        case 0xEA: // NOP (implied)
            break;
        case 0xEC: // CPX (ABS)
            break;
        case 0xED: // SBC (ABS)
            break;
        case 0xEE: // INC (ABS)
            break; // End 15th row
        case 0xF0: // BEQ (relative)
            break;
        case 0xF1: // SBC ((IND), Y)
            break;
        case 0xF5: // SBC (ZP, X)
            break;
        case 0xF6: // INC (ZP, X)
            break;
        case 0xF8: // SED (implied)
            break;
        case 0xF9: // SBC (ABS, Y)
            break;
        case 0xFD: // SBC (ABS, X)
            break;
        case 0xFE: // INC (ABS, X)
            break;

        default:
            break
    }
}