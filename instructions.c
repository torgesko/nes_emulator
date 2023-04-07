#include "instructions.h"
#include "cpu.h"

/* THESE ARE THE ADDRESSING MODES FOR THE 6502 CPU */
unsigned short IMM(unsigned char* address_space, unsigned short PC){
    unsigned short address = PC + 1; // Optimize with compiler

    return address;
}

unsigned short ABS(unsigned char* address_space, unsigned short PC){
    unsigned short address = *(unsigned short*)(address_space + PC + 1);

    return address;
}

unsigned short ABSI(unsigned char* address_space, unsigned short PC, unsigned char I){
    unsigned short address = *(unsigned short*)(address_space + PC + 1) + I;

    return address;
}

unsigned short ABSIND(unsigned char* address_space , unsigned short PC){
    unsigned short address = *(unsigned short*)(address_space + PC + 1);
    address = *(unsigned short*)(address_space + address);

    return address;
}

unsigned short IND(unsigned char* address_space, unsigned short PC){
    unsigned char zp_addr = address_space[PC];
    
    unsigned short address = *(unsigned short*) (address_space + zp_addr);

    return address;
}

unsigned short INDX(unsigned char* address_space, unsigned short PC, unsigned char X){
    unsigned char arg = address_space[PC + 1];
    unsigned short address = *(address_space + (arg + X) % 256) + *(address_space + (arg + X + 1) % 256) << 8;

    return address;
}

unsigned short INDY(unsigned char* address_space, unsigned short PC, unsigned char Y){
    unsigned char arg = address_space[PC + 1];
    unsigned short address = address_space[arg] + Y + *(address_space + (arg + 1) % 256) << 8;

    return address;
}

unsigned short ZP0(unsigned char* address_space, unsigned short PC){
    unsigned short address = address_space[PC + 1];

    return address;
}

unsigned short ZPI(unsigned char* address_space, unsigned short PC, unsigned char I){
    unsigned short address = (I + address_space[PC + 1]) % 256;

    return address;
}

/* BASIC INSTRUCTIONS */
void ADC(struct cpu* CPU, unsigned char value){}

void AND(struct cpu* CPU, unsigned char value){}

void BCC(struct cpu* CPU, unsigned char value){}

void BCS(struct cpu* CPU, unsigned char value){}

void BEQ(struct cpu* CPU, unsigned char value){}

void BIT(struct cpu* CPU, unsigned char value){}

void BMI(struct cpu* CPU, unsigned char value){}

void BNE(struct cpu* CPU, unsigned char value){}

void BPL(struct cpu* CPU, unsigned char value){}

void BRK(struct cpu* CPU, unsigned char value){}

void BVC(struct cpu* CPU, unsigned char value){}

void BVS(struct cpu* CPU, unsigned char value){}

void CLC(struct cpu* CPU, unsigned char value){}

void CLD(struct cpu* CPU, unsigned char value){}

void CLI(struct cpu* CPU, unsigned char value){}

void CLV(struct cpu* CPU, unsigned char value){}

void CMP(struct cpu* CPU, unsigned char value){}

void CPX(struct cpu* CPU, unsigned char value){}

void CPY(struct cpu* CPU, unsigned char value){}

void DEC(struct cpu* CPU, unsigned char value){}

void DEX(struct cpu* CPU, unsigned char value){}

void DEY(struct cpu* CPU, unsigned char value){}

void EOR(struct cpu* CPU, unsigned char value){}

void INC(struct cpu* CPU, unsigned char value){}

void INX(struct cpu* CPU, unsigned char value){}

void INY(struct cpu* CPU, unsigned char value){}

void JMP(struct cpu* CPU, unsigned char value){}

void JSR(struct cpu* CPU, unsigned char value){}

void LDA(struct cpu* CPU, unsigned char value){}

void LDX(struct cpu* CPU, unsigned char value){}

void LDY(struct cpu* CPU, unsigned char value){}

void LSR(struct cpu* CPU, unsigned char value){}

void NOP(struct cpu* CPU, unsigned char value){}

void ORA(struct cpu* CPU, unsigned char value){}

void PHA(struct cpu* CPU, unsigned char value){}

void PHP(struct cpu* CPU, unsigned char value){}

void PLA(struct cpu* CPU, unsigned char value){}

void PLP(struct cpu* CPU, unsigned char value){}

void ROL(struct cpu* CPU, unsigned char value){}

void RTI(struct cpu* CPU, unsigned char value){}

void RTS(struct cpu* CPU, unsigned char value){}

void SBC(struct cpu* CPU, unsigned char value){}

void SEC(struct cpu* CPU, unsigned char value){}

void SED(struct cpu* CPU, unsigned char value){}

void SEI(struct cpu* CPU, unsigned char value){}

void STA(struct cpu* CPU, unsigned char value){}

void STX(struct cpu* CPU, unsigned char value){}

void STY(struct cpu* CPU, unsigned char value){}

void TAX(struct cpu* CPU, unsigned char value){}

void TAY(struct cpu* CPU, unsigned char value){}

void TSX(struct cpu* CPU, unsigned char value){}

void TXA(struct cpu* CPU, unsigned char value){}

void TXS(struct cpu* CPU, unsigned char value){}

void TYA(struct cpu* CPU, unsigned char value){}



/* OPCODES */
void I00(struct cpu* CPU){}

void I01(struct cpu* CPU){}

void I05(struct cpu* CPU){}

void I06(struct cpu* CPU){}

void I08(struct cpu* CPU){}

void I09(struct cpu* CPU){}

void I0A(struct cpu* CPU){}

void I0E(struct cpu* CPU){}

void I10(struct cpu* CPU){}

void I11(struct cpu* CPU){}

void I15(struct cpu* CPU){}

void I16(struct cpu* CPU){}

void I18(struct cpu* CPU){}

void I19(struct cpu* CPU){}

void I1D(struct cpu* CPU){}

void I1E(struct cpu* CPU){}

void I20(struct cpu* CPU){}

void I21(struct cpu* CPU){}

void I24(struct cpu* CPU){}

void I25(struct cpu* CPU){}

void I26(struct cpu* CPU){}

void I28(struct cpu* CPU){}

void I29(struct cpu* CPU){}

void I2A(struct cpu* CPU){}

void I2C(struct cpu* CPU){}

void I2D(struct cpu* CPU){}

void I2E(struct cpu* CPU){}

void I30(struct cpu* CPU){}

void I31(struct cpu* CPU){}

void I35(struct cpu* CPU){}

void I36(struct cpu* CPU){}

void I38(struct cpu* CPU){}

void I39(struct cpu* CPU){}

void I3D(struct cpu* CPU){}

void I3E(struct cpu* CPU){}

void I40(struct cpu* CPU){
    CPU->I = 0;

    CPU->SP++;
    CPU->SR = readb(CPU, 0x0100 + CPU->SP, 0);

    CPU->SP++;
    CPU->PCL = readb(CPU, 0x0100 + CPU->SP, 0);
    CPU->SP++;
    CPU->PCH = readb(CPU, 0x0100 + CPU->SP, 0);
}

void I41(struct cpu* CPU){}

void I45(struct cpu* CPU){}

void I46(struct cpu* CPU){}

void I48(struct cpu* CPU){}

void I49(struct cpu* CPU){}

void I4A(struct cpu* CPU){}

void I4C(struct cpu* CPU){}

void I4D(struct cpu* CPU){}

void I4E(struct cpu* CPU){}

void I50(struct cpu* CPU){}

void I51(struct cpu* CPU){}

void I55(struct cpu* CPU){}

void I56(struct cpu* CPU){}

void I58(struct cpu* CPU){}

void I59(struct cpu* CPU){}

void I5D(struct cpu* CPU){}

void I5E(struct cpu* CPU){}

void I60(struct cpu* CPU){}

void I60(struct cpu* CPU){}

void I61(struct cpu* CPU){}

void I65(struct cpu* CPU){}

void I66(struct cpu* CPU){}

void I68(struct cpu* CPU){}

void I69(struct cpu* CPU){}

void I6A(struct cpu* CPU){}

void I6C(struct cpu* CPU){}

void I6D(struct cpu* CPU){}

void I6E(struct cpu* CPU){}

void I70(struct cpu* CPU){}

void I71(struct cpu* CPU){}

void I75(struct cpu* CPU){}

void I76(struct cpu* CPU){}

void I78(struct cpu* CPU){}

void I79(struct cpu* CPU){}

void I7D(struct cpu* CPU){}

void I7E(struct cpu* CPU){}

void I81(struct cpu* CPU){}

void I84(struct cpu* CPU){}

void I85(struct cpu* CPU){}

void I86(struct cpu* CPU){}

void I88(struct cpu* CPU){}

void I8A(struct cpu* CPU){}

void I8C(struct cpu* CPU){}

void I8D(struct cpu* CPU){}

void I8E(struct cpu* CPU){}

void I90(struct cpu* CPU){}

void I91(struct cpu* CPU){}

void I94(struct cpu* CPU){}

void I95(struct cpu* CPU){}

void I96(struct cpu* CPU){}

void I98(struct cpu* CPU){}

void I99(struct cpu* CPU){}

void I9A(struct cpu* CPU){}

void I9D(struct cpu* CPU){}

void IA0(struct cpu* CPU){}

void IA1(struct cpu* CPU){}

void IA2(struct cpu* CPU){}

void IA4(struct cpu* CPU){}

void IA5(struct cpu* CPU){}

void IA6(struct cpu* CPU){}

void IA8(struct cpu* CPU){}

void IA9(struct cpu* CPU){}

void IAA(struct cpu* CPU){}

void IAC(struct cpu* CPU){}

void IAD(struct cpu* CPU){}

void IAE(struct cpu* CPU){}

void IB0(struct cpu* CPU){}

void IB1(struct cpu* CPU){}

void IB4(struct cpu* CPU){}

void IB5(struct cpu* CPU){}

void IB6(struct cpu* CPU){}

void IB8(struct cpu* CPU){}

void IB9(struct cpu* CPU){}

void IBA(struct cpu* CPU){}

void IBC(struct cpu* CPU){}

void IBD(struct cpu* CPU){}

void IBE(struct cpu* CPU){}

void IC0(struct cpu* CPU){}

void IC1(struct cpu* CPU){}

void IC4(struct cpu* CPU){}

void IC5(struct cpu* CPU){}

void IC6(struct cpu* CPU){}

void IC8(struct cpu* CPU){}

void IC9(struct cpu* CPU){}

void ICA(struct cpu* CPU){}

void ICC(struct cpu* CPU){}

void ICD(struct cpu* CPU){}

void ICE(struct cpu* CPU){}

void ID0(struct cpu* CPU){}

void ID1(struct cpu* CPU){}

void ID5(struct cpu* CPU){}

void ID6(struct cpu* CPU){}

void ID8(struct cpu* CPU){}

void ID9(struct cpu* CPU){}

void IDD(struct cpu* CPU){}

void IDE(struct cpu* CPU){}

void IE0(struct cpu* CPU){}

void IE1(struct cpu* CPU){}

void IE4(struct cpu* CPU){}

void IE5(struct cpu* CPU){}

void IE6(struct cpu* CPU){}

void IE8(struct cpu* CPU){}

void IE9(struct cpu* CPU){}

void IEA(struct cpu* CPU){}

void IEC(struct cpu* CPU){}

void IED(struct cpu* CPU){}

void IEE(struct cpu* CPU){}

void IF0(struct cpu* CPU){}

void IF1(struct cpu* CPU){}

void IF5(struct cpu* CPU){}

void IF6(struct cpu* CPU){}

void IF8(struct cpu* CPU){}

void IF9(struct cpu* CPU){}

void IFD(struct cpu* CPU){}

void IFE(struct cpu* CPU){}