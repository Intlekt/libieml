cmake_minimum_required(VERSION 3.10)

# set the project name
project(IEML VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

LIST( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# set(BUILD_TEST True)
# set(BUILD_CHECK_ERRORS True)
# set(BUILD_PYBIND11 True)


# set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
if (DEFINED BUILD_TEST OR DEFINED BUILD_PYBIND11 OR DEFINED BUILD_WASM)
    configure_file(IEMLConfig.h.in IEMLConfig.h)

    if (DEFINED BUILD_WASM)
        set(CMAKE_SYSTEM_NAME Emscripten)
    endif()

    add_subdirectory(${CMAKE_SOURCE_DIR}/src)

    if (DEFINED BUILD_TEST)
        message("Building libieml ctest")
        add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/googletest-release-1.11.0)
        # test section
        add_subdirectory(test)
    endif()

    if(DEFINED BUILD_PYBIND11)
        message("Building python wrapper")

        add_subdirectory(thirdparty/pybind11)
        add_subdirectory(binding/python)
    endif()

    if (DEFINED BUILD_WASM)
        message("Building WASM with emscripten")
        
        add_subdirectory(binding/wasm)
    endif()
endif()

if (DEFINED BUILD_DOC)
    message("Building doc")
    add_subdirectory(docs)
endif()
