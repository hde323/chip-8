#include "cpu.h"
#include "graphics.h"
#include <stdbool.h>
#include <stdio.h>

//debuging messages 
void print_memory(uint8_t *mem, size_t start, size_t end, const char *label) {
    printf("=== %s ===\n", label);
    for (size_t i = start; i < end; i++) {
        printf("%02X ", mem[i]);
        if ((i - start + 1) % 16 == 0) printf("\n");
    }
    printf("\n\n");
}

int main(int argc, char **argv) {
    uint64_t looptimer = 0;
    uint32_t lastInstructionEx = SDL_GetTicks();
    uint32_t lastDisplayUpdate = SDL_GetTicks();
    Cpu cpu;
    memset(&cpu, 0, sizeof(Cpu));
    init_cpu(&cpu);
    print_memory(cpu.Ram, 0x00, 0x50, "Font Data");
    if (load_cpu_program(&cpu, "../src/roms/BRIX") != 0) {
        printf("Failed to load ROM!\n");
        return 1;
    }
    print_memory(cpu.Ram, 0x200, 0x200 + 256, "Loaded ROM (first 256 bytes)");

    if (init_graphics() != 0) return 1;

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                cpu.keyPressed = 1;
                switch (e.key.keysym.sym)
                {
                case SDLK_SPACE:
                    running = false;
                    break;
                case SDLK_1:
                    cpu.KeyPad[0x1] = 1;
                    break;
                case SDLK_2:
                    cpu.KeyPad[0x2] = 1;
                    break;
                case SDLK_3:
                    cpu.KeyPad[0x3] = 1;
                    break;
                case SDLK_4:
                    cpu.KeyPad[0xC] = 1;
                    break;
                case SDLK_q:
                    cpu.KeyPad[0x4] = 1;
                    break;
                case SDLK_w:
                    cpu.KeyPad[0x5] = 1;
                    break;
                case SDLK_e:
                    cpu.KeyPad[0x6] = 1;
                    break;
                case SDLK_r:
                    cpu.KeyPad[0xD] = 1;
                    break;
                case SDLK_a:
                    cpu.KeyPad[0x7] = 1;
                    break;
                case SDLK_s:
                    cpu.KeyPad[0x8] = 1;
                    break;
                case SDLK_d:
                    cpu.KeyPad[0x9] = 1;
                    break;
                case SDLK_f:
                    cpu.KeyPad[0xE] = 1;
                    break;
                case SDLK_z:
                    cpu.KeyPad[0xA] = 1;
                    break;
                case SDLK_x:
                    cpu.KeyPad[0x0] = 1;
                    break;
                case SDLK_c:
                    cpu.KeyPad[0xB] = 1;
                    break;
                case SDLK_v:
                    cpu.KeyPad[0xF] = 1;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                cpu.keyPressed = 0;
                switch (e.key.keysym.sym)
                {
                case SDLK_1:
                    cpu.KeyPad[0x1] = 0;
                    break;
                case SDLK_2:
                    cpu.KeyPad[0x2] = 0;
                    break;
                case SDLK_3:
                    cpu.KeyPad[0x3] = 0;
                    break;
                case SDLK_4:
                    cpu.KeyPad[0xC] = 0;
                    break;
                case SDLK_q:
                    cpu.KeyPad[0x4] = 0;
                    break;
                case SDLK_w:
                    cpu.KeyPad[0x5] = 0;
                    break;
                case SDLK_e:
                    cpu.KeyPad[0x6] = 0;
                    break;
                case SDLK_r:
                    cpu.KeyPad[0xD] = 0;
                    break;
                case SDLK_a:
                    cpu.KeyPad[0x7] = 0;
                    break;
                case SDLK_s:
                    cpu.KeyPad[0x8] = 0;
                    break;
                case SDLK_d:
                    cpu.KeyPad[0x9] = 0;
                    break;
                case SDLK_f:
                    cpu.KeyPad[0xE] = 0;
                    break;
                case SDLK_z:
                    cpu.KeyPad[0xA] = 0;
                    break;
                case SDLK_x:
                    cpu.KeyPad[0x0] = 0;
                    break;
                case SDLK_c:
                    cpu.KeyPad[0xB] = 0;
                    break;
                case SDLK_v:
                    cpu.KeyPad[0xF] = 0;
                    break;
                default:
                    break;
                }
            default:
                break;
            }
            break;
            
        }
        uint32_t now = SDL_GetTicks();
        if(now - lastInstructionEx >= 2)
        {
            run_cpu_opcode(&cpu);
            lastInstructionEx = now;
        }
        if(now - lastDisplayUpdate >= 16)
        {
            if(cpu.DT > 0) cpu.DT --;
            if(cpu.ST > 0) cpu.ST --;
            draw_graphics(&cpu);
            lastDisplayUpdate = now;
        }
    }
    close_graphics();
    return 0;
}
