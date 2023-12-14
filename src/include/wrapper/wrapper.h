#pragma once

#include <vector>
#include <string>

#include "utils/logger_routine.h"
#include "adaptor/leveldb_adaptor.h"

namespace wrapperfs {

enum wrapper_tag {
    directory_relation,
    similarity_relation,
};

struct group_metadata_t {
    size_t id;
    size_t tag;
};

struct location_metadata_t {
    std::string location;
    std::vector<size_t> list;
};

struct index_metadata_t {
    int distance;
    std::vector<size_t> list;
};

struct wrapper_t {
    group_metadata_t group_metadata;
    location_metadata_t location_metadata;
    index_metadata_t index_metadata;
};

bool get_wrapper(LevelDBAdaptor* adaptor, size_t wrapper_id, wrapper_t* &wrapper);
bool put_wrapper(LevelDBAdaptor* adaptor, size_t wrapper_id, wrapper_t* wrapper);
int distance_compute(wrapper_t* a, wrapper_t* b);

}