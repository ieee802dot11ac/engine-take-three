#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "bases/obj.hpp"

class IStream {
public:
    virtual ~IStream() = 0;
    virtual void ReadImpl(size_t size, void* buf) = 0;
    virtual void WriteImpl(size_t size, const void* buf) = 0;

    #define BASIC_REED(T) IStream& operator>>(T& in) { ReadImpl(sizeof(T), &in); return *this;}
    #define BASIC_WRIT(T) IStream& operator<<(const T& out) { WriteImpl(sizeof(T), &out); return *this;}

    BASIC_REED(char) // thanks gnu stdlib
    BASIC_REED(int8_t)
    BASIC_REED(int16_t)
    BASIC_REED(int32_t)
    BASIC_REED(int64_t)
    BASIC_REED(uint8_t)
    BASIC_REED(uint16_t)
    BASIC_REED(uint32_t)
    BASIC_REED(uint64_t)

    BASIC_WRIT(char)
    BASIC_WRIT(int8_t)
    BASIC_WRIT(int16_t)
    BASIC_WRIT(int32_t)
    BASIC_WRIT(int64_t)
    BASIC_WRIT(uint8_t)
    BASIC_WRIT(uint16_t)
    BASIC_WRIT(uint32_t)
    BASIC_WRIT(uint64_t)

    #undef BASIC_REED
    #undef BASIC_WRIT

    IStream& operator<<(const std::string& in) {
        *this << in.size();
        for (const char& c : in) {
            *this << c;
        }
        return *this;
    }

    IStream& operator>>(std::string& out) {
        int size;
        *this >> size;
        out.resize(size);
        for (char& c : out) {
            *this >> c;
        }
        return *this;
    }

    template <typename T>
    IStream& operator<<(const T* in) requires IsObj<T> {
        in->Save(*this);
        return *this;
    }

    template <typename T>
    IStream& operator>>(T* out) requires IsObj<T> {
        out->Load(*this);
        return *this;
    }

    template <typename T>
    IStream& operator<<(const std::vector<T*>& in) {
        *this << in.size();
        for (const T* c : in) {
            *this << c;
        }
        return *this;
    }

    template <typename T>
    IStream& operator>>(std::vector<T*>& out) {
        int size;
        *this >> size;
        out.resize(size);
        for (T* c : out) {
            *this >> c;
        }
        return *this;
    }
};
