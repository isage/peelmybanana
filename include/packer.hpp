#pragma once
#include "pfs.hpp"
#include "pkh.hpp"
#include <filesystem>
#include <fstream>
#include <utility>

class Packer {
public:
    Packer(const std::filesystem::path& in);

    void run();
    std::pair<size_t, size_t> compress_file(const std::filesystem::path& inputPath, std::ofstream& output);

private:
    std::filesystem::path in_dir;
};
