# LibIEML

## Installation

```
git clone --recurse-submodules git@github.com:Intlekt/libieml.git
```

If you plan to work on the parser in addition of the frontend or the api, we suggest you to look at repository [ieml-project](https://github.com/Intlekt/ieml-project) instead.

## Build

### Dockerized

```bash
docker-compose up --build --force-recreate build_python
docker-compose up --build --force-recreate build_wasm
```

### Native

### Dependencies

#### General dependencies

Requires `GCC 8+` if building the tests (requires `<filesystem>`)

```
sudo apt install pkg-config uuid-dev
```

Java for generating the parser

```
sudo apt install default-jre
```

#### Python binding dependencies

```
sudo apt install python3-dev
```

### Building with CMake

#### CMake options

- `BUILD_TEST` build the test suite
- `BUILD_PYBIND11` build the python wrapper

#### Build command

```
mkdir build
cd build
cmake [opts] ..
make
```

For instance `opts = "-DBUILD_TEST"`.

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

### Building WASM

To build wasm, you have to download and install the [emscripten toolchain](https://emscripten.org/docs/getting_started/downloads.html).

At the moment, a package-config file is missing from the emscripten toolchain for uuid. For antlr4 to build, you have to add the following file
to the ${EMSDK_ROOT}/upstream/emscripten/cache/sysroot/lib/pkgconfig/ folder.

filename: `uuid.pc`

```
Name: uuid
Description: Universally unique id library
Version: 2.37.3
```

Then run the following build commands:

```
mkdir build
cd build
emcmake cmake -DBUILD_WASM:bool=True ..
emmake make
```

### Build the doc

#### Requirements

## Tests

### Dockerized

Uses images built from [repo](https://github.com/Intlekt/dockers-config).

```bash
docker-compose up --build --force-recreate test
```
