#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    uint8_t Type;
    uint16_t NNN;
    uint8_t NN;
    uint8_t N;
    uint8_t X;
    uint8_t Y;
}inst_t;

typedef struct
{
    uint8_t Ram[4096];// 0X00 - 0X80 reserved for fonts , 0x200 - 0xfff reserved for program
    uint8_t FrameBuffer[64*32];
    uint8_t V[16]; //regiters
    uint16_t Index;
    uint16_t Stack[16];
    uint8_t SP; //stack pointer
    uint16_t PC; //program counter
    uint8_t DT; //delay timer
    uint8_t ST; //sound timer
    uint8_t KeyPad[16];
    uint8_t keyPressed;
    inst_t inst;
    uint32_t speed;//instructions per seccond
}Cpu;

void init_cpu(Cpu *cpu);
uint8_t load_cpu_program(Cpu *cpu, char *fileName);
void fetch_cpu_opcode(Cpu *cpu);
void run_cpu_opcode(Cpu *cpu);
//cpu instructions
void I00E0(Cpu *cpu);// clear display
void I00EE(Cpu *cpu);// exit subroutine
void I1NNN(Cpu *cpu, uint16_t address);// jump to address NNN
void I2NNN(Cpu *cpu, uint16_t address);// call subroutine NNN
void I3XNN(Cpu *cpu, uint8_t x, uint8_t byte);// skip instruction if vx = NN
void I4NN(Cpu *cpu, uint8_t x, uint8_t byte);// skip inststruction if vx != NN
void I5XY0(Cpu *cpu, uint8_t x, uint8_t y);// skip instruction if x = y
void I6XNN(Cpu *cpu, uint8_t x, uint8_t byte);// vx = NN
void I7XNN(Cpu *cpu, uint8_t x, uint8_t byte);// vx += NN
void I8XY0(Cpu *cpu, uint8_t x, uint8_t y);// vx = vy
void I8XY1(Cpu *cpu, uint8_t x, uint8_t y);// vx = vx OR vy
void I8XY2(Cpu *cpu, uint8_t x, uint8_t y);// vx = vx AND vy
void I8XY3(Cpu *cpu, uint8_t x, uint8_t y);// vx = vx XOR vy
void I8XY4(Cpu *cpu, uint8_t x, uint8_t y);// vx += vy , vf = 1 on carry. lowest 8bits kept in vx
void I8XY5(Cpu *cpu, uint8_t x, uint8_t y);// vx -= vy, vf = 0 on borrow
void I8XY6(Cpu *cpu, uint8_t x, uint8_t y);// vx <<= vy vf = old least significant bit
void I8XY7(Cpu *cpu, uint8_t x, uint8_t y);// vx = vy - vx vf = 0 on borrow
void I8XYE(Cpu *cpu, uint8_t x, uint8_t y);// vx <<= vy vf = old most significant bit
void I9XY0(Cpu *cpu, uint8_t x, uint8_t y);// skip instruction if vx != vy
void IANNN(Cpu *cpu, uint16_t address);// set register I to NNN
void IBNNN(Cpu *cpu, uint16_t address);// Jump to location nnn + V0. The program counter is set to nnn plus the value of V0.
void ICXNN(Cpu *cpu, uint8_t x, uint8_t byte);// vx = random byte AND NN
//Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. The interpreter reads n
//bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen
//at coordinates (Vx, Vy). Sprites are XOR’d onto the existing screen. If this causes any pixels to be erased,
//VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of
//the display, it wraps around to the opposite side of the screen.
void IDXYN(Cpu *cpu, uint8_t x, uint8_t y,uint8_t n);
void IEX9E(Cpu *cpu, uint8_t x);// skip instruction if keypad[vx] is pressed
void IEXA1(Cpu *cpu, uint8_t x);// skip instruction if keypad[vx] not pressed
void IFX07(Cpu *cpu, uint8_t x);// vx = dt
void IFX0A(Cpu *cpu, uint8_t x);// wait kepressed store in vx
void IFX15(Cpu *cpu, uint8_t x);// dt = vx
void IFX18(Cpu *cpu, uint8_t x);// vx = st
void IFX1E(Cpu *cpu, uint8_t x);// I += vx
void IFX29(Cpu *cpu, uint8_t x);
void IFX33(Cpu *cpu, uint8_t x);// Ram[I] = (hundreds)x, Ram[I+1] = (tens)x, Ram[I+2] = (ones)x
void IFX55(Cpu *cpu, uint8_t x);// Save v0-vx to i through (i+x)
void IFX65(Cpu *cpu, uint8_t x);//Load v0-vx from i through (i+x)


#endif