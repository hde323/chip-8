#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "cpu.h"   // for Cpu struct

#define WINDOW_SCALE 10   // each CHIP-8 pixel is scaled up

int init_graphics();
void draw_graphics(Cpu *cpu);
void close_graphics();

#endif
