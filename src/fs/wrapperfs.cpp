#include "fs/wrapperfs.h"

namespace wrapperfs {

wrapperfs::wrapperfs(const std::string &data_dir, const std::string &db_dir) {
    data_dir_ = data_dir;
    adaptor_ = new LevelDBAdaptor(db_dir);
}

}