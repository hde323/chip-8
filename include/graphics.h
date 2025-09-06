#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "cpu.h"

#define WINDOW_SCALE 10

int init_graphics();
void draw_graphics(Cpu *cpu);
void close_graphics();

#endif
