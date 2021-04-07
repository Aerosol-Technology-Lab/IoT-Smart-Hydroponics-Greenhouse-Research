#pragma once

#include "utils.h"
#include <stdlib.h>
#include "stdint.h"

/**
 * @brief Creates a buffer object for pointer allocation. The intention is to create
 * this class in the stack at the beginning in an Arduino program. This provides a source
 * for managing and creating pointer objects that lives in the stack, whcih will never be
 * delete throughout the life of the program.
 * 
 */
class StaticBuffer
{
public:
    /**
     * @brief global stack buffer
     */
    static StaticBuffer *global;

public:

    virtual void clean();

    template<class T> T * talloc(size_t size=1);

    void * alloc(size_t size, size_t typeSize = sizeof(char));
};

StaticBuffer * StaticBuffer::global = nullptr;
