#ifndef ALLOCATOR2_H
#define ALLOCATOR2_H

#include "../Task2/allocator.h"
#include "../Task2/memory_denis.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"
#include <iostream>

class My_memory : public memory
{
private:
    void *_ptr;

public:
    enum class mode
    {
        first_fit,
        best_fit,
        worst_fit
    };

public:
    My_memory(My_memory const &) = delete;
    My_memory &operator=(My_memory const &obj) = delete;
    // My_memory(My_memory &&);
    // My_memory &operator=(My_memory &&obj);

public:
    virtual void *allocate(size_t target_size) const override;

protected:
    virtual void *get_pointer(void *target_ptr) const;
    virtual size_t get_size_block(void *target_ptr) const;

public:
    virtual void deallocate(void const *const target_to_dealloc) const override;

public:
    virtual void debug_alloc(const void *target_ptr, size_t size, logger *log) const override;

public:
    My_memory(logger *log, memory *allocator, size_t size, mode method);

public:
    ~My_memory();
};

#endif // ALLOCATOR2_H