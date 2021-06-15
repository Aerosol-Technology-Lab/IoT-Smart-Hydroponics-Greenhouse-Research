
#include "utils.h"
#include <Arduino.h>

unsigned int UtilsDF::nextWord(const char *buffer, 
            unsigned int pos,
            size_t buffer_size, 
            char *out,
            size_t out_size) {
    
    
    unsigned int i = 0;
    for (i = 0;
        pos < buffer_size && i < out_size &&
        buffer[pos] != 0 && buffer[pos] != ' ' && buffer[pos] != '\n';
        ++i, ++pos) {

        out[i] = buffer[pos];
    }

    out[i] = 0;      // sets the end of the string. This will still work if command size is 0
    return i;
}

#ifdef DEBUG

size_t UtilsDF::print(const char *c) {
    return Serial.print(c);
}

size_t UtilsDF::println(const char *c) {
    return Serial.println(c);
}

#else

size_t UtilsDF::print(const char *c) {
    return 0;
}

size_t UtilsDF::println(const char *c) {
    return 0;
}

#endif

unsigned int UtilsDF::readSerial(char *buffer, size_t maxSize, bool nullTerminate) {
    unsigned int bytesRead = 0;
    char *lastByteRead;
    
    do {
        if (Serial.available()) {
            lastByteRead = buffer + bytesRead;
            *lastByteRead = Serial.read();
            ++bytesRead;
        }
    } while(bytesRead < maxSize - 1 && *lastByteRead != 0 && *lastByteRead != '\n' && *lastByteRead != '\r');

    if (nullTerminate && lastByteRead && (*lastByteRead == '\n' || *lastByteRead == '\r')) *lastByteRead = 0;
    else if (bytesRead == maxSize - 1) {
        buffer[maxSize - 1] = 0;
    }
    
    return bytesRead;
}

size_t UtilsDF::sendSerial(const char *cstr) {
    size_t count = 0;
    while(*cstr) {
        Serial.write(*cstr);
        ++cstr;
        ++count;
    }
    Serial.write((byte) 0);
    return count + 1;
}

char * UtilsDF::movePointer(char *ptr, int move) {
    if (move) {
        ptr[move - 1] = ' ';
        return ptr + move;
    }
}

bool UtilsDF::equals(const char *a, const char *b) {
    
    while (true) {
        if (*a != *b) return false;
        else if (*a == 0) return true;
        ++a;
        ++b;
    }
}

template<class T>
void UtilsDF::swap(T&a, T&b)
{
    T tmp = static_cast<T&&>(a);
    a = static_cast<T&&>(b);
    b = static_cast<T&&>(tmp);
}

template<class T>
inline T && UtilsDF::move(T &t)
{
    return static_cast<T&&>(t);
}

void UtilsDF::strupr(char *str)
{
    for (size_t i = 0; i < strlen(str); ++i) {
        str[i] = toupper(str[i]);
    }
}

template<class T>
int UtilsDF::partition(T arr[], int low, int high)
{
    T pivot = arr[high]; // pivot
    int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far
 
    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element
            UtilsDF::swap(arr[i], arr[j]);
        }
    }
    UtilsDF::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

template<class T>
void UtilsDF::quickSort(T arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
