#pragma once
#include <cstdint>
#include <istream>
#include <filesystem>

uint32_t bswap32(uint32_t v);
void read_or_throw(std::istream& in, void* buf, std::size_t size);
std::filesystem::path make_relative(const std::filesystem::path& path, const std::filesystem::path& base);