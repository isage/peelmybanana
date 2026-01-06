#pragma once
#include "pfs.hpp"
#include "pkh.hpp"
#include <filesystem>
#include <fstream>

class Extractor {
public:
    Extractor(const std::filesystem::path& pk,
              const PKHIndex& pkh,
              const PFSTree& pfs,
              const std::filesystem::path& out, const bool iavt);

    void run();

private:
    void traverse(const std::filesystem::path& cur, const PFSNode& node);
    void extract_file(const PKHNode& n, const std::filesystem::path& out);

    std::ifstream pk_file;
    const PKHIndex& pkh;
    const PFSTree& pfs;
    std::filesystem::path out_dir;
    bool iavt;
};
