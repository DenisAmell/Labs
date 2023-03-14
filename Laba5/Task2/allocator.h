#ifndef ALLOCATOR_H
#define ALLOCATOR_H

class memory
{
public:
    virtual void *memory::allocate(size_t target_size) const = 0;
public:
    virtual void memory::deallocate(void const * const target_to_dealloc) const = 0;
public:
    virtual ~memory();
};

#endif