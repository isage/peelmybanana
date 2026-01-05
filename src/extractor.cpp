#include "extractor.hpp"
#include "crc32.hpp"
#include "utils.hpp"
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <cstdio>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

Extractor::Extractor(const fs::path& pk,
                     const PKHIndex& pkh,
                     const PFSTree& pfs,
                     const fs::path& out)
    : pk_file(pk, std::ios::binary), pkh(pkh), pfs(pfs), out_dir(out) {
    if (!pk_file) throw std::runtime_error("Failed to open PK file");
    fs::create_directories(out_dir);
}

void Extractor::run() {
    traverse("", pfs.root());
}

void Extractor::extract_file(const PKHNode& n, const fs::path& out) {

    std::vector<uint8_t> cbuf(n.compressed_size);
    std::vector<uint8_t> ubuf(n.uncompressed_size);
    if (n.compressed_size > 0)
    {
        pk_file.seekg(n.offset);
        read_or_throw(pk_file, cbuf.data(), cbuf.size());

        z_stream z{};
        z.next_in = cbuf.data();
        z.avail_in = cbuf.size();
        z.next_out = ubuf.data();
        z.avail_out = ubuf.size();

        if (inflateInit2(&z, 16 + MAX_WBITS) != Z_OK)
            throw std::runtime_error("inflateInit failed");

        int rc = inflate(&z, Z_FINISH);
        inflateEnd(&z);

        if (rc != Z_STREAM_END)
        {
            std::cerr << "inflate failed for " << out << std::endl;
            return;
            //throw std::runtime_error("inflate failed");
        }
    }
    else
    {
        pk_file.seekg(n.offset);
        read_or_throw(pk_file, ubuf.data(), ubuf.size());
    }
    fs::create_directories(out.parent_path());
    std::ofstream f(out, std::ios::binary);
    f.write(reinterpret_cast<char*>(ubuf.data()), ubuf.size());
    std::cout << "extracted " << out << std::endl;
}

void Extractor::traverse(const fs::path& cur, const PFSNode& node) {
    fs::path dir = cur / pfs.name(node.name_idx);
    fs::create_directories(out_dir / dir);

    for (uint32_t i = 0; i < node.num_dir_childs; ++i)
        traverse(dir, pfs.node(node.first_dir_child + i));

    for (uint32_t i = 0; i < node.num_file_childs; ++i)
    {
        fs::path rel = dir / pfs.file_name(node.first_file_child + i);

        uint32_t crc = CRC32::compute(rel.string());
        if (const PKHNode* n = pkh.find(crc))
            extract_file(*n, out_dir / rel);
        else
            std::cerr << rel << " - can't find crc" << std::endl;
    }
}
