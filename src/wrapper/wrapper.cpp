#include "wrapper/wrapper.h"

namespace wrapperfs {

bool get_entries(LevelDBAdaptor* adaptor, entries_t* &entries) {
    return true;
}

bool put_entries(LevelDBAdaptor* adaptor, entries_t* entries) {
    return true;
}

bool get_relation(LevelDBAdaptor* adaptor, relation_t* &relation) {
    return true;
}

bool put_relation(LevelDBAdaptor* adaptor, relation_t* relation) {
    return true;
}

bool get_location(LevelDBAdaptor* adaptor, location_t* &location) {
    return true;
}

bool put_location(LevelDBAdaptor* adaptor, location_t* location) {
    return true;
}

}