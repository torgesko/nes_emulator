#include "cpu.h"
#include "bus.h"
#include "instructions.h"

char writeb(struct cpu* CPU, unsigned short addr, unsigned char data){
    return write(&CPU->BUS, addr, data);
}

unsigned char readb(struct cpu* CPU, unsigned short addr, unsigned char bReadOnly){
    return read(&CPU->BUS, addr, bReadOnly);
}

void connect_bus(struct cpu* CPU, struct bus* BUS){
    CPU->BUS = BUS;
}

void reset(struct cpu* CPU){
    CPU->A = 0;
    CPU->X = 0;
    CPU->Y = 0;
    CPU->SP = 0xFF;
    CPU->SR = 0b00100100;

    CPU->PCL = readb(CPU, 0xFFFC, 0);
    CPU->PCH = readb(CPU, 0xFFFD, 0);

    // This takes will take time.
}

void IRQ(struct cpu* CPU){
    // Check the pool of IRQ interrupts
    
    if(CPU->I == 0 && CPU->NMIFLAG == 0){
        writeb(CPU, 0x0100 + CPU->SP, CPU->PCH);
        CPU->SP--;
        writeb(CPU, 0x0100 + CPU->SP, CPU->PCL);
        CPU->SP--;

        CPU->SR |= 0b00100100;
        writeb(CPU, 0x0100 + CPU->SP, CPU->SR);
        CPU->SP--; 

        CPU->PCL = readb(CPU, 0xFFFE, 0);
        CPU->PCH = readb(CPU, 0xFFFF, 0);

        // This takes will take time
    } else {
        CPU->NMIFLAG = 0;
    }
}

void NMI(struct cpu* CPU){
    // Check the pool of NMI interrupts
    if(CPU->NMIFLAG == 0){
        CPU->NMIFLAG = 1;

        writeb(CPU, 0x0100 + CPU->SP, CPU->PCH);
        CPU->SP--;
        writeb(CPU, 0x0100 + CPU->SP, CPU->PCL);
        CPU->SP--;
    
        CPU->SR |= 0b00010100;

        writeb(CPU, 0x0100 + CPU->SP, CPU->SR);
        CPU->SP--;

        CPU->PCL = readb(CPU, 0xFFFA, 0);
        CPU->PCH = readb(CPU, 0xFFFB, 0);
    }
}

void execute_instruction(struct cpu* CPU){
    unsigned char instruction = CPU->BUS->ram[CPU->PC];

    switch(instruction){
        case 0x00: // BRK (implied)
            I00(CPU);
            break;
        case 0x01: // ORA (IND, X)
            I01(CPU);
            break;
        case 0x05: // ORA (ZP)
            I05(CPU);
            break;
        case 0x06: // ASL (ZP)
            break;
        case 0x08: // PHP (implied)
            I08(CPU);
            break;
        case 0x09: // ORA (IMM)
            I09(CPU);
            break;
        case 0x0A: // ASL (accum)
            I0A(CPU);
            break;
        case 0x0E: // ORA (abs)
            I0E(CPU);
            break; // End 1th row
        case 0x10: // BPL (relative)
            I10(CPU);
            break;
        case 0x11: // ORA (IND, Y)
            I11(CPU);
            break;
        case 0x15: // ORA (ZP, X)
            I15(CPU);
            break;
        case 0x16: // ASL (ZP, X)
            I16(CPU);
            break;
        case 0x18: // CLC (implied)
            I18(CPU);
            break;
        case 0x19: // ORA (ABS, Y)
            I19(CPU);
            break;
        case 0x1D: // ORA (ABS, X)
            I1D(CPU);
            break;
        case 0x1E: // ASL (ABS, X)
            I1E(CPU);
            break; // End 2th row
        case 0x20: // JSR (abs)
            I20(CPU);
            break;
        case 0x21: // AND ((IND), X)
            I21(CPU);
            break;
        case 0x24: // BIT (ZP)
            I24(CPU);
            break;
        case 0x25: // AND (ZP)
            I25(CPU);
            break;
        case 0x26: // ROL (ZP)
            I26(CPU);
            break;
        case 0x28: // PLP (implied)
            I28(CPU);
            break;
        case 0x29: // AND (IMM)
            I29(CPU);
            break;
        case 0x2A: // ROL (accum)
            I2A(CPU);
            break;
        case 0x2C: // BIT (abs)
            I2C(CPU);
            break;
        case 0x2D: // AND (abs)
            I2D(CPU);
            break;
        case 0x2E: // ROL (abs)
            I2E(CPU);
            break; // End 3th row
        case 0x30: // BMI (relative)
            I30(CPU);
            break;
        case 0x31: // AND ((IND), Y)
            I31(CPU);
            break;
        case 0x35: // AND (ZP, X)
            I35(CPU);
            break;
        case 0x36: // ROL (ZP, X)
            I36(CPU);
            break;
        case 0x38: // SEC (implied)
            I38(CPU);
            break;
        case 0x39: // AND (ABS, Y)
            I39(CPU);
            break;
        case 0x3D: // AND (ABS, X)
            I3D(CPU);
            break;
        case 0x3E: // ROL (ABS, X)
            I3E(CPU);
            break; // End 4th row
        case 0x40: // RTI (implied)
            I40(CPU);
            break;
        case 0x41: // EOR (IND, X)
            I41(CPU);
            break;
        case 0x45: // EOR (ZP)
            I45(CPU);
            break;
        case 0x46: // LSR (ZP)
            I46(CPU);
            break;
        case 0x48: // PHA (implied)
            I48(CPU);
            break;
        case 0x49: // EOR (IMM)
            I49(CPU);
            break;
        case 0x4A: // LSR (accum)
            I4A(CPU);
            break;
        case 0x4C: // JMP (ABS)
            I4C(CPU);
            break;
        case 0x4D: // EOR (ABS)
            I4D(CPU);
            break;
        case 0x4E: // LSR (ABS)
            I4E(CPU);
            break; // End 5th row
        case 0x50: // BVC (relative)
            I50(CPU);
            break;
        case 0x51: // EOR ((IND), Y)
            I51(CPU);
            break;
        case 0x55: // EOR (ZP, X)
            I55(CPU);
            break;
        case 0x56: // LSR (ZP, X)
            I56(CPU);
            break;
        case 0x58: // CLI (implied)
            I58(CPU);
            break;
        case 0x59: // EOR (ABS, Y)
            I59(CPU);
            break;
        case 0x5D: // EOR (ABS, X)
            I5D(CPU);
            break;
        case 0x5E: // LSR (ABS, X)
            I5E(CPU);
            break; // End 6th row
        case 0x60: // RTS (implied)
            I60(CPU);
            break;
        case 0x61: // ADC (IND, X)
            I61(CPU);
            break;
        case 0x65: // ADC (ZP)
            I65(CPU);
            break;
        case 0x66: // ROR (ZP)
            I66(CPU);
            break;
        case 0x68: // PLA (implied)
            I68(CPU);
            break;
        case 0x69: // ADC (imm)
            I69(CPU);
            break;
        case 0x6A: // ROR (accum)
            I6A(CPU);
            break;
        case 0x6C: // JMP (indirect)
            I6C(CPU);
            break;
        case 0x6D: // ADC (ABS)
            I6D(CPU);
            break;
        case 0x6E: // ROR (ABS)
            I6E(CPU);
            break; // End 7th row
        case 0x70: // BVS (relative)
            I70(CPU);
            break;
        case 0x71: // ADC (IND, X)
            I71(CPU);
            break;
        case 0x75: // ADC (ZP, X)
            I75(CPU);
            break;
        case 0x76: // ROR (ZP, X)
            I76(CPU);
            break;
        case 0x78: // SEI (implied)
            I78(CPU);
            break;
        case 0x79: // ADC (ABS, Y)
            I79(CPU);
            break;
        case 0x7D: // ADC (ABS, X)
            I7D(CPU);
            break;
        case 0x7E: // ROR (ABS, X)
            I7E(CPU);
            break; // End 8th row
        case 0x81: // STA (IND, X)
            I81(CPU);
            break;
        case 0x84: // STY (ZP)
            I84(CPU);
            break;
        case 0x85: // STA (ZP)
            I85(CPU);
            break;
        case 0x86: // STX (ZP)
            I86(CPU);
            break;
        case 0x88: // Dey (implied)
            I88(CPU);
            break;
        case 0x8A: // TXA (implied)
            I8A(CPU);
            break;
        case 0x8C: // STY (ABS)
            I8C(CPU);
            break;
        case 0x8D: // STA (ABS)
            I8D(CPU);
            break;
        case 0x8E: // STX (ABS)
            I8E(CPU);
            break; // End 9th row
        case 0x90: // BCC (relative)
            I90(CPU);
            break;
        case 0x91: // STA ((IND), Y)
            I91(CPU);
            break;
        case 0x94: // STY (ZP, X)
            I94(CPU);
            break;
        case 0x95: // STA (ZP)
            I95(CPU);
            break;
        case 0x96: // STX (ZP)
            I96(CPU);
            break;
        case 0x98: // TYA (implied)
            I98(CPU);
            break;
        case 0x99: // STA (ABS, Y)
            I99(CPU);
            break;
        case 0x9A: // TXS (implied)
            I9A(CPU);
            break;
        case 0x9D: // STA (ABS, X)
            I9D(CPU);
            break; // End 10th row
        case 0xA0: // LDY (IMM)
            IA0(CPU);
            break;
        case 0xA1: // LDA (IND, X)
            IA1(CPU);
            break;
        case 0xA2: // LDX (IMM)
            IA2(CPU);
            break;
        case 0xA4: // LDY (ZP)
            IA4(CPU);
            break;
        case 0xA5: // LDA (ZP)
            IA5(CPU);
            break;
        case 0xA6: // LDX (ZP)
            IA6(CPU);
            break;
        case 0xA8: // TAY (implied)
            IA8(CPU);
            break;
        case 0xA9: // LDA (IMM)
            IA9(CPU);
            break;
        case 0xAA: // TAX (implied)
            IAA(CPU);
            break;
        case 0xAC: // LDY (ABS)
            IAC(CPU);
            break;
        case 0xAD: // LDA (ABS)
            IAD(CPU);
            break;
        case 0xAE: // LDX (ABS)
            IAE(CPU);
            break; // End 11th row
        case 0xB0: // BCS (relative)
            IB0(CPU);
            break;
        case 0xB1: // LDA ((IND), Y)
            IB1(CPU);
            break;
        case 0xB4: // LDY (ZP, X) 
            IB4(CPU);
            break;
        case 0xB5: // LDA (ZP, X)
            IB5(CPU);
            break;
        case 0xB6: // LDX (ZP, Y)
            IB6(CPU);
            break;
        case 0xB8: // CLV (implied)
            IB8(CPU);
            break;
        case 0xB9: // LDA (ABS, Y)
            IB9(CPU);
            break;
        case 0xBA: // TSX (implied)
            IBA(CPU);
            break;
        case 0xBC: // LDY (ABS, X)
            IBC(CPU);
            break;
        case 0xBD: // LDA (ABS, X)
            IBD(CPU);
            break;
        case 0xBE: // LDA (ABS, Y)
            IBE(CPU);
            break; // End 12th row
        case 0xC0: // CPY (IMM) 
            IC0(CPU);
            break;
        case 0xC1: // CMP ((IND), Y)
            IC1(CPU);
            break;
        case 0xC4: // CPY (ZP)
            IC4(CPU);
            break;
        case 0xC5: // CMP (ZP)
            IC5(CPU);
            break;
        case 0xC6: // DEC (ZP)
            IC6(CPU);
            break;
        case 0xC8: // INY (implied)
            IC8(CPU);
            break;
        case 0xC9: // CMP (IMM)
            IC9(CPU);
            break;
        case 0xCA: // DEX (implied)
            ICA(CPU);
            break;
        case 0xCC: // CPY (ABS)
            ICC(CPU);
            break;
        case 0xCD: // CMP (ABS)
            ICD(CPU);
            break;
        case 0xCE: // DEC (ABS)
            ICE(CPU);
            break; // End 13th row
        case 0xD0: // BNE (relative)
            ID0(CPU);
            break;
        case 0xD1: // CMP ((IND), Y)
            ID1(CPU);
            break;
        case 0xD5: // CMP (ZP, X)
            ID5(CPU);
            break;
        case 0xD6: // DEC (ZP, X)
            ID6(CPU);
            break;
        case 0xD8: // CLD (implied)
            ID8(CPU);
            break;
        case 0xD9: // CMP (ABS, Y)
            ID9(CPU);
            break;
        case 0xDD: // CMP (ABS, X)
            IDD(CPU);
            break;
        case 0xDE: // CMP (ABS, X)
            IDE(CPU);
            break; // End 14th row
        case 0xE0: // CPX (IMM)
            IE0(CPU);
            break;
        case 0xE1: // SBC (IND, X)
            IE1(CPU);
            break;
        case 0xE4: // CPX (ZP)
            IE4(CPU);
            break;
        case 0xE5: // SBC (ZP)
            IE5(CPU);
            break;
        case 0xE6: // INC (ZP)
            IE6(CPU);
            break;
        case 0xE8: // INX (implied)
            IE8(CPU);
            break;
        case 0xE9: // SBC (IMM)
            IE9(CPU);
            break;
        case 0xEA: // NOP (implied)
            IEA(CPU);
            break;
        case 0xEC: // CPX (ABS)
            IEC(CPU);
            break;
        case 0xED: // SBC (ABS)
            IED(CPU);
            break;
        case 0xEE: // INC (ABS)
            IEE(CPU);
            break; // End 15th row
        case 0xF0: // BEQ (relative)
            IF0(CPU);
            break;
        case 0xF1: // SBC ((IND), Y)
            IF1(CPU);
            break;
        case 0xF5: // SBC (ZP, X)
            IF5(CPU);
            break;
        case 0xF6: // INC (ZP, X)
            IF6(CPU);
            break;
        case 0xF8: // SED (implied) Not sure if this is even going to have any effect, I may implement this functionality later
            IF8(CPU);
            break;
        case 0xF9: // SBC (ABS, Y)
            IF9(CPU);
            break;
        case 0xFD: // SBC (ABS, X)
            IFD(CPU);
            break;
        case 0xFE: // INC (ABS, X)
            IFE(CPU);
            break;

        default:
            break;
    }

    NMI(CPU);
    IRQ(CPU);
}