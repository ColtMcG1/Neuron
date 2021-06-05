#pragma once

#include "../std/stdint.h"

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


struct Console
{
    Framebuffer *Buffer;
    PSF1_FONT *Font;
    uint32_t CarageLocation[2] = {0};

    void Initalize(Framebuffer *, PSF1_FONT *);

    void ReturnCarage();
    void AdvanceCarage();

    void Clear(uint32_t);

    void Put(char);
};

extern Console GlobalConsole;

void putc(char);
void print(const char*);
void print(uint64_t);
void print(int64_t);
uint64_t strlen(const char*);
void clearscn(uint32_t);