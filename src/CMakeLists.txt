include(src/updater/CMakeLists.txt)
include(src/factory/CMakeLists.txt)
include(src/simplices/CMakeLists.txt)
include(src/observer/CMakeLists.txt)
include(src/universe/CMakeLists.txt)
include(src/runner/CMakeLists.txt)
include(src/label/CMakeLists.txt)
include(src/jsonutilities/CMakeLists.txt)
include(src/rng/CMakeLists.txt)
include(src/argumenthandler/CMakeLists.txt)
include(src/misc/CMakeLists.txt)
#include(src/structs/CMakeLists.txt)

list(APPEND myApp_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        )