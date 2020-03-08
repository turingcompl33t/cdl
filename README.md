## C++ Development Library

An experimental library for improving the C++ development experience.

### Usage

Integrate `cdl` into an existing C++ project by adding the following line to the project's _CMakeLists.txt_:

```
target_link_libraries(your-project PRIVATE cdl)
```

### Building the Tests

`cdl` utilizes CMake / CTest in conjunction with Catch2 for unit testing.

1. Create a directory to contain the outputs of the build.

```
mkdir build
cd build
```

2. Use CMake to generate the build system for your tool of choice. For example, with Ninja:

```
cmake .. -DCDL_ENABLE_TESTING=ON -G Ninja
```

3. Build the library using your tool of choice. For example, with Ninja:

```
ninja
```

4. Run the tests

```
ctest
```

All tests should pass.