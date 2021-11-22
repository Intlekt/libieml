# LibIEML

## Installation

### Dependencies

#### General dependencies
Requires `GCC 8+` if building the tests (requires `<filesystem>`)
```
sudo apt install pkg-config uuid-dev
```

#### Python binding dependencies
```
sudo apt install python3-dev
```

### Building

#### CMake options
 
 - `BUILD_TEST` build the test suite
 - `BUILD_CHECK_ERRORS` build the IEML_CHECK_ERROR executable
 - `BUILD_PYBIND11` build the python wrapper

#### Build command
```
mkdir build
cd build
cmake [opts] ..
make
```
For instance, to build IEML_CHECK_ERROR, `opts = "-DBUILD_CHECK_ERRORS"`.
To build IEML_CHECK_ERROR and the tests, `opts = "-DBUILD_CHECK_ERRORS -DBUILD_TEST"`.

