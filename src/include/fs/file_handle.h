#pragma once

namespace wrapperfs {

struct file_handle_t {
    int fd;
    int flags;
};

}