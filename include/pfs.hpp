#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

struct PFSNode {
    uint32_t name_idx;
    uint32_t parent;
    uint32_t first_dir_child;
    uint32_t num_dir_childs;
    uint32_t first_file_child;
    uint32_t num_file_childs;
};

class PFSTree {
public:
    PFSTree();
    explicit PFSTree(const std::filesystem::path& path);

    const PFSNode& root() const;
    const PFSNode& node(uint32_t i) const;
    std::string_view name(uint32_t idx) const;
    std::string_view file_name(uint32_t idx) const;
    int save(const std::filesystem::path& path);
    void addNode(const PFSNode& node);

private:
    uint32_t node_count{}, file_count{}, total_count{};
    std::vector<PFSNode> nodes;
    std::vector<uint32_t> string_offsets;
    std::vector<std::string> names;
    std::string strings;
};
