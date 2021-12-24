pybind11_add_module(pyieml pyieml.cpp)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/pybind11/include)
target_link_libraries(pyieml PUBLIC IEMLParser_static)
