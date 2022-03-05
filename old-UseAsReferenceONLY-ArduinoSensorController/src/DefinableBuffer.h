#pragma once

#include "StaticBuffer.h"
#include "utils.h"
#include <stdlib.h>
#include "stdint.h"

/**
 * @brief Creates a buffer object for pointer allocation. The intention is to create
 * this class in the stack at the beginning in an Arduino program. This provides a source
 * for managing and creating pointer objects that lives in the stack, whcih will never be
 * delete throughout the life of the program.
 * 
 * @tparam SIZE size of buffer
 */
template<size_t SIZE>
class DefinableBuffer : public StaticBuffer
{
public:
    /**
     * @brief global stack buffer
     */
    static DefinableBuffer *global;

private:
    char buffer[SIZE];
    void * startAddress;
    size_t ptr;
    
public:
    /**
     * @brief Construct a new Static Buffer object
     * 
     * @param clean assigns the flash with 0x00 for every byte in the buffer
     */
    DefinableBuffer(bool _clean=false) : ptr(0)
    {
        startAddress = reinterpret_cast<void*>(&buffer);
        if (_clean) clean();
    }

    void clean()
    {
        memset(startAddress, 0, SIZE);
    }

    template<class T>
    T * talloc(size_t size=1)
    {
        return alloc(size, sizeof(T));
    }

    void * alloc(size_t size, size_t typeSize = sizeof(char))
    {
        size_t requestSize = size * typeSize;
        if (ptr + requestSize > SIZE) return nullptr;

        void *ret = reinterpret_cast<void*>(startAddress + ptr);
        ptr += requestSize;
        
        return ret;
    }
};
