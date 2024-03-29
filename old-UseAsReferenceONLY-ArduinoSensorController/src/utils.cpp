
#include "utils.h"
#include <Arduino.h>
#include <Stream.h>
#include <ArduinoJson.h>
#include <alloca.h>

#ifdef DUE

    // void * operator new (size_t size, void * ptr) { return ptr; }
    
#endif

unsigned int Utils::nextWord(const char *buffer, 
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

size_t Utils::print(const char *c) {
    return Serial.print(c);
}

size_t Utils::println(const char *c) {
    return Serial.println(c);
}

#else

size_t Utils::print(const char *c) {
    return 0;
}

size_t Utils::println(const char *c) {
    return 0;
}

#endif

unsigned int Utils::readSerial(char *buffer, size_t maxSize, bool nullTerminate) {
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

bool readJSON(JsonDocument &json, Stream &serial, size_t bufferSize)
{
    char *buffer = (char*) (alloca(sizeof(char) * bufferSize));
    bool success = false;

    for (size_t i = 0; i < bufferSize; ++i) {
        // wait for serial avaialble
        while(!serial.available());
        buffer[i] = serial.read();

        if (buffer[i] == 0) {
            success = true;
            break;
        }
    }

    // build json
    deserializeJson(json, buffer);
    return success;
}

size_t Utils::sendSerial(const char *cstr) {
    size_t count = 0;
    while(*cstr) {
        Serial.write(*cstr);
        ++cstr;
        ++count;
    }
    Serial.write((byte) 0);
    return count + 1;
}

char * Utils::movePointer(char *ptr, int move) {
    if (move) {
        ptr[move - 1] = ' ';
        return ptr + move;
    }
}

bool Utils::equals(const char *a, const char *b) {
    
    while (true) {
        if (*a != *b) return false;
        else if (*a == 0) return true;
        ++a;
        ++b;
    }
}

void Utils::strupr(char *str)
{
    for (size_t i = 0; i < strlen(str); ++i) {
        str[i] = toupper(str[i]);
    }
}
