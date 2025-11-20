#pragma once

#include <cstdint>
#include <cstddef>
#include <sstream>
#include <iomanip>

template<typename T>
inline T bigToLittle(T big_in) {
    T little_out = 0;
    for (size_t i = 0; i < sizeof(T); i++) {
        ((uint8_t *) &little_out)[i] = ((uint8_t *) &big_in)[sizeof(T) - i - 1];
    }
    return little_out;
}

inline bool bigToLittle(void* data, uint32_t size) {
    if (data == nullptr || size == 0) {
        return false;
    }
    
    uint8_t *ptr = (uint8_t *) data;
    const uint32_t len = size / 2;
    for (uint32_t i = 0; i < len; ++i) {
        std::swap(ptr[i], ptr[size - i - 1]);
    }
    return true;
}

template <typename T, int32_t N=10>
std::string toString(const T& t, uint16_t fill_len = 1)
{
    std::ostringstream ss;
    if (std::is_same<T, std::string>::value) {
        ss << t;
    } else {
        if (N == 16){
            ss << std::setw(fill_len) << std::setfill('0') << std::hex << t;
        } else if (N == 8) {
            ss << std::setw(fill_len) << std::setfill('0') << std::oct << t;
        } else {
            ss << std::setw(fill_len) << std::setfill('0') << std::dec << t;
        }
    }
    return ss.str();
}

uint32_t crcGenerator(const uint8_t *data, uint32_t size);

int32_t convertUint24ToInt32(uint32_t data);
int16_t convertUint32ToInt16(uint32_t data);