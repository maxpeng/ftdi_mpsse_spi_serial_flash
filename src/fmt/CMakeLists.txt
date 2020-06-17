# add source and header files when compiling the given target.
# although it is not necessary to add header files, we also do that for easing the management of IDE.
target_sources(${CURRENT_EXE_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/format.cc
        ${CMAKE_CURRENT_LIST_DIR}/posix.cc
        ${CMAKE_CURRENT_LIST_DIR}/chrono.h
        ${CMAKE_CURRENT_LIST_DIR}/color.h
        ${CMAKE_CURRENT_LIST_DIR}/core.h
        ${CMAKE_CURRENT_LIST_DIR}/format.h
        ${CMAKE_CURRENT_LIST_DIR}/format-inl.h
        ${CMAKE_CURRENT_LIST_DIR}/locale.h
        ${CMAKE_CURRENT_LIST_DIR}/ostream.h
        ${CMAKE_CURRENT_LIST_DIR}/posix.h
        ${CMAKE_CURRENT_LIST_DIR}/prepare.h
        ${CMAKE_CURRENT_LIST_DIR}/printf.h
        ${CMAKE_CURRENT_LIST_DIR}/ranges.h
        ${CMAKE_CURRENT_LIST_DIR}/safe-duration-cast.h
    )


# We don't add fmt to the include directory list, because we want to explicitly specify the fmt directory,
# for example  #include "fmt/core.h"
## # add current directory to the compiler included directories when compiling the given target.
## target_include_directories(${CURRENT_EXE_NAME}
##     PUBLIC
##         "${CMAKE_CURRENT_LIST_DIR}"
## )
