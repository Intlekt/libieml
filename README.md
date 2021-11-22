# LibIEML

## Installation

```
git clone --recurse-submodules git@github.com:Intlekt/libieml.git
```

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

### Building with CMake

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

### Python wrapper installation

To install the Python wrapper (with 4 cores for compilation) use :
```
CMAKE_BUILD_PARALLEL_LEVEL=4 python setup.py install
```

An usage example :
```python
import pyieml

parser = pyieml.Parser("""@component fr"test" (0 ~nom #'wa.') . """)
parser.parse()
print([e.to_json() for e in parser.errors()])

```