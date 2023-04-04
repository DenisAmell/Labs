#ifndef MEMORY_DENIS_H
#define MEMORY_DENIS_H

#include "allocator.h"

class memory_denis final : public memory
{
public:
    virtual void *allocate(size_t target_size) const override;

public:
    virtual void deallocate(void const *const target_to_dealloc) const override;

public:
    ~memory_denis() = default;
};

#endif // MEMORY_DENIS_H