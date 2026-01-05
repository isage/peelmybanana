#include "utils.hpp"
#include <stdexcept>

uint32_t bswap32(uint32_t v) {
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) << 8)  |
           ((v & 0x00FF0000u) >> 8)  |
           ((v & 0xFF000000u) >> 24);
}

void read_or_throw(std::istream& in, void* buf, std::size_t size) {
    if (!in.read(reinterpret_cast<char*>(buf), size)) {
        throw std::runtime_error("Unexpected EOF");
    }
}

std::filesystem::path make_relative(const std::filesystem::path& path, const std::filesystem::path& base) {
    auto path_it = path.begin();
    auto base_it = base.begin();

    while (path_it != path.end() && base_it != base.end() && *path_it == *base_it) {
        ++path_it;
        ++base_it;
    }

    std::filesystem::path relative_p;

    while (path_it != path.end()) {
        relative_p /= *path_it;
        ++path_it;
    }

    return relative_p;
}
