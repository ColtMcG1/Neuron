#include "Bitmap.h"


Bitmap::Bitmap(uint8_t* buffer, size_t size)
    : m_Buffer(buffer), m_Size(size)
{

}
Bitmap::~Bitmap()
{

}

bool Bitmap::Get(uint64_t index)
{
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;

    uint8_t bitIndexer = 0b10000000 >> bitIndex;

    if((m_Buffer[byteIndex] & bitIndexer) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Bitmap::Set(uint64_t index, bool value)
{
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;

    uint8_t bitIndexer = 0b10000000 >> bitIndex;

    m_Buffer[byteIndex] &= ~bitIndexer;

    if(value)
    {
        m_Buffer[byteIndex] |= bitIndexer;
    }
}

const size_t& Bitmap::Size() const
{
    return m_Size;
}