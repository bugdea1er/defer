## `defer` keyword for C++11 from Swift

The defer statement defers execution until the current scope is exited, and consists of the defer keyword and the statements to be executed later.

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

> [!NOTE]
> Unlike the Swift `defer`, this implementation executes the deferred actions in the same order they are written in the source code
