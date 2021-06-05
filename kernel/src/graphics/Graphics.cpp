#include "Graphics.h"

Console GlobalConsole;

void Console::Initalize(Framebuffer *buffer, PSF1_FONT *font)
{
    Buffer = buffer;
    Font = font;
}

void Console::Put(char character)
{

    if (character == '\n')
    {
        ReturnCarage();
        return;
    }

    unsigned int *pixel = (unsigned int *)Buffer->BaseAddress;
    char *font = (char *)Font->GlyphBuffer + (character * Font->psf1_Header->CharSize);

    for (unsigned int y = CarageLocation[1]; y < CarageLocation[1] + 16; y++)
    {
        for (unsigned int x = CarageLocation[0]; x < CarageLocation[0] + 8; x++)
        {
            if ((*font & (0b100000000 >> (x - CarageLocation[0]))) > 0) //Is this pixel in the glyph lit?
            {
                *(unsigned int *)(pixel + x + (y * Buffer->PixelsPerLine)) = WHITE;
            }
        }
        font++;
    }

    AdvanceCarage();
}
void Console::Clear(uint32_t color)
{
    for(uint32_t y = 0; y < Buffer->Height; y++)
    {
        for (uint32_t x = 0; x < Buffer->PixelsPerLine; x++)
        {
            *((unsigned int *)Buffer->BaseAddress + x + (y * Buffer->PixelsPerLine)) = color;
        }
    }

    CarageLocation[0] = 0;
    CarageLocation[1] = 0;
}

void Console::ReturnCarage()
{
    CarageLocation[0] = 0;
    CarageLocation[1] += 16;
}

void Console::AdvanceCarage()
{
    CarageLocation[0] += 8;

    if (CarageLocation[0] > Buffer->PixelsPerLine)
    {
        ReturnCarage();
    }
}

void putc(char character)
{
    GlobalConsole.Put(character);
}
void print(const char* string)
{
    while(*string)
    {
        putc(*string);
        ++string;
    }
}
void print(uint64_t integer)
{
    char buffer[32];
    unsigned int place = 0;

    for (; integer > 0; integer /= 10, place++)
    {
        unsigned int result = integer % 10;
        char character = (char)('0' + result);
        buffer[place] = character;
    }

    do
    {
        --place;
        putc(buffer[place]);
    } while (place > 0);
}
void print(int64_t integer)
{
    char buffer[32];
    unsigned int place = 0;

    if(integer < 0)
    {
        -integer;
        putc('-');
    }

    for( ;integer > 0; integer /= 10, place++)
    {
        unsigned int result = integer % 10;
        char character = (char)('0' + result);
        buffer[place] = character;
    }

    do
    {
        --place;
        putc(buffer[place]);
    }
    while(place > 0);
}

uint64_t strlen(const char *string)
{
    uint64_t length = 0;

    while (string[length])
    {
        length++;
    }

    return length;
}

void clearscn(uint32_t color)
{
    GlobalConsole.Clear(color);
}