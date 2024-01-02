#pragma once

#include <vector>
#include <string>
#include <utility>

#include "adaptor/leveldb_adaptor.h"

namespace wrapperfs {

enum wrapper_tag {
    directory_relation,
};

// 点查询
struct entries_t {
    wrapper_tag tag;
    size_t wrapper_id;
    std::vector<std::pair<size_t, std::string>> list;
};

// 范围查询
struct relation_t {
    wrapper_tag tag;
    size_t wrapper_id;
    std::string distance;
    size_t next_wrapper_id;
};

// 目录树适配
struct location_t {
    wrapper_tag tag;
    size_t wrapper_id;
    size_t ino;
};

bool get_entries(LevelDBAdaptor* adaptor, entries_t* &entries);
bool put_entries(LevelDBAdaptor* adaptor, entries_t* entries);
bool get_relation(LevelDBAdaptor* adaptor, relation_t* &relation);
bool put_relation(LevelDBAdaptor* adaptor, relation_t* relation);
bool get_location(LevelDBAdaptor* adaptor, location_t* &location);
bool put_location(LevelDBAdaptor* adaptor, location_t* location);

}