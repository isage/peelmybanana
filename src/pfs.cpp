#include "pfs.hpp"
#include "utils.hpp"
#include <fstream>
#include <stdexcept>

PFSTree::PFSTree() {
}

PFSTree::PFSTree(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Failed to open PFS file");

    uint32_t hdr[4];
    read_or_throw(in, hdr, sizeof(hdr));

    node_count = bswap32(hdr[2]);
    file_count = bswap32(hdr[3]);
    total_count = node_count + file_count;

    nodes.resize(node_count);
    for (auto& n : nodes) {
        uint32_t r[6];
        read_or_throw(in, r, sizeof(r));
        n.name_idx = bswap32(r[0]);
        n.parent = bswap32(r[1]);
        n.first_dir_child = bswap32(r[2]);
        n.num_dir_childs = bswap32(r[3]);
        n.first_file_child = bswap32(r[4]);
        n.num_file_childs = bswap32(r[5]);
    }

    string_offsets.resize(total_count);
    for (auto& o : string_offsets) {
        uint32_t r;
        read_or_throw(in, &r, sizeof(r));
        o = bswap32(r);
    }

    strings.assign(std::istreambuf_iterator<char>(in),
                   std::istreambuf_iterator<char>());
}

const PFSNode& PFSTree::root() const { return nodes[0]; }
const PFSNode& PFSTree::node(uint32_t i) const { return nodes.at(i); }

std::string_view PFSTree::name(uint32_t idx) const {
    return std::string_view(strings.data() + string_offsets.at(idx));
}

std::string_view PFSTree::file_name(uint32_t idx) const {
    return std::string_view(strings.data() + string_offsets.at(nodes.size() + idx));
}

void PFSTree::addNode(const PFSNode& node)
{
    nodes.push_back(node);
}

int PFSTree::save(const std::filesystem::path& path) {
    return 0;
}