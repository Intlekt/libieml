cmake_minimum_required(VERSION 3.10)

# set the project name
project(IEML VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/grammar)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/googletest-release-1.11.0)

configure_file(IEMLConfig.h.in IEMLConfig.h)

# add the executable
add_executable(IEML main.cxx)
target_link_libraries(IEML IEMLParser)

# test section
add_subdirectory(test)
