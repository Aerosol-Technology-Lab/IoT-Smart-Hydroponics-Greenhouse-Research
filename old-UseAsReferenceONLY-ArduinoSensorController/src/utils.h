#pragma once

#include <Arduino.h>
#include <Stream.h>
#include <ArduinoJson.h>

#ifdef DUE
    #define RESOLUTION_BITS                   12
    #define VREF                            3.3f
    #define VREF_MILLI          (VREF * 1000.0f)
    #define ANALOG_RESOLUTION            4095.0f

    /**
     * @brief
     * 
     * @param size 
     * @param ptr 
     * @return void* 
     */
    // void * operator new (size_t size, void * ptr);
    // inline void* operator new (std::size_t n, void* ptr) {return ptr;};

#elif UNO
    #define RESOLUTION_BITS                   10
    #define VREF                            5.0f
    #define VREF_MILLI          (VREF * 1000.0f)
    #define ANALOG_RESOLUTION            1023.0f
#else
    #define RESOLUTION_BITS                   10
    #define VREF                            5.0f
    #define VREF_MILLI          (VREF * 1000.0f)
    #define ANALOG_RESOLUTION            1023.0f
#endif

#ifndef DUE
    #pragma message "WARNING! This code is not tested on other boards besides the Arduino Due"
#endif

// #define DEBUG
#ifdef DEBUG
    #pragma message "DEBUG mode is enabled!"
    #define dev_print(x) Serial.print(x)
    #define dev_println(x) Serial.println(x)
#else
    #define dev_print(x)
    #define dev_println(x)
#endif

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

    bool readJSON(JsonObject &json, Stream &stream, size_t bufferSize=1024);

    size_t sendSerial(const char *cstring);
    
    char * movePointer(char *ptr, int move);
        
    /**
     * @deprecated
     * This does not even work
     */
    bool equals(const char *a, const char *b);

    template<class T>

    void swap(T&a, T&b);
    
    template<class T>
    T && move(T &t);

    /**
     * @brief Capitlaizes every character in the c-string
     * 
     */
    void strupr(char *);
    
    /**
     * @brief parition helper for merge sort
     * @citation https://www.geeksforgeeks.org/quick-sort/
     * 
     * @param arr array to sort
     * @param low low index
     * @param high high index
     * @return int location of new partition
     */
    template<class T>
    int partition(T arr[], int low, int high);

    /**
     * @brief Performs quicksort on arr
     * 
     * @tparam T type with comparison operators
     * @param arr array to sort
     * @param l starting index to sort
     * @param r ending index to sort. NOTE - this is the index, not size
     */
    template<class T>
    void quickSort(T arr[],int l,int r);
}

#include "utils_impl.h"
