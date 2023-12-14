#pragma once

#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "adaptor/leveldb_adaptor.h"

namespace wrapperfs {

struct inode_metadata {
    struct stat stat;
    std::string data_id;
};

struct inode_data {
    std::unordered_map<std::string, std::string> map;
};

struct inode {
    inode_metadata metadata;
    inode_data data;
};


bool get_inode(size_t inode_id, inode &inode) {
    std::string json_string;
    try {
        nlohmann::json json_map = nlohmann::json::parse(json_string);
        inode.data.map = json_map.get<std::unordered_map<std::string, std::string>>();
    } catch (const std::exception& e) {
        
        return false;
    }
}

bool put_inode(size_t inode_id, inode inode) {
    nlohmann::json json_map = inode.data.map;
    std::string json_string = json_map.dump();

}

}