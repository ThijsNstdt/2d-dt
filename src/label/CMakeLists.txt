# include all source files in the directory
file(GLOB files1 "${CMAKE_CURRENT_LIST_DIR}/*.hpp")
file(GLOB files2 "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

list(APPEND myApp_SOURCES
        ${files1}
        ${files2}
        )