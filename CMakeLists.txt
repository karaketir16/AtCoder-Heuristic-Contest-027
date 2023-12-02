cmake_minimum_required(VERSION 3.14)
project(ahc027 VERSION 1.0 DESCRIPTION "ahc027")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



file(GLOB_RECURSE SOURCES
    src/*.c 
    src/*.cpp
    )

file(GLOB_RECURSE HEADERS
    include/*.hpp
    include/*.h
    )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME}_s ${SOURCES} ${HEADERS})


target_compile_definitions(
    ${PROJECT_NAME}_s PRIVATE
    # DEVEL_ASSERT=1
    # DEVEL=1
    SHUFFLE=1
)

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    # DEVEL_ASSERT=1
    # DEVEL=1
    SHUFFLE=0
)


# Define the library target

