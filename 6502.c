#include<stdio.h>
#include<stdlib.h>

#define ADDRESS_SPACE_SIZE 65536 // 16 bit program counter
#define CYCLE_TIME 559 // Nanoseconds

struct cpu {
    unsigned char A; // Accumulator (uses all arithmetic and logical operations except for increments and decrements. The content of A can be stored  and retrieved either from memory or from the stack. Most complex operations will need to use the accumulator for arithmetic and efficient optimisation of its use is a key feature of time critical routines.)
    unsigned char X; // Index register (Typically holds counters or offsets for accessing memory. The value of the X register can be loaded and saved in memory, compared with values held in memory or incremented and decremented. The X register has one special function. It can be used to get a copy of the stack pointer or change its value.)
    unsigned char Y; // Index register (Same as X. It has no special functions) 
    union{
        unsigned char PC;
        struct {
            unsigned char PCL;
            unsigned char PCH;
        };
    };
     // Program counter (points to the next instruction to be executed. PCL and PCH registers within 8 bit each)
    unsigned short SP; // Stack pointer (pushing bytes results in decrementing the stack pointer and vice versa. The CPU does not detect if the stack is overflowed by excessive pushing or pulling operations and will most likely result in the program crashing.)
    union {
        struct {
            unsigned char C: 1; // Carry flag (1 if there was a carry in the result of the most the recent operation)
            unsigned char Z: 1; // Zero flag (1 if the result of last arithmetic operation resulted in zero)
            unsigned char I: 1; // Interrupt flag (1 for disable of interrupts)
            unsigned char D: 1; // Decimal flag (1 if arithmetic operations are performed in decimal mode)
            unsigned char B: 1; // Break flag (1 if a software BRK instruction was executed)
            unsigned char U: 1; // Unused flag (always set to 1)
            unsigned char O: 1; // Overflow flag (1 if last arithmetic operation resulted in overflow)
            unsigned char N: 1; // Negative flag (1 if result most significant bit was set)
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

// Should I use reset instead??????
void initialize_cpu(struct cpu* cpu_6502, unsigned char** address_space){
    cpu_6502->SR = 0b00100000; // LOOK MORE CLOSELY AT THIS LATER, MAY NOT BE RIGHT!
    cpu_6502->SP = 0x01FF; // This is an indirect address ($0100 to $01FF reserved for the stack, grows downward)

    *address_space = malloc(ADDRESS_SPACE_SIZE);

    if(!address_space){
        fprintf(stderr, "Allocation of memory failed for emulation");
        exit(EXIT_FAILURE);
    } 
}

/* Interrupts */
void IRQ(){

}

void NMI(){

}

void reset(){

}


/* FUNCTION FOR CLOCK */


/* THESE ARE THE ADDRESSING MODES FOR THE 6502 CPU */
unsigned char IMM(unsigned char* address_space, unsigned short PC){
    unsigned short address = PC + 1; // Optimize with compiler

    return address_space[address];
}

unsigned char ABS(unsigned char* address_space, unsigned short PC){
    unsigned short address = *(unsigned short*)(address_space + PC + 1);

    return address_space[address];
}

unsigned char ABSI(unsigned char* address_space, unsigned short PC, unsigned char I){
    unsigned short address = *(unsigned short*)(address_space + PC + 1) + I;

    return address_space[address];
}

unsigned char ABSIND(unsigned char* address_space , unsigned short PC){
    unsigned short address = *(unsigned short*)(address_space + PC + 1);
    address = *(unsigned short*)(address_space + address);

    return address_space[address];
}

unsigned char IND(unsigned char* address_space, unsigned short PC){
    unsigned char zp_addr = address_space[PC];
    
    unsigned short address = *(unsigned short*) (address_space + zp_addr);

    return address_space[address];
}

unsigned char INDX(unsigned char* address_space, unsigned short PC, unsigned char X){
    unsigned char arg = address_space[PC + 1];
    unsigned short address = *(address_space + (arg + X) % 256) + *(address_space + (arg + X + 1) % 256) << 8;

    return address_space[address];
}

unsigned char INDY(unsigned char* address_space, unsigned short PC, unsigned char Y){
    unsigned char arg = address_space[PC + 1];
    unsigned short address = address_space[arg] + Y + *(address_space + (arg + 1) % 256) << 8;

    return address_space[address];
}

unsigned char ZP0(unsigned char* address_space, unsigned short PC){
    unsigned short address = address_space[PC + 1];

    return address_space[address];
}

unsigned char ZPI(unsigned char* address_space, unsigned short PC, unsigned char I){
    unsigned short address = (I + address_space[PC + 1]) % 256;

    return address_space[address];
}

// Instruction med char args (this one is going to be long lol)
void execute_instruction(struct cpu* cpu_6502, unsigned char* address_space, unsigned char instruction, char* args){
    switch(instruction){
        case 0x00: // BRK (implied)
            cpu_6502->PC += 2;

            cpu_6502->B = 1; // I need to think more about this later 

            // Saving the program counter after interrupt handler
            address_space[0x0100 + cpu_6502->SP] = cpu_6502->PCL; 
            cpu_6502->SP--;

            address_space[0x0100 + cpu_6502->SP] = cpu_6502->PCH;
            cpu_6502->SP--;

            address_space[0x0100 + cpu_6502->SP] = cpu_6502->SR; // Save status register
            cpu_6502->SP--;

            cpu_6502->I = 1; // Setting interrupt flag

            // This needs to be checked l8ter
            cpu_6502->PCH = address_space[0xFFFF]; // Load the PCH
            cpu_6502->PCL = address_space[0xFFFE]; // Load the PCL
            break;
        case 0x01: // ORA (IND, X)
            cpu_6502->PC += 2;
            unsigned short indirect_addr = cpu_6502->X + address_space[cpu_6502->PC - 2];
            // Set the overflow flag somehow if resulting operation overflow
            if(indirect_addr > 0x00FF){ // If there is an overflow, set the overflow flag (research if this is a correct understanding, ChatGPT is not trustworthy)
                cpu_6502->O = 1; // Set the overflow flag, 
                indirect_addr %= 0x0100;
            }

            unsigned short direct_address = *(unsigned short*)(address_space + indirect_addr);
            cpu_6502->A = cpu_6502->A | address_space[direct_address];
            
            break;
        case 0x05: // ORA (ZP)
            unsigned char zp_value = ZP0(address_space, cpu_6502->PC);
            cpu_6502->PC += 2;
            cpu_6502->A = cpu_6502->A | zp_value;
            break;
        case 0x06: // ASL (ZP)
            break;
        case 0x08: // PHP (implied)
            cpu_6502->PC++;
            cpu_6502->SP--;
            address_space[0x0100 + cpu_6502->SP] = cpu_6502->SR;
            break;
        case 0x09: // ORA (IMM)
            cpu_6502->PC += 2;
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
            cpu_6502->PC++;
            cpu_6502->SP++;
            cpu_6502->SR = address_space[0x0100 + cpu_6502->SP];
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
            cpu_6502->C = 1;
            cpu_6502->PC++;
            break;
        case 0x39: // AND (ABS, Y)
            break;
        case 0x3D: // AND (ABS, X)
            break;
        case 0x3E: // ROL (ABS, X)
            break; // End 4th row
        case 0x40: // RTI (implied)
            cpu_6502->SP++; // Increment stack pointer
            cpu_6502->SR = address_space[0x0100 + cpu_6502->SP]; // Restore status register (SR)
            cpu_6502->SP++; // Increment stack pointer
            cpu_6502->PCH = address_space[0x0100 + cpu_6502->SP]; // Load PCLH with next instruction higher byte
            cpu_6502->SP++; // Increment stack pointer
            cpu_6502->PCL = address_space[0x0100 + cpu_6502->SP]; // Load PCL with next instruction lower byte
            cpu_6502->B = 0;
            break;
        case 0x41: // EOR (IND, X)
            break;
        case 0x45: // EOR (ZP)
            break;
        case 0x46: // LSR (ZP)
            break;
        case 0x48: // PHA (implied)
            address_space[0x0100 + cpu_6502->SP] = cpu_6502->A;
            cpu_6502->SP--;
            cpu_6502->PC++;
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
            cpu_6502->PC++;
            cpu_6502->I = 0;
            break;
        case 0x59: // EOR (ABS, Y)
            break;
        case 0x5D: // EOR (ABS, X)
            break;
        case 0x5E: // LSR (ABS, X)
            break; // End 6th row
        case 0x60: // RTS (implied)
            cpu_6502->SP++;
            cpu_6502->PCL = address_space[0x0100 + cpu_6502->SP];
            cpu_6502->SP++;
            cpu_6502->PCH = address_space[0x0100 + cpu_6502->SP];
            cpu_6502->PC++;
            break;
        case 0x61: // ADC (IND, X)
            break;
        case 0x65: // ADC (ZP)
            break;
        case 0x66: // ROR (ZP)
            break;
        case 0x68: // PLA (implied)
            cpu_6502->SP++;
            cpu_6502->A = address_space[cpu_6502->SP];

            if(cpu_6502->A & 0b10000000){
                cpu_6502->N = 1;
            } else {
                cpu_6502->N = 0;

                if(cpu_6502 == 0    ){
                    cpu_6502->Z = 0;
                }
            }

            cpu_6502->PC++;

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
            cpu_6502->I = 1;
            cpu_6502->PC++;
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
            cpu_6502->PC++;
            cpu_6502->Y--;
            break;
        case 0x8A: // TXA (implied)
            cpu_6502->A = cpu_6502->X;
            cpu_6502->PC++;
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
            cpu_6502->A = cpu_6502->Y;
            cpu_6502->PC++;
            break;
        case 0x99: // STA (ABS, Y)
            break;
        case 0x9A: // TXS (implied)
            cpu_6502->SP = cpu_6502->X;
            cpu_6502->PC++;
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
            // Does these instructions set the flags?
            cpu_6502->Y = cpu_6502->A;
            cpu_6502->PC++;
            break;
        case 0xA9: // LDA (IMM)
            break;
        case 0xAA: // TAX (implied)
            cpu_6502->X = cpu_6502->A;
            cpu_6502->PC++;
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
            cpu_6502->PC++; // ????
            cpu_6502->O = 0;
            // Pause cycles (????)
            break;
        case 0xB9: // LDA (ABS, Y)
            break;
        case 0xBA: // TSX (implied)
            cpu_6502->X = cpu_6502->SP;
            cpu_6502->PC++;
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
            cpu_6502->Y++;
            cpu_6502->PC++;
            break;
        case 0xC9: // CMP (IMM)
            break;
        case 0xCA: // DEX (implied)
            cpu_6502->X--;
            cpu_6502->PC++;
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
            cpu_6502->D = 0;
            cpu_6502->PC++;
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
            cpu_6502->X++;
            cpu_6502->PC++;
            break;
        case 0xE9: // SBC (IMM)
            break;
        case 0xEA: // NOP (implied)
            cpu_6502->PC++;
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
        case 0xF8: // SED (implied) Not sure if this is even going to have any effect, I may implement this functionality later
            cpu_6502->D = 1;
            break;
        case 0xF9: // SBC (ABS, Y)
            break;
        case 0xFD: // SBC (ABS, X)
            break;
        case 0xFE: // INC (ABS, X)
            break;

        default:
            // Pause one cycle?
            break

        // There will be added more unofficial instructions
    }
}