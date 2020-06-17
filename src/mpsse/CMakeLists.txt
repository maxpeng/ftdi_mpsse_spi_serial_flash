if (MSVC)
    include(${CMAKE_CURRENT_LIST_DIR}/msvc-x86/CMakeLists.txt)
elseif(MINGW)
    include(${CMAKE_CURRENT_LIST_DIR}/mingw-x86/CMakeLists.txt)
else()
    message( FATAL_ERROR "The compiler is not supported.")
endif()

target_sources(${CURRENT_EXE_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libMPSSE_i2c.h
        ${CMAKE_CURRENT_LIST_DIR}/libMPSSE_spi.h
)

target_include_directories(${CURRENT_EXE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)
