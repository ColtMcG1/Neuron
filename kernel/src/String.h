#pragma once

uint64_t Length(const char* const string)
{
    uint64_t length = 0;

    while(string[length] != '\0')
    {
        length++;
    }

    return length;
}
uint64_t Length(const char* const begin, const char* const end)
{
    return end - begin;
}

