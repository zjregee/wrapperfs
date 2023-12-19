#include "fs/wrapperfs.h"

namespace wrapperfs {

wrapperfs::wrapperfs(const std::string &data_dir, const std::string &db_dir) {
    data_dir_ = data_dir;
    adaptor_ = new LevelDBAdaptor(db_dir);
}

int wrapperfs::getattr(const char* path, struct stat* statbuf) {
    return 0;
}

int wrapperfs::mknod(const char* path, mode_t mode, dev_t dev) {
    return 0;
}

int wrapperfs::mkdir(const char* path, mode_t mode) {
    return 0;
}

int wrapperfs::open(const char* path, struct fuse_file_info* file_info) {
    return 0;
}

int wrapperfs::read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* file_info) {
    return 0;
}

int wrapperfs::write(const char* path, const char* buf, size_t size, off_t offset, struct fuse_file_info* file_info) {
    return 0;
}

int wrapperfs::release(const char* path, struct fuse_file_info* file_info) {
    return 0;
}

int wrapperfs::opendir(const char* path, struct fuse_file_info* file_info) {
    return 0;
}

int wrapperfs::readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* file_info) {
    return 0;
}

int wrapperfs::releasedir(const char* path, struct fuse_file_info* file_info) {
    return 0;
}

}