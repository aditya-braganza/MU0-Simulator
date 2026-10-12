#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define SIXTEENBITMASK 0xffff
#define TWELVEBITMASK 0x0fff
#define TWOBITMASK 0x003

#define MEMORYSIZE 0x1000

typedef struct{
    uint16_t Acc; // 16-Bit accumulator
    bool N; // Negative Flag
    bool Z; // Zero Flag
} flags;

typedef struct {
    uint16_t PC; // 12-Bit program counter (defined with a 16bit unsigned integer, needs masking)
    flags F; // Accumulator, Negative Flag, Zero Flag
    uint16_t IR; // 16-Bit Instruction Register

    uint16_t memory[MEMORYSIZE]; // MU0 Memory of unsigned 16 bit integers
    bool running; // Flag for to enable and disable hardware running
} hardware;

void adder(uint16_t X, uint16_t Y, uint8_t M, uint16_t *Q){
    switch(M & TWOBITMASK){
        case 0x0: *Q = Y; break;
        case 0x1: *Q = (X + Y); break;
        case 0x2: *Q = (X + 1); break;
        case 0x3: *Q = (X - Y); break;
    }
    *Q &= SIXTEENBITMASK;
}

int clock(hardware *hw){
    // Initial running check
    if(!hw->running) return 1;

    // Next state variables to prevent accidental race conditions
    uint16_t next_PC = hw->PC;
    uint16_t next_Acc = hw->F.Acc;

    // Fetch Phase
    uint16_t next_IR = hw->memory[hw->PC];
    adder(hw->PC, 0, 0x2, &next_PC);

    // Decode Phase
    uint8_t opcode = (next_IR >> 12) & 0x0F;
    uint16_t address = next_IR & TWELVEBITMASK;

    // Execute Phase
    switch (opcode){
        case 0x0: next_Acc = hw->memory[address]; break; // LDA
        case 0x1: hw->memory[address] = hw->F.Acc; break; // STO
        case 0x2: adder(hw->F.Acc, hw->memory[address], 0x1, &next_Acc); break; // ADD
        case 0x3: adder(hw->F.Acc, hw->memory[address], 0x3, &next_Acc); break; // SUB
        case 0x4: next_PC = address; break; // JMP
        case 0x5: if (!hw->F.N) next_PC = address; break; // JGE
        case 0x6: if (!hw->F.Z) next_PC = address; break; // JNE
        case 0x7: hw->running = false; break; // STP
        default: printf("Oopsies, that opcode doesn't exist\n"); hw->running = false; return -1;
    }

    // Register Updates
    hw->PC = next_PC & TWELVEBITMASK;
    hw->F.Acc = next_Acc;
    hw->F.Z = hw->F.Acc == 0x0000;
    hw->F.N = (hw->F.Acc >> 15) == 1;
    hw->IR = next_IR;
    return 0;
}

int main() {
    return 0;
}