#pragma once
#include <cstdint>
#include <filesystem>
#include <unordered_map>
#include <vector>

struct PKHNode {
    uint32_t crc;
    uint32_t offset;
    uint32_t uncompressed_size;
    uint32_t compressed_size;
};

class PKHIndex {
public:
    explicit PKHIndex(const std::filesystem::path& path);
    const PKHNode* find(uint32_t crc) const;

private:
    std::vector<PKHNode> nodes;
    std::unordered_map<uint32_t, PKHNode*> by_crc;
};
