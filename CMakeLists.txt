cmake_minimum_required(VERSION 3.14)
project(ahc027 VERSION 1.0 DESCRIPTION "ahc027")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



add_executable(dfs src/dfs.cpp)
add_executable(bfs_rt src/bfs_rt.cpp)
add_executable(bfs src/bfs.cpp)


target_compile_definitions(
    dfs PRIVATE
    # DEVEL_ASSERT=1
    # DEVEL=1
    MIN_PRINT=1
    SHUFFLE=1
)

target_compile_definitions(
    bfs_rt PRIVATE
    # DEVEL_ASSERT=1
    # DEVEL=1
    MIN_PRINT=1
    SHUFFLE=0
)



# Define the library target

