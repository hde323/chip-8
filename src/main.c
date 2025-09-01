#include "cpu.h"
#include "graphics.h"
#include <stdbool.h>
#include <stdio.h>

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
    Cpu cpu;
    memset(&cpu, 0, sizeof(Cpu));

    // Initialize CPU (loads fonts)
    init_cpu(&cpu);

    // Print font data (first 80 bytes, 0x00-0x4F)
    print_memory(cpu.Ram, 0x00, 0x50, "Font Data");

    // Load ROM
    if (load_cpu_program(&cpu, "../src/roms/TETRIS") != 0) {
        printf("Failed to load ROM!\n");
        return 1;
    }

    // Print loaded ROM memory (0x200 - 0x200 + rom size)
    print_memory(cpu.Ram, 0x200, 0x200 + 256, "Loaded ROM (first 256 bytes)");

    if (init_graphics() != 0) return 1;

    bool running = true;
    SDL_Event e;

    while (running) {
        run_cpu_opcode(&cpu);
        while (SDL_PollEvent(&e)) {
            //if (e.type == SDL_QUIT) running = false;
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
                case SDLK_0:
                    cpu.KeyPad[0] = 1;
                    break;
                case SDLK_1:
                    cpu.KeyPad[1] = 1;
                    break;
                case SDLK_2:
                    cpu.KeyPad[2] = 1;
                    break;
                case SDLK_3:
                    cpu.KeyPad[3] = 1;
                    break;
                case SDLK_4:
                    cpu.KeyPad[4] = 1;
                    break;
                case SDLK_5:
                    cpu.KeyPad[5] = 1;
                    break;
                case SDLK_6:
                    cpu.KeyPad[6] = 1;
                    break;
                case SDLK_7:
                    cpu.KeyPad[7] = 1;
                    break;
                case SDLK_8:
                    cpu.KeyPad[8] = 1;
                    break;
                case SDLK_9:
                    cpu.KeyPad[9] = 1;
                    break;
                case SDLK_a:
                    cpu.KeyPad[10] = 1;
                    break;
                case SDLK_b:
                    cpu.KeyPad[11] = 1;
                    break;
                case SDLK_c:
                    cpu.KeyPad[12] = 1;
                    break;
                case SDLK_d:
                    cpu.KeyPad[13] = 1;
                    break;
                case SDLK_e:
                    cpu.KeyPad[14] = 1;
                    break;
                case SDLK_f:
                    cpu.KeyPad[15] = 1;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                cpu.keyPressed = 0;
                switch (e.key.keysym.sym)
                {
                case SDLK_SPACE:
                    running = false;
                    break;
                case SDLK_0:
                    cpu.KeyPad[0] = 0;
                    break;
                case SDLK_1:
                    cpu.KeyPad[1] = 0;
                    break;
                case SDLK_2:
                    cpu.KeyPad[2] = 0;
                    break;
                case SDLK_3:
                    cpu.KeyPad[3] = 0;
                    break;
                case SDLK_4:
                    cpu.KeyPad[4] = 0;
                    break;
                case SDLK_5:
                    cpu.KeyPad[5] = 0;
                    break;
                case SDLK_6:
                    cpu.KeyPad[6] = 0;
                    break;
                case SDLK_7:
                    cpu.KeyPad[7] = 0;
                    break;
                case SDLK_8:
                    cpu.KeyPad[8] = 0;
                    break;
                case SDLK_9:
                    cpu.KeyPad[9] = 0;
                    break;
                case SDLK_a:
                    cpu.KeyPad[10] = 0;
                    break;
                case SDLK_b:
                    cpu.KeyPad[11] = 0;
                    break;
                case SDLK_c:
                    cpu.KeyPad[12] = 0;
                    break;
                case SDLK_d:
                    cpu.KeyPad[13] = 0;
                    break;
                case SDLK_e:
                    cpu.KeyPad[14] = 0;
                    break;
                case SDLK_f:
                    cpu.KeyPad[15] = 0;
                    break;
                default:
                    break;
                }
                break;
            
            default:
                break;
            }
        }
        if(looptimer = 42)
        {
           draw_graphics(&cpu);
           if(cpu.DT > 0) cpu.DT --;
           if(cpu.ST > 0) cpu.ST --;
           looptimer = 0;
        }
        looptimer ++;
        SDL_Delay(1.428);
    }

    close_graphics();
    return 0;
}
