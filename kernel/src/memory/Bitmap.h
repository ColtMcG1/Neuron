#pragma once

#include "../std/stdint.h"

class Bitmap
{

public:
    Bitmap(uint8_t* buffer, size_t size);
    ~Bitmap();

    void Set(uint64_t index, bool value);
    bool Get(uint64_t index);

    const size_t& Size() const;

private:
    size_t m_Size;
    uint8_t* m_Buffer;

};