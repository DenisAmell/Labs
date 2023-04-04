#ifndef MEMORY_DENIS_H
#define MEMORY_DENIS_H

#include "allocator.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"

class memory_denis final : public memory
{
public:
    virtual void *allocate(size_t target_size) const override;

public:
    virtual void deallocate(void const *const target_to_dealloc) const override;

public:
    void debug_alloc(unsigned char *target_ptr, size_t size, logger *log);

public:
    ~memory_denis() = default;
};

#endif // MEMORY_DENIS_H