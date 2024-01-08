#pragma once

#include <string>
#include <sstream>
#include <sys/stat.h>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "common/config.h"
#include "adaptor/leveldb_adaptor.h"

namespace wrapperfs {

struct inode_metadata_t {
    struct stat stat;

    std::string debug() {
        std::stringstream s;
        s << "inode_id - " << stat.st_ino;
        return s.str();
    }
};

struct inode_data_t {
    std::unordered_map<std::string, std::string> map;

    std::string debug() {
        std::stringstream s;
        for (auto &kv : map) {
            s << kv.first << " - " << kv.second;
        }
        return s.str();
    }
};

struct inode_t {
    inode_metadata_t* metadata;
    inode_data_t* data;

    std::string debug() {
        std::stringstream s;
        s << "inode_id - " << metadata->stat.st_ino;
        for (auto &kv : data->map) {
            s << " " << kv.first << " - " << kv.second;
        }
        return s.str();
    }
};

bool get_inode_metadata(LevelDBAdaptor* adaptor, size_t inode_id, inode_metadata_t* &inode_metadata);
bool put_inode_metadata(LevelDBAdaptor* adaptor, size_t inode_id, inode_metadata_t* &inode_metadata);
bool get_inode_data(LevelDBAdaptor* adaptor, size_t inode_id, inode_data_t* &inode_data);
bool put_inode_data(LevelDBAdaptor* adaptor, size_t inode_id, inode_data_t* &inode_data);
bool get_inode(LevelDBAdaptor* adaptor, size_t inode_id, inode_t* &inode);
bool put_inode(LevelDBAdaptor* adaptor, size_t inode_id, inode_t* inode);

}