#pragma once

#include <string>

#include "leveldb/db.h"
#include "leveldb/options.h"

namespace wrapperfs {

class LevelDBAdaptor {
public:
    LevelDBAdaptor(const std::string &database_name);
    ~LevelDBAdaptor();
    bool Insert(const std::string &key, const std::string &value);
    bool GetValue(const std::string &key, std::string &value);
    bool Remove(const std::string &key);

private:
    leveldb::DB* db_;
};

}