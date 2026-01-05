#pragma once
#include <cstdint>
#include <string>

class CRC32 {
public:
//    static uint32_t compute(const char* in, uint32_t cnt);
    static uint32_t compute(const std::string& in);
};
