#include <iostream>

#include "wrapper/inode.h"
#include "wrapper/wrapper.h"
#include "adaptor/leveldb_adaptor.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "wrapperfs mkfs error." << std::endl;
        exit(1);
    }
    wrapperfs::LevelDBAdaptor* adaptor = new wrapperfs::LevelDBAdaptor(argv[1]);
    // 创建 ROOT_WRAPPER_ID inode
    // 创建 ROOT_WRAPPER_ID entries
    // 创建 ROOT_WRAPPER_ID location
    return 0;
}