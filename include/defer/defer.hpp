#pragma once

#include <type_traits>
#include <utility>

/// The defer statement defers execution until the current scope is exited, and
/// consists of the defer keyword and the statements to be executed later.
///
/// The defer statement is used to execute a set of statements before the code
/// execution leaves the current block of code. This is useful for performing
/// necessary cleanup tasks, regardless of how the execution leaves the block of
/// code. For instance, it can be used to ensure that file descriptors are
/// closed and memory is freed.
///
/// @note The defer statement cannot contain any code that would transfer
/// control out of the statements, such as a break or return statement, or by
/// throwing an error. If the error is thrown anyway, std::terminate is called.
///
/// @code{.cpp}
///   #include <defer/defer.hpp>
///
///   auto readfile() {
///     FILE* file = fopen("file", "rb");
///     defer {
///       fclose(file);
///     };
///
///     // work with file without worrying about closing it
///     // fclose(file) is called when leaving this function in any way
///   }
/// @endcode
///
/// The above example uses a defer statement to ensure that the file is always
/// closes when returning or throwing from this function.
#define defer \
DEFER_UNUSED const auto DEFER_NAME = ::deferred::tag{} << [&]() noexcept -> void

/// Defer statement implementation
namespace deferred {

/// Creates a name for the deferred statement storage
#define DEFER_NAME DEFER_CONCAT(DEFER_, __LINE__)

#define DEFER_CONCAT_(s1, s2) s1##s2
#define DEFER_CONCAT(s1, s2)  DEFER_CONCAT_(s1, s2)

// [[maybe_unused]] implementation for C++11
#if defined(__clang__) || defined(__GNUC__)
#define DEFER_UNUSED __attribute__((__unused__))
#elif defined(_MSC_VER)
#define DEFER_UNUSED __pragma(warning(suppress : 4100 4101 4189))
#else
#define DEFER_UNUSED
#endif

/// Helper tag to use for creating a defer statement
/// inside of a macro
struct tag {};

/// Defer statement implementation;
/// runs the closure it contains when the destructor is called
template<typename F>
class defer_stmt {
    F body;                 ///< Closure executed when the destructor is called
    bool is_engaged = true; ///< Whether this statement should execute the body

public:
    /// Creates a defer statement from a moved @p other
    defer_stmt(defer_stmt&& other) noexcept : body(std::move(other.body)) {
        other.is_engaged = false;
    }

    /// Creates a defer statement from a given @p closure
    explicit defer_stmt(F&& closure) noexcept : body(std::move(closure)) {}

    /// Runs the closure it contains when the enclosing scope is exited
    ~defer_stmt() noexcept { if (this->is_engaged) this->body(); }

    defer_stmt(const defer_stmt&) = delete;        ///< not copy-constructible
    auto operator=(const defer_stmt&) = delete;    ///< not copy-assignable
    auto operator=(defer_stmt&&) = delete;         ///< not move-assignable
};

/// Creates a defer statement from the given @p closure
template<typename F>
inline auto operator<<(tag, F&& closure) noexcept {
    return defer_stmt<typename std::decay<F>::type>{ std::forward<F>(closure) };
}

}    // namespace deferred
