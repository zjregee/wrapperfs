#pragma once

#include <sys/stat.h>

namespace wrapperfs {

struct file_handle_t {
    int fd;
    int flags;
    struct stat stat;
};

}