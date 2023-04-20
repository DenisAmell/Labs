#ifndef BORDER_DESCRIPTORS_MEMORY_H
#define BORDER_DESCRIPTORS_MEMORY_H

#include <iostream>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_concrete.h"
#include "memory.h"

class border_descriptors_memory final : public memory
{
private:
    void *_all_memory;

private:
    size_t get_service_part_allocator_size() const;
    void **get_first_occup_block() const;
    size_t get_allocator_size() const;
    size_t get_serv_occup_block() const;
    void **get_pointer_prev_block(void *target_ptr) const;
    void **get_pointer_next_block(void *target_ptr) const;
    size_t *get_block_size(void *target_ptr) const;
    void dump_allocator_state(bool is_allocate) const noexcept;
    logger *get_logger() const;
    std::pair<void *, size_t> get_aviable_block_address_and_size(
        void *const previous_occupied_block_address,
        void *const next_occupied_block_address) const;

public:
    void *allocate(size_t target_size) const override;

public:
    void deallocate(void const *const target_to_dealloc) const override;

public:
    border_descriptors_memory(logger *log, memory *allocator, size_t size, memory::allocate_mode method);
    border_descriptors_memory(border_descriptors_memory const &) = delete;
    border_descriptors_memory &operator=(border_descriptors_memory const &) = delete;
    ~border_descriptors_memory();
};

#endif // BORDER_DESCRIPTORS_MEMORY_H