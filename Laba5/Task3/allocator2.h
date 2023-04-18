#ifndef ALLOCATOR2_H
#define ALLOCATOR2_H

#include "memory.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"
#include <iostream>

class My_memory : public memory
{
private:
    void *_ptr_to_memory;

public:
    enum class mode
    {
        first_fit,
        best_fit,
        worst_fit
    };
    class dealloc_fail final : public std::exception
    {
    public:
        char const *what() const noexcept override
        {
            return "Failed to deallocate";
        }
    };

public:
    My_memory(My_memory const &) = delete;
    My_memory &operator=(My_memory const &obj) = delete;
    // My_memory(My_memory &&);
    // My_memory &operator=(My_memory &&obj);

public:
    virtual void *allocate(size_t target_size) const override;

private:
    virtual void *get_pointer_block(void *target_ptr) const;
    virtual size_t get_size_block(void *target_ptr) const;
    virtual void *const get_first_avail_block() const;
    virtual void **get_first_avail_block_ptr() const;
    virtual size_t get_service_part_allocator_size() const;
    virtual size_t get_service_part_avail_block_size() const;
    virtual logger *get_logger() const;
    void dump_allocator_state(bool is_allocate) const noexcept;
    virtual void *get_right_border_of_memory_ptr() const;
    virtual std::string get_pointer_address(void *target_ptr) const;
    virtual void debug_alloc(const void *target_ptr) const;

public:
    virtual void deallocate(void const *const target_to_dealloc) const override;

public:
    My_memory(logger *log, memory *allocator, size_t size, mode method);

public:
    ~My_memory();
};

#endif // ALLOCATOR2_H