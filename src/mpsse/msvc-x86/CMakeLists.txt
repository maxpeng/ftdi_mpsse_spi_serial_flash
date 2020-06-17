find_library(MPSSE_LIBRARY
    NAMES libMPSSE.lib
    HINTS ${CMAKE_CURRENT_LIST_DIR}
)

if (MPSSE_LIBRARY-NOTFOUND )
    message("MPSSE library - not found")
else()
    message("Found MPSSE library - ${MPSSE_LIBRARY}")
endif ()

target_sources(${CURRENT_EXE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ftd2xx.h
)

target_link_libraries(${CURRENT_EXE_NAME} ${MPSSE_LIBRARY})

target_include_directories(${CURRENT_EXE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)
