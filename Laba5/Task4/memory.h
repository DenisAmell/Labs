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
    virtual size_t get_service_part_allocator_size() const = 0;
    virtual void **get_first_occup_block() const = 0;
    virtual size_t get_allocator_size() const = 0;
    virtual size_t get_serv_occup_block() const = 0;
    virtual void **get_pointer_prev_block(void *target_ptr) const = 0;
    virtual void **get_pointer_next_block(void *target_ptr) const = 0;
    virtual size_t *get_block_size(void *target_ptr) const = 0;
    virtual void dump_allocator_state(bool is_allocate) const noexcept = 0;
    virtual logger *get_logger() const noexcept = 0;
    virtual std::pair<void *, size_t> get_aviable_block_address_and_size(
        void *const previous_occupied_block_address,
        void *const next_occupied_block_address) const = 0;
    virtual void debug_alloc(const void *target_ptr) const = 0;
    virtual std::string get_pointer_address(void *target_ptr) const = 0;
    virtual std::string get_allocate_mode_string(allocate_mode method) const = 0;

public:
    virtual void *allocate(size_t target_size) const = 0;

public:
    virtual void deallocate(void const *const target_to_dealloc) const = 0;

public:
    virtual ~memory() = default;
};

#endif // MEMORY_H