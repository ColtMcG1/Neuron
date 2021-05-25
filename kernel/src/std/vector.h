#pragma once

#include "new.h"

template<typename T>
class vector_iterator
{

public:
    explicit vector_iterator(T* begin, T* end)
        : m_Begin(begin), m_Index(begin), m_End(end)
    {
        ;
    }
    ~vector_iterator()
    {
        ;
    }

    inline T &next()
    {
        ++m_Index;
        return (*m_Index);
    }
    inline T &previous()
    {
        --m_Index;
        return (*m_Index);
    }
    inline T &current()
    {
        return (*m_Index);
    }

    T *begin()
    {
        return m_Begin;
    }
    T *end()
    {
        return m_End;
    }
    const T *begin() const
    {
        return m_Begin;
    }
    const T *end() const
    {
        return m_End;
    }

    inline const uint64_t Size() const
    {
        return m_End - m_Begin;
    }

private:
    T *m_Begin;
    T *m_End;
    T *m_Index;
};

template<typename T>
class vector
{

public:
    inline vector() 
        : m_Begin(), m_End(), m_Index() 
    {
        Initalize(16);
    }
    inline ~vector();

private:
    inline void Initalize(uint64_t elements) 
    {
        if(m_Begin)
        {
            return;
        }

        m_Begin = new T[elements];
        m_Index = m_Begin;
        m_End = m_Begin + elements;

        UpdateIterator();
    }
    inline void ChangeSize(uint64_t newSize)
    {
        if(!m_Begin)
            return Initalize();

        uint64_t currentSize = m_End - m_Begin;
        uint64_t currentElements = m_Index - m_Begin;

        T *t_Begin = new T[newSize];

    }
    inline void Destroy()
    {
        if(!m_Begin)
        {
            return;
        }

        delete m_Begin;

        m_Begin = nullptr;
        m_Index = nullptr;
        m_End   = nullptr;
    }

    inline void UpdateIterator()
    {
        m_Iterator = vector_iterator(m_Begin, m_Index);
    }

private:
    T *m_Begin;
    T *m_End;
    T *m_Index;

    vector_iterator m_Iterator;
};