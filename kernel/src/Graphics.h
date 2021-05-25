#pragma once

#include "std/stdint.h"

struct Framebuffer
{
    void *BaseAddress;
    size_t BufferSize;
    unsigned int Width;
    unsigned int Height;
    unsigned int PixelsPerLine;
};

struct PSF1_HEADER
{
	unsigned char Magic[2];
	unsigned char Mode;
	unsigned char CharSize;
};
struct PSF1_FONT
{
	PSF1_HEADER* psf1_Header;
	void* GlyphBuffer;
};

#define BLACK 0xff000000
#define WHITE 0xffffffff
#define RED 0xffff0000
#define GREEN 0xff00ff00
#define BLUE 0xff0000ff
#define YELLOW 0xffffff00
#define CYAN 0xff00ffff
#define MAGENTA 0xffff00ff

extern Framebuffer* neuron_Global_Framebuffer;
extern PSF1_FONT* neuron_Global_Font;

void InitalizeConsole(Framebuffer *, PSF1_FONT *);

void putc(char);
void print(const char*);
uint64_t strlen(const char*);