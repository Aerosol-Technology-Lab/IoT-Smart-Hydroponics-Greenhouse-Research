#pragma once

#include <Arduino.h>
// #define DEBUG

namespace Utils {

    /**
     * Copies the next word from the buffer to the output string
     * NOTE: Because the null terminator is included in out_size,
     *       the maximum number of characters than can be written
     *       to 'out' is out_size - 1
     * 
     * @param buffer: Source of string
     * @param pos: starting position to copy from buffer
     * @param buffer_size: size of buffer (including NULL terminator)
     * @param out: The next word from buffer will be stored in out
     * @param out_size: size of out (including NULL terminator)
     * @return the number of bytes transfered from buffer to out
     */
    unsigned int nextWord(const char *buffer, 
                unsigned int pos,
                size_t buffer_size, 
                char *out,
                size_t out_size);

    size_t print(const char *c);

    size_t println(const char *c);
    
    /**
     * Reads a string from the serial until the string ends with 0, newline, or carriage return
     * NOTE: If the string read from UART is less than maxSize and is not terminated with NULL,
     *       newline, or carriage return, then this function blocks the whole program.
     * 
     * @param buffer: buffer to write contents from Serial.read()
     * @param maxSize: maximum size of buffer
     * @param nullTerminate: (Default true) Replaces carriage return or newline character with NULL
     *                       If false, the carriage return or newline stays as is
     * @return the number of bytes read, including NULL, newline, or carriage return if present or
     *         added to the end of the cstring
     */
    unsigned int readSerial(char *buffer, size_t maxSize, bool nullTerminate = true);

    size_t sendSerial(const char *cstring);
    
    /**
     * @deprecated
     */
    bool equals(const char *a, const char *b);
}
