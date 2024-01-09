#include "fs/wrapperfs.h"

namespace wrapperfs {

wrapperfs::wrapperfs(const std::string &data_dir, const std::string &db_dir) {
    max_ino = 0;
    data_dir_ = data_dir;
    adaptor_ = new LevelDBAdaptor(db_dir);
}

int wrapperfs::getattr(const char* path, struct stat* statbuf) {
    std::string path_string = path;
    if (path_string.empty() || path_string.front() != *PATH_DELIMITER) {
        if (ENABELD_LOG) {
            spdlog::warn("getattr: return failed");
        }
        return -ENOENT;
    }
    std::vector<std::string> path_items = split_string(path_string, PATH_DELIMITER);
    size_t wrapper_id_in_search = ROOT_WRAPPER_ID;
    if (path_items.size() < 2) {
        if (path_items.size() == 0) {
            location_t* location = new location_t;
            location->tag = directory_relation;
            location->wrapper_id = wrapper_id_in_search;
            if (!get_location(adaptor_, location)) {
                if (ENABELD_LOG) {
                    spdlog::warn("getattr: return failed");
                }
                delete location;
                return -ENOENT;
            } else {
                std::memcpy(statbuf, &location->stat, sizeof(struct stat));
                delete location;
                return 0;
            }
        }
    } else {
        int index = -1;
        while (index != path_items.size() - 2) {
            relation_t* relation = new relation_t;
            relation->tag = directory_relation;
            relation->wrapper_id = wrapper_id_in_search;
            relation->distance = path_items[index + 1];
            if (!get_relation(adaptor_, relation)) {
                break;
            } else {
                index += 1;
                wrapper_id_in_search  = relation->next_wrapper_id;
            }
        }
        if (index != path_items.size() - 2) {
            if (ENABELD_LOG) {
                spdlog::warn("getattr: return failed");
            }
            return -ENOENT;
        }
    }
    relation_t* relation = new relation_t;
    relation->tag = directory_relation;
    relation->wrapper_id = wrapper_id_in_search;
    relation->distance = path_items[path_items.size() - 1];
    if (!get_relation(adaptor_, relation)) {
        entries_t* entries = new entries_t;
        entries->tag = directory_relation;
        entries->wrapper_id = wrapper_id_in_search;
        if (!get_entries(adaptor_, entries)) {
            if (ENABELD_LOG) {
                spdlog::warn("getattr: return failed");
            }
            delete relation;
            delete entries;
            return -ENOENT;
        } else {
            for (auto &entry : entries->list) {
                if (path_items[0] == entry.second) {
                    inode_metadata_t* metadata = new inode_metadata_t;
                    if (!get_inode_metadata(adaptor_, entry.first, metadata)) {
                        if (ENABELD_LOG) {
                            spdlog::warn("getattr: return failed");
                        }
                        delete relation;
                        delete entries;
                        delete metadata;
                        return -ENOENT;
                    } else {
                        std::memcpy(statbuf, &metadata->stat, sizeof(struct stat));
                        delete relation;
                        delete entries;
                        delete metadata;
                        return 0;
                    }
                }
            }
            if (ENABELD_LOG) {
                spdlog::warn("getattr: return failed");
            }
            delete relation;
            delete entries;
            return -ENOENT;
        }
    } else {
        location_t* location = new location_t;
        location->tag = directory_relation;
        location->wrapper_id = relation->wrapper_id;
        if (!get_location(adaptor_, location)) {
            if (ENABELD_LOG) {
                spdlog::warn("getattr: return failed");
            }
            delete relation;
            delete location;
            return -ENOENT;
        } else {
            std::memcpy(statbuf, &location->stat, sizeof(struct stat));
            delete relation;
            delete location;
            return 0;
        }
    }
}

int wrapperfs::mknod(const char* path, mode_t mode, dev_t dev) {
    return 0;
}

int wrapperfs::mkdir(const char* path, mode_t mode) {
    return 0;
}

//
int wrapperfs::open(const char* path, struct fuse_file_info* file_info) {
    return 0;
}

int wrapperfs::read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* file_info) {
    return 0;
}

int wrapperfs::write(const char* path, const char* buf, size_t size, off_t offset, struct fuse_file_info* file_info) {
    return 0;
}

//
int wrapperfs::release(const char* path, struct fuse_file_info* file_info) {
    return 0;
}

//
int wrapperfs::opendir(const char* path, struct fuse_file_info* file_info) {
    return 0;
}

// 
int wrapperfs::readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* file_info) {
    return 0;
}

//
int wrapperfs::releasedir(const char* path, struct fuse_file_info* file_info) {
    return 0;
}

void wrapperfs::get_file_path(size_t ino, std::string &path) {
    std::stringstream ss;
    ss << (ino >> NUM_FILES_IN_DATADIR_BITS) << "/" << (ino % NUM_FILES_IN_DATADIR);
    path = combine_path(data_dir_, ss.str());
}

}