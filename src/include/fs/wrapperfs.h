#pragma once

#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <sstream>
#include <spdlog/spdlog.h>

#include "wrapper/inode.h"
#include "wrapper/wrapper.h"
#include "utils/hash_routine.h"
#include "utils/string_routine.h"
#include "utils/filesystem_routine.h"
#include "adaptor/leveldb_adaptor.h"

namespace wrapperfs {

class wrapperfs {
public:
    wrapperfs(const std::string &data_dir, const std::string &db_dir);
    int getattr(const char* path, struct stat* statbuf);
    int mknod(const char* path, mode_t mode, dev_t dev);
    int mkdir(const char* path, mode_t mode);
    int open(const char* path, struct fuse_file_info* file_info);
    int read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* file_info);
    int write(const char* path, const char* buf, size_t size, off_t offset, struct fuse_file_info* file_info);
    int release(const char* path, struct fuse_file_info* file_info);
    int opendir(const char* path, struct fuse_file_info* file_info);
    int readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* file_info);
    int releasedir(const char* path, struct fuse_file_info* file_info);

private:
    size_t max_ino;
    std::string data_dir_;
    LevelDBAdaptor* adaptor_;

    void get_file_path(size_t ino, std::string &path);
};

}