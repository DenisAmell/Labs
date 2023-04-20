#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>

class memory
{
public:
    enum class allocate_mode
    {
        first_fit,
        best_fit,
        worst_fit
    };

public:
    virtual void *allocate(size_t target_size) const = 0;

public:
    virtual void deallocate(void const *const target_to_dealloc) const = 0;

public:
    virtual ~memory() = default;
};

#endif // MEMORY_H