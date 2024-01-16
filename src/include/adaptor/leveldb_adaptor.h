#pragma once

#include <string>
#include <vector>
#include <utility>
#include <leveldb/db.h>
#include <leveldb/cache.h>
#include <leveldb/options.h>
#include <leveldb/iterator.h>
#include <spdlog/spdlog.h>

namespace wrapperfs {

class LevelDBAdaptor {
public:
    LevelDBAdaptor(const std::string &database_name);
    ~LevelDBAdaptor();
    bool Insert(const std::string &key, const std::string &value);
    bool GetValue(const std::string &key, std::string &value);
    bool Remove(const std::string &key);
    bool GetRange(const std::string &start_key, const std::string &end_key, std::vector<std::pair<std::string, std::string>> &key_value_pair_list);

private:
    leveldb::DB* db_;
};

}