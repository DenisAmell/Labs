#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"

class memory
{
public:
    virtual void *allocate(size_t target_size) const = 0;

public:
    virtual void deallocate(void const *const target_to_dealloc) const = 0;

public:
    virtual void debug_alloc(const void *target_ptr, size_t size, logger *log) const = 0;

public:
    virtual ~memory();
};

#endif // ALLOCATOR_H
