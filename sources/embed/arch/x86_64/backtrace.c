#include <embed/debug.h>

struct stackframe
{
    struct stackframe *rbp;
    uint64_t rip;
};

static size_t embed_debug_backtrace_impl(uintptr_t rbp, uintptr_t *buf, size_t cap)
{
    struct stackframe *stackframe = (struct stackframe *)rbp;

    size_t i = 0;

    while (stackframe && i < cap)
    {
        buf[i++] = stackframe->rip;
        stackframe = stackframe->rbp;
    }

    return i;
}

size_t embed_debug_backtrace(uintptr_t *buf, size_t cap)
{
    uintptr_t rbp;
    asm volatile("mov %%rbp, %0"
                 : "=r"(rbp));
    return embed_debug_backtrace_impl(rbp, buf, cap);
}
