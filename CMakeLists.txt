cmake_minimum_required(VERSION 3.10)

# set the project name
project(IEML VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# set(BUILD_TEST True)
# set(BUILD_CHECK_ERRORS True)
set(BUILD_PYBIND11 True)

configure_file(IEMLConfig.h.in IEMLConfig.h)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)


if (DEFINED BUILD_TEST)
    message("Building tests")
    add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/googletest-release-1.11.0)
    # test section
    add_subdirectory(test)

endif()

if (DEFINED BUILD_CHECK_ERRORS)
    message("Building CHECK_ERRORS_EXECUTABLE")

    add_executable(IEML_CHECK_ERRORS check_errors.cxx)
    target_link_libraries(IEML_CHECK_ERRORS IEMLParser)
endif()

# # add the executable
# add_executable(IEML main.cxx)
# target_link_libraries(IEML IEMLParser)


if(DEFINED BUILD_PYBIND11)
    message("Building python wrapper")

    add_subdirectory(thirdparty/pybind11)
    add_subdirectory(binding/python)
endif()