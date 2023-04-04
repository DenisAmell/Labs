#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>

class memory
{
public:
    virtual void *allocate(size_t target_size) const = 0;

public:
    virtual void deallocate(void const *const target_to_dealloc) const = 0;

public:
    virtual ~memory();
};

#endif // ALLOCATOR_H
