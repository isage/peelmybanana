#include "pkh.hpp"
#include "utils.hpp"
#include <fstream>
#include <stdexcept>

PKHIndex::PKHIndex(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Failed to open PKH file");

    uint32_t count_be;
    read_or_throw(in, &count_be, sizeof(count_be));
    uint32_t count = bswap32(count_be);

    nodes.resize(count);
    for (auto& n : nodes) {
        uint32_t r[4];
        read_or_throw(in, r, sizeof(r));
        n.crc = bswap32(r[0]);
        n.offset = bswap32(r[1]);
        n.uncompressed_size = bswap32(r[2]);
        n.compressed_size = bswap32(r[3]);
        by_crc[n.crc] = &n;
    }
}

const PKHNode* PKHIndex::find(uint32_t crc) const {
    auto it = by_crc.find(crc);
    return it == by_crc.end() ? nullptr : it->second;
}
