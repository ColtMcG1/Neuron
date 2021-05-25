#include "Graphics.h"

Framebuffer* neuron_Global_Framebuffer;
PSF1_FONT* neuron_Global_Font;

void InitalizeConsole(Framebuffer* framebuffer, PSF1_FONT *font)
{
    neuron_Global_Framebuffer = framebuffer;
    neuron_Global_Font = font;
}

void ReturnCarage(unsigned long& x, unsigned long& y)
{
    x = 0;
    y += 16;
}

void AdvanceCarage(unsigned long& x, unsigned long& y)
{
    x += 8;

    if (x > neuron_Global_Framebuffer->PixelsPerLine)
    {
        ReturnCarage(x, y);
    }
}

void putc(char character)
{

    static unsigned long m_CarageLocation[2] = { 0 };

    if(character == '\n')
    {
        ReturnCarage(m_CarageLocation[0], m_CarageLocation[1]);
        return;
    }

    unsigned int *pixel = (unsigned int *)neuron_Global_Framebuffer->BaseAddress;
    char *font = (char *)neuron_Global_Font->GlyphBuffer + (character * neuron_Global_Font->psf1_Header->CharSize);

    for(unsigned int y = m_CarageLocation[1]; y < m_CarageLocation[1] + 16; y++)
    {
        for(unsigned int x = m_CarageLocation[0]; x < m_CarageLocation[0] + 8; x++)
        {
            if ((*font & (0b100000000 >> (x - m_CarageLocation[0]))) > 0) //Is this pixel in the glyph lit?
            {
                *(unsigned int *)(pixel + x + (y * neuron_Global_Framebuffer->PixelsPerLine)) = WHITE;
            }
        }
        font++;
    }

    AdvanceCarage(m_CarageLocation[0], m_CarageLocation[1]);
}
void print(const char* string)
{
    while(*string)
    {
        putc(*string);
        ++string;
    }
}

uint64_t strlen(const char* string)
{
    uint64_t length = 0;

    while(string[length])
    {
        length++;
    }

    return length;
}
void PrintInt(unsigned long long integer, unsigned int color)
{
    char buffer[32];
    unsigned int place = 0;

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