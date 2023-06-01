#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_concrete.h"
#include "../logger/log2/logger_holder.h"

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
    virtual size_t get_block_avail_size(void *target_ptr) const = 0;
    virtual size_t get_occup_block_serv_size() const = 0;
    virtual size_t get_avail_block_serv_size() const = 0;
    virtual void *get_pointer_avail_prev_block(void *target_ptr) const = 0;
    virtual void *get_pointer_avail_next_block(void *target_ptr) const = 0;
    virtual void *get_first_avail_block() const = 0;
    virtual void **get_first_avail_block_ptr() const = 0;
    virtual size_t get_service_part_allocator_size() const = 0;
    virtual void dump_allocator_state(bool is_allocate) const noexcept = 0;
    virtual std::string get_pointer_address(void *target_ptr) const = 0;
    virtual void debug_alloc(const void *target_ptr) const = 0;
    virtual std::string get_allocate_mode_string(allocate_mode method) const = 0;
    virtual logger *get_logger() const noexcept = 0;
    virtual unsigned char compact_block_size_and_block_occup(size_t size_as_power_of_2, bool block_occup) const = 0;
    virtual void *get_buddies(void *target_ptr) const = 0;
    virtual bool is_free_block(void *target_ptr) const = 0;

protected:
    virtual memory *get_memory() const = 0;

public:
    virtual void *allocate(size_t target_size) const = 0;

public:
    virtual void deallocate(void const *const target_to_dealloc) const = 0;

public:
    virtual ~memory() = default;
};

#endif // MEMORY_H
