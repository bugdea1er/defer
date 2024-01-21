## `defer` keyword for C++11 from Swift

The defer statement defers execution until the current scope is exited, and consists of the defer keyword and the statements to be executed later.


> [!NOTE]
> [RAII](https://en.cppreference.com/w/cpp/language/raii) approach is still recommended, especially when you want to transfer the ownership of the aquired objects. Checkout [C++ Code Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#e6-use-raii-to-prevent-leaks) on this recommendation.
> Use this defer syntax only when working with older (e.g. C) APIs and without transferring ownership.

## Introduction

The defer statement is used to execute a set of statements before the code execution leaves the current block of code. This is useful for performing necessary cleanup tasks, regardless of how the execution leaves the block of code. For instance, it can be used to ensure that file descriptors are closed and memory is freed.

> [!CAUTION]
> The defer statement cannot contain any code that would transfer control out of the statements, such as a break or return statement, or by throwing an error. If the error is thrown anyway, std::terminate is called.

```cpp
#include <defer/defer.hpp>

auto readfile() {
  FILE* file = fopen("file", "rb");
  defer {
    fclose(file);
  };

  // work with file without worrying about closing it
  // fclose(file) is called when leaving this function in any way
}
```

The above example uses a defer statement to ensure that the file is always
closes when returning or throwing from this function.

## Integration

### Copy a single file

Since this is a header-only library, you can just copy the [defer.hpp](https://github.com/bugdea1er/defer/blob/main/include/defer/defer.hpp) file to your project and enable C++11 

### CMake integration

You can also use the CMake interface target `defer::defer` and described belowю

#### Embedded

To embed the library into your existing CMake project, place the entire source tree in a subdirectory (for example, using `git submodule` commands) and call `add_subdirectory()` in your `CMakeLists.txt` file:
```cmake
add_subdirectory(defer)
...
add_library(foo ...)
target_link_libraries(foo PRIVATE defer::defer)
```

#### FetchContent

You can also use the FetchContent functions to automatically download a dependency. Put this in your `CMakeLists.txt` file:
```cmake
include(FetchContent)

FetchContent_Declare(defer URL https://github.com/bugdea1er/defer/releases/download/v1.0/defer.tar.xz)
FetchContent_MakeAvailable(defer)

target_link_libraries(foo PRIVATE defer::defer)
```
