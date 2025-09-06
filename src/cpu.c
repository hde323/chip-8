#include <cpu.h>

void init_cpu(Cpu *cpu){
    memset(cpu, 0, sizeof(Cpu)); // clear RAM, FrameBuffer, registers, etc.
    cpu->PC = 0X200;
    cpu->speed = 500;
    const uint8_t fonts[] = {
        0XF0, 0X90, 0X90, 0X90, 0XF0,//0
        0X20, 0X60, 0X20, 0X20, 0X70,//1
        0XF0, 0X10, 0XF0, 0X80, 0XF0,//2
        0XF0, 0X10, 0XF0, 0X10, 0XF0,//3
        0X90, 0X90, 0XF0, 0X10, 0X10,//4
        0XF0, 0X80, 0XF0, 0X10, 0XF0,//5
        0XF0, 0X80, 0XF0, 0X90, 0XF0,//6
        0XF0, 0X10, 0X20, 0X40, 0X40,//7
        0XF0, 0X90, 0XF0, 0X90, 0XF0,//8
        0XF0, 0X90, 0XF0, 0X10, 0XF0,//9
        0XF0, 0X90, 0XF0, 0X90, 0X90,//A
        0XE0, 0X90, 0XE0, 0X90, 0XE0,//B
        0XF0, 0X80, 0X80, 0X80, 0XF0,//C
        0XE0, 0X90, 0X90, 0X90, 0XE0,//D
        0XF0, 0X80, 0XF0, 0X80, 0XF0,//E
        0XF0, 0X80, 0XF0, 0X80, 0X80 //F 
    };
    memcpy(cpu->Ram,fonts,sizeof(fonts));
};

uint8_t load_cpu_program(Cpu *cpu, char *fileName){
    FILE* rom = fopen(fileName, "rb");
    if(rom == NULL){
        printf("rom corrupted or dont exist!");
        return 1;
    }
    fseek(rom,0,SEEK_END);
    long romLength = ftell(rom);
    rewind(rom);
    if(romLength > 0XFFF - 0X200){
        printf("rom is too big!");
        return 1;
    }
    fread(&cpu->Ram[0x200],1,romLength,rom);
    fclose(rom);
    return 0;
};

void fetch_cpu_opcode(Cpu *cpu){
    uint16_t opcode = cpu->Ram[cpu->PC] << 8 | cpu->Ram[cpu->PC + 1];
    cpu->inst.Type = opcode >> 12;
    cpu->inst.X = (opcode >> 8) & 0XF;
    cpu->inst.Y = (opcode >> 4) & 0XF;
    cpu->inst.N = opcode & 0XF;
    cpu->inst.NN = opcode & 0xFF; 
    cpu->inst.NNN = opcode & 0XFFF;
    return;
};
void run_cpu_opcode(Cpu *cpu)
{
    fetch_cpu_opcode(cpu);
    printf("PC: %03X Opcode: %04X Type: %X X:%X Y:%X N:%X NN:%X NNN:%03X VX:%03x VY:%03x\n",
           cpu->PC,
           cpu->Ram[cpu->PC] << 8 | cpu->Ram[cpu->PC + 1],
           cpu->inst.Type, cpu->inst.X, cpu->inst.Y,
           cpu->inst.N, cpu->inst.NN, cpu->inst.NNN,
           cpu->V[cpu->inst.X],
           cpu->V[cpu->inst.Y]);
    switch (cpu->inst.Type)
    {
    case 0X0:
        switch(cpu->inst.N)
        {
            case 0X0:
                I00E0(cpu);
                break;
            case 0XE:
                I00EE(cpu);    
        }
        break;
    case 0X1:
        I1NNN(cpu, cpu->inst.NNN);
        break;
    case 0X2:
        I2NNN(cpu, cpu->inst.NNN);
        break;
    case 0X3:
        I3XNN(cpu, cpu->inst.X, cpu->inst.NN);
        break;
    case 0X4:
        I4NN(cpu, cpu->inst.X, cpu->inst.NN);
        break;
    case 0X5:
        I5XY0(cpu, cpu->inst.X, cpu->inst.Y);
        break;
    case 0X6:
        I6XNN(cpu, cpu->inst.X, cpu->inst.NN);
        break;
    case 0X7:
        I7XNN(cpu, cpu->inst.X, cpu->inst.NN);
        break;
    case 0X8:
        switch (cpu->inst.N)
        {
        case 0X0:
            I8XY0(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        case 0X1:
            I8XY1(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        case 0X2:
            I8XY2(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        case 0X3:
            I8XY3(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        case 0X4:
            I8XY4(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        case 0X5:
            I8XY5(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        case 0X6:
            I8XY6(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        case 0X7:
            I8XY7(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        case 0XE:
            I8XYE(cpu, cpu->inst.X, cpu->inst.Y);
            break;
        default:
            break;
        }
        break;
    case 0X9:
        I9XY0(cpu, cpu->inst.X, cpu->inst.Y);
        break;
    case 0XA:
        IANNN(cpu, cpu->inst.NNN);
        break;
    case 0XB:
        IBNNN(cpu, cpu->inst.NNN);
        break;
    case 0XC:
        ICXNN(cpu, cpu->inst.X, cpu->inst.NN);
        break;
    case 0XD:
        IDXYN(cpu, cpu->inst.X, cpu->inst.Y, cpu->inst.N);
        break;
    case 0XE:
        switch (cpu->inst.NN)
        {
        case 0X9E:
            IEX9E(cpu, cpu->inst.X);
            break;
        case 0XA1:
            IEXA1(cpu, cpu->inst.X);
        default:
            break;
        }
        break;
    case 0XF:
        switch (cpu->inst.NN)
        {
        case 0X07:
            IFX07(cpu, cpu->inst.X);
            break;
        case 0X0A:
            IFX0A(cpu, cpu->inst.X);
            break;
        case 0X15:
            IFX15(cpu, cpu->inst.X);
            break;
        case 0X18:
            IFX18(cpu, cpu->inst.X);
            break;
        case 0X1E:
            IFX1E(cpu, cpu->inst.X);
            break;
        case 0X29:
            IFX29(cpu, cpu->inst.X);
            break;
        case 0X33:
            IFX33(cpu, cpu->inst.X);
            break;
        case 0X55:
            IFX55(cpu, cpu->inst.X);
            break;
        case 0X65:
            IFX65(cpu, cpu->inst.X);
            break;
        default:
            break;
        }
        break;            
    default:
        break;
    }
};
//cpu instructions
void I00E0(Cpu *cpu)// clear display
{
    for (uint16_t i = 0; i < sizeof(cpu->FrameBuffer); i++)
    {
        cpu->FrameBuffer[i] = 0;
    }
    cpu->PC += 2;
    return;
};

void I00EE(Cpu *cpu)// exit subroutine
{
    cpu->PC = cpu->Stack[cpu->SP];
    cpu->SP--;
};

void I1NNN(Cpu *cpu, uint16_t address)// jump to address NNN
{
    cpu->PC = address;
    return;
};

void I2NNN(Cpu *cpu, uint16_t address)// call subroutine NNN
{
    cpu->SP ++;
    cpu->Stack[cpu->SP] = cpu->PC + 2;
    cpu->PC = address;
    return;
};

void I3XNN(Cpu *cpu, uint8_t x, uint8_t byte)// skip instruction if vx = NN
{
    if(cpu->V[x] == byte) 
       cpu->PC += 4;
    else 
       cpu->PC += 2;
    return;
};

void I4NN(Cpu *cpu, uint8_t x, uint8_t byte)// skip inststruction if vx != NN
{
    if(cpu->V[x] != byte) 
       cpu->PC += 4;
    else 
       cpu->PC += 2;
    return;
};

void I5XY0(Cpu *cpu, uint8_t x, uint8_t y)// skip instruction if x = y
{
    if(cpu->V[x] == cpu->V[y]) 
       cpu->PC += 4;
    else 
       cpu->PC += 2;
    return;
};

void I6XNN(Cpu *cpu, uint8_t x, uint8_t byte)// vx = NN
{
    cpu->V[x] = byte;
    cpu->PC += 2;
    return;
};

void I7XNN(Cpu *cpu, uint8_t x, uint8_t byte)// vx += NN
{
    cpu->V[x] += byte;
    cpu->PC += 2;
    return;
};

void I8XY0(Cpu *cpu, uint8_t x, uint8_t y)// vx = vy
{
    cpu->V[x] = cpu->V[y];
    cpu->PC += 2;
    return;
};

void I8XY1(Cpu *cpu, uint8_t x, uint8_t y)// vx = vx OR vy
{
    cpu->V[x] = cpu->V[x] | cpu->V[y];
    cpu->PC += 2;
    return;
};

void I8XY2(Cpu *cpu, uint8_t x, uint8_t y)// vx = vx AND vy
{
    cpu->V[x] = cpu->V[x] & cpu->V[y];
    cpu->PC += 2;
    return;
};

void I8XY3(Cpu *cpu, uint8_t x, uint8_t y)// vx = vx XOR vy
{
    cpu->V[x] = cpu->V[x] ^ cpu->V[y];
    cpu->PC += 2;
    return;
};

void I8XY4(Cpu *cpu, uint8_t x, uint8_t y)// vx += vy , vf = 1 on carry. lowest 8bits kept in vx
{
    uint16_t sum = cpu->V[y] + cpu->V[x];
    cpu->V[0xF] = (sum > 0xFF) ? 1 : 0;
    cpu->V[x] = sum & 0xFF; 
    cpu->PC += 2;
    return;
};

void I8XY5(Cpu *cpu, uint8_t x, uint8_t y)// vx -= vy, vf = 0 on borrow
{
    if(cpu->V[x] >= cpu->V[y]) {
        cpu->V[0xF] = 1;           
        cpu->V[x] -= cpu->V[y];     
    } else {
        cpu->V[0xF] = 0;           
        cpu->V[x] = (cpu->V[x] - cpu->V[y]) & 0xFF; 
    }
    cpu->PC += 2;
    return;
};

void I8XY7(Cpu *cpu, uint8_t x, uint8_t y)// vx = vy - vx vf = 0 on borrow
{
    if (cpu->V[y] >= cpu->V[x])
    {
        cpu->V[0XF] = 1;
        cpu->V[x] = cpu->V[y] - cpu->V[x];
    }
    else
    {
        cpu->V[0XF] = 0;
        cpu->V[x] = (cpu->V[y] - cpu->V[x]) & 0xFF; 

    }
    cpu->PC += 2;
    return;
};
void I8XY6(Cpu *cpu, uint8_t x, uint8_t y) // SHR Vx
{
    cpu->V[0xF] = cpu->V[x] & 0x1; 
    cpu->V[x] /= 2;                
    cpu->PC += 2;
};

void I8XYE(Cpu *cpu, uint8_t x, uint8_t y) // SHL Vx
{
    cpu->V[0xF] = (cpu->V[x] & 0x80) >> 7; 
    cpu->V[x] *= 2;                        
    cpu->PC += 2;
};

void I9XY0(Cpu *cpu, uint8_t x, uint8_t y)// skip instruction if vx != vy
{
    if(cpu->V[x] != cpu->V[y])
       cpu->PC += 4;
    else
       cpu->PC += 2;
    return;
};

void IANNN(Cpu *cpu, uint16_t address)// set register I to NNN
{
    cpu->Index = address;
    cpu->PC += 2;
    return;
};

void IBNNN(Cpu *cpu, uint16_t address)// Jump to location nnn + V0. The program counter is set to nnn plus the value of V0.
{
    cpu->PC = address + cpu->V[0x0];
    return;
};

void ICXNN(Cpu *cpu, uint8_t x, uint8_t byte)// vx = random byte AND NN
{
    uint8_t randomByte = (rand() % 255);
    cpu->V[x] = randomByte & byte;
    cpu->PC += 2;
    return;
};

//Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. The interpreter reads n
//bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen
//at coordinates (Vx, Vy). Sprites are XOR’d onto the existing screen. If this causes any pixels to be erased,
//VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of
//the display, it wraps around to the opposite side of the screen.
void IDXYN(Cpu *cpu, uint8_t x, uint8_t y, uint8_t n)
{
    uint8_t xPos = cpu->V[x] % 64;
    uint8_t yPos = cpu->V[y] % 32;

    cpu->V[0xF] = 0; // reset collision flag

    for (unsigned int row = 0; row < n; row++) {
        uint8_t spriteByte = cpu->Ram[cpu->Index + row];

        for (unsigned int col = 0; col < 8; col++) {
            uint8_t spritePixel = (spriteByte >> (7 - col)) & 1;
            if (spritePixel) {
                uint16_t fbIndex = ((yPos + row) % 32) * 64 + ((xPos + col) % 64);

                if (cpu->FrameBuffer[fbIndex] == 1) {
                    cpu->V[0xF] = 1; // collision
                }

                cpu->FrameBuffer[fbIndex] ^= 1; // XOR draw
            }
        }
    }
    cpu->PC += 2;
    return;
}



void IEX9E(Cpu *cpu, uint8_t x)// skip instruction if keypad[vx] is pressed
{
    if(cpu->KeyPad[cpu->V[x]] == 1)
      cpu->PC += 4;
    else
      cpu->PC += 2;
    return;
};

void IEXA1(Cpu *cpu, uint8_t x)// skip instruction if keypad[vx] not pressed
{
    if(cpu->KeyPad[cpu->V[x]] == 0)
      cpu->PC += 4;
    else
      cpu->PC += 2;
    return;
};

void IFX07(Cpu *cpu, uint8_t x)// vx = dt
{
    cpu->V[x] = cpu->DT;
    cpu->PC += 2;
    return;
};

void IFX0A(Cpu *cpu, uint8_t x)
{
    for (int i = 0; i < 16; i++)
    {
        if(cpu->KeyPad[i] == 1) // check all keys
        {
            cpu->V[x] = i;      // store key index
            cpu->PC += 2;       // advance PC
            return;
        }
    }
    // If no key pressed, do NOT advance PC (CPU "waits")
}


void IFX15(Cpu *cpu, uint8_t x)// dt = vx
{
    cpu->DT = cpu->V[x];
    cpu->PC += 2;
    return;
};

void IFX18(Cpu *cpu, uint8_t x)// vx = st
{
    cpu->ST = cpu->V[x];
    cpu->PC += 2;
    return;
};

void IFX1E(Cpu *cpu, uint8_t x)// I += vx
{
    cpu->Index += cpu->V[x];
    cpu->PC += 2;
    return;
};

void IFX29(Cpu *cpu, uint8_t x)
{
    cpu->Index = cpu->V[x] * 5;
    cpu->PC += 2;
    return;
};

void IFX33(Cpu *cpu, uint8_t x)// Ram[I] = (hundreds)x, Ram[I+1] = (tens)x, Ram[I+2] = (ones)x
{
    cpu->Ram[cpu->Index] = cpu->V[x] / 100;
    cpu->Ram[cpu->Index + 1] = (cpu->V[x] / 10) % 10;
    cpu->Ram[cpu->Index + 2] = cpu->V[x] % 10;
    cpu->PC += 2;
    return;
};

void IFX55(Cpu *cpu, uint8_t x)
{
    for (int i = 0; i <= x; i++) 
    {
        cpu->Ram[cpu->Index + i] = cpu->V[i];
    }
    cpu->PC += 2;
}

void IFX65(Cpu *cpu, uint8_t x)
{
    for (int i = 0; i <= x; i++)
    {
        cpu->V[i] = cpu->Ram[cpu->Index + i];
    }
    cpu->PC += 2;
}