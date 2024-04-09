cmake_minimum_required(VERSION 3.16)
project(2d_dt)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes -Wno-unknown-pragmas")
endif ()

include_directories(.)

include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#        DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps
#        DOWNLOAD_NO_EXTRACT TRUE
#)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#
#enable_testing()
#
#include(GoogleTest)

FetchContent_Declare(
        jsoncpp_lib
        URL https://github.com/open-source-parsers/jsoncpp/archive/refs/heads/master.zip
        DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps
#        DOWNLOAD_NO_EXTRACT TRUE
)

FetchContent_MakeAvailable(jsoncpp_lib)

#include(tests/CMakeLists.txt)
#
#add_executable(hello_test
#        ${myTest_SOURCES}
#        )
#
#target_link_libraries(
#        hello_test
#        gtest_main
#        jsoncpp_lib
#)
#
#gtest_discover_tests(hello_test
#        DISCOVERY_TIMEOUT 30
#        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

include(src/CMakeLists.txt)

add_executable(2d_dt
        ${myApp_SOURCES}
        )

target_link_libraries(
        2d_dt
        jsoncpp_lib
)

#string(REPLACE "-O3" "-O2" CMAKE_CXX_FLAGS_RELEASE_INIT ${CMAKE_CXX_FLAGS_RELEASE_INIT})

MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE_INIT: " ${CMAKE_CXX_FLAGS_RELEASE_INIT})

