#ifndef CPU_H
#define CPU_H
struct cpu {
    struct bus* BUS;
    // Creating queues for interrupts (use threading for emulating interrupts)
    unsigned char NMIFLAG;

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
            unsigned char U: 1; // Unused flag (always set to 1) - Will use it for nmi heheh
            unsigned char O: 1; // Overflow flag (1 if last arithmetic operation resulted in an overflow)
            unsigned char N: 1; // Negative flag (1 if result most significant bit was set)
        };
        unsigned char SR; // Status register (contains flags)
    };
};

// Signatures
void reset(struct cpu* CPU);
void IRQ(struct cpu* CPU);
void NMI(struct cpu* cpu);

// Read and writes
char writeb(struct cpu* CPU, unsigned short addr, unsigned char data);
unsigned char readb(struct cpu* CPU, unsigned short addr, unsigned char bReadOnly);

void connect_bus(struct cpu* CPU, struct bus* BUS);
void execute_instruction(struct cpu* CPU);
#endif