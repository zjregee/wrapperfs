#include "adaptor/leveldb_adaptor.h"

namespace wrapperfs {

LevelDBAdaptor::LevelDBAdaptor(const std::string &database_name) {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, database_name, &db_);
    if (!status.ok()) {
        perror("open database error");
        exit(1);
    }
}

LevelDBAdaptor::~LevelDBAdaptor() {
    delete db_;
}

bool LevelDBAdaptor::Insert(const std::string &key, const std::string &value) {
    leveldb::WriteOptions write_options;
    write_options.sync = true;
    leveldb::Status status = db_->Put(write_options, key, value);
    return status.ok();
}

bool LevelDBAdaptor::GetValue(const std::string &key, std::string &value) {
    leveldb::ReadOptions read_options;
    leveldb::Status status = db_->Get(read_options, key, &value);
    return status.ok();
}

bool LevelDBAdaptor::Remove(const std::string &key) {
    leveldb::WriteOptions write_options;
    write_options.sync = true;
    leveldb::Status status = db_->Delete(write_options, key);
    return status.ok();
}

}