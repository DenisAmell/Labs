#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_concrete.h"

class memory
{
public:
    enum class allocate_mode
    {
        first_fit,
        best_fit,
        worst_fit
    };

private:
    virtual void *get_pointer_block(void *target_ptr) const = 0;
    virtual size_t get_size_block(void *target_ptr) const = 0;
    virtual void *const get_first_avail_block() const = 0;
    virtual void **get_first_avail_block_ptr() const = 0;
    virtual size_t get_service_part_allocator_size() const = 0;
    virtual size_t get_service_part_avail_block_size() const = 0;
    virtual void dump_allocator_state(bool is_allocate) const noexcept = 0;
    virtual void *get_right_border_of_memory_ptr() const = 0;
    virtual std::string get_pointer_address(void *target_ptr) const = 0;
    virtual void debug_alloc(const void *target_ptr) const = 0;
    virtual std::string get_allocate_mode_string(allocate_mode method) const = 0;

public:
    virtual void *allocate(size_t target_size) const = 0;

public:
    virtual void deallocate(void const *const target_to_dealloc) const = 0;

public:
    virtual ~memory() = default;
};

#endif // MEMORY_H
