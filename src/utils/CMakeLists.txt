add_library(
    wrapperfs_utils
    OBJECT
    hash_routine.cpp
    logger_routine.cpp
)

set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:wrapperfs_utils> PARENT_SCOPE)