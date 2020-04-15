# modern-cpp-lib
An example of a C++ library built using modern cmake approach as described
[here](https://cliutils.gitlab.io/modern-cmake/) and the related example
[here](https://gitlab.com/CLIUtils/modern-cmake/tree/master/examples/extended-project)

This project is used by [another project](https://github.com/chafey/modern-cpp-lib-js)
which shows how to build JS/WASM code from a C++ library like this one via EMSCRIPTEN

## Dependencies

* CMake 3.15 or better
* A C++ compatible compiler

## Visual Studio Code Remote Containers Support

This project includes support for developing in a docker container using the 
Visual Studio Code Remote - Containers extension.  The configured docker container
includes everything needed to build the project so you don't have to deal with
installing the dependencies above

# to configure
``` bash
cmake -S . -B build
```

# to build
``` bash
cmake --build build
```

# to test
``` bash
cmake --build build -t test -- -e CTEST_OUTPUT_ON_FAILURE=1
```

# configure, build and run tests
``` bash
cmake -S . -B build && cmake --build build && cmake --build build -t test -- -e CTEST_OUTPUT_ON_FAILURE=1
```