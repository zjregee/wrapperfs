#include "wrapper/wrapper.h"

namespace wrapperfs {

bool get_wrapper(LevelDBAdaptor* adaptor, size_t wrapper_id, wrapper_t* &inode) {
    return true;
}

bool put_wrapper(LevelDBAdaptor* adaptor, size_t wrapper_id, wrapper_t* inode) {
    return true;
}

int distance_compute(wrapper_t* a, wrapper_t* b) {
    return 0;
}

}