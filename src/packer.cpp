#include "packer.hpp"
#include "crc32.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <zlib.h>

namespace fs = std::filesystem;

static std::time_t to_time_t(std::filesystem::file_time_type ft) {
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(
        ft - std::filesystem::file_time_type::clock::now()
        + system_clock::now()
    );
    return system_clock::to_time_t(sctp);
}

Packer::Packer(const fs::path& in, bool iavt): in_dir(in), iavt(iavt)
{
}

std::pair<size_t, size_t> Packer::compress_file(const fs::path& inputPath, std::ofstream& output)
{
std::ifstream input(inputPath, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Failed to open input file");
    }

    std::vector<char> original(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>()
    );

    const size_t originalSize = original.size();

    std::time_t mtime = 0;
    try {
        mtime = to_time_t(std::filesystem::last_write_time(inputPath));
    } catch (...) {
    }

    z_stream strm{};
    if (iavt)
    {
        if (deflateInit2(&strm,
                     Z_BEST_COMPRESSION,
                     Z_DEFLATED,
                     15 + 16, // gzip
                     8,
                     Z_DEFAULT_STRATEGY) != Z_OK) {
            throw std::runtime_error("deflateInit2 failed");
        }
    }
    else
    {
        if (deflateInit2(&strm,
                     Z_DEFAULT_COMPRESSION,
                     Z_DEFLATED,
                     15, // zlib
                     8,
                     Z_DEFAULT_STRATEGY) != Z_OK) {
            throw std::runtime_error("deflateInit2 failed");
        }
    }

    if (!iavt)
    {
        gz_header header{};
        std::memset(&header, 0, sizeof(header));

        std::string fname = inputPath.filename().string();
        header.name     = reinterpret_cast<Bytef*>(fname.data());
        header.name_max = fname.size() + 1;
        header.time     = static_cast<uLong>(mtime);
        header.os       = 11; // Windows / NTFS

        if (deflateSetHeader(&strm, &header) != Z_OK) {
            deflateEnd(&strm);
            throw std::runtime_error("deflateSetHeader failed");
        }
    }

    std::vector<char> compressed;
    compressed.resize(compressBound(originalSize) + 64);

    strm.next_in   = reinterpret_cast<Bytef*>(original.data());
    strm.avail_in  = originalSize;
    strm.next_out  = reinterpret_cast<Bytef*>(compressed.data());
    strm.avail_out = compressed.size();

    int ret = deflate(&strm, Z_FINISH);

    if (ret != Z_STREAM_END) {
        deflateEnd(&strm);
        throw std::runtime_error(std::string("Compression failed: ") + std::to_string(ret));
    }

    compressed.resize(strm.total_out);
    deflateEnd(&strm);

    if (compressed.size() <= originalSize) {
        output.write(compressed.data(), compressed.size());
        return { compressed.size(), originalSize };
    } else {
        output.write(original.data(), originalSize);
        return { 0, originalSize };
    }
}

void Packer::run() {
  std::cout << "iterating" << std::endl;
  PFSTree tree;
  std::vector<fs::directory_entry> entries;

  for (const auto& dirEntry : fs::recursive_directory_iterator(in_dir))
  {
     entries.push_back(dirEntry);
  }

  std::sort(entries.begin(), entries.end(), 
    [](const fs::directory_entry& a,
      const fs::directory_entry& b) {
      return a.path().string()
        < b.path().string();
  });

  fs::path outputPath = in_dir;
  outputPath = outputPath.replace_extension(".pk").filename();
  std::ofstream pkoutput(outputPath, std::ios::binary | std::ios::app);

  std::map<uint32_t, PKHNode> pkh_nodes;

  for(const auto& dirEntry: entries)
  {
     fs::path rel = make_relative(dirEntry, in_dir);

     if (fs::is_regular_file(dirEntry))
     {
         uint32_t crc = CRC32::compute(rel.string());
         std::cout << "Packing " << rel << std::endl;
         // gzip and append to pk
         auto [compressedSize, originalSize] = compress_file(dirEntry, pkoutput);
         // create pkh node
         PKHNode node;
         node.offset = bswap32(pkoutput.tellp());
         node.crc = bswap32(crc);
         node.compressed_size = bswap32(compressedSize);
         node.uncompressed_size = bswap32(originalSize);
         pkh_nodes.insert({crc, node});
     }

  }

  fs::path pkhoutputPath = in_dir;
  pkhoutputPath = pkhoutputPath.replace_extension(".pkh").filename();

  std::ofstream pkhoutput(pkhoutputPath, std::ios::binary | std::ios::app);
  uint32_t pkh_num = bswap32(pkh_nodes.size());
  pkhoutput.write(reinterpret_cast<const char*>(&pkh_num), sizeof(uint32_t));
  for (auto it: pkh_nodes)
  {
    pkhoutput.write(reinterpret_cast<const char*>(&it.second), sizeof(PKHNode));
  }

}

