// This is a dummy program that just needs to compile and link to tell us if
// the C++20 coroutine API is available. Use CMake's configure_file command
// to replace the COROUTINE_HEADER and COROUTINE_NAMESPACE tokens for each
// combination of headers and namespaces which we want to pass to the CMake
// try_compile command.

#include <coroutine>
#include <future>

struct task
{
    struct promise_type
    {
        task get_return_object() noexcept
        {
            return {};
        }

        std::suspend_never initial_suspend() noexcept
        {
            return {};
        }

        std::suspend_never final_suspend() noexcept
        {
            return {};
        }

        void return_void() noexcept
        {
            promise.set_value();
        }

        void unhandled_exception()
        {
            promise.set_exception(std::current_exception());
        }

        std::promise<void> promise;
    };

    constexpr bool await_ready() const noexcept
    {
        return true;
    }

    std::future<void> future;
};

task test_co_return()
{
    co_return;
}

int main()
{
    test_co_return().future.get();
    return 0;
}