#ifndef BORDER_DESCRIPTORS_MEMORY_H
#define BORDER_DESCRIPTORS_MEMORY_H

#include "memory.h"
#include <iostream>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"

class border_descriptors_memory final : public memory
{
private:
    void *_all_memory;

public:
    enum class allocate_mode
    {
        first_fit,
        best_fit,
        worst_fit
    };

private:
    size_t get_service_part_allocator_size() const;
    void *get_first_occup_block() const;
    size_t get_allocator_size() const;
    size_t get_serv_occup_block() const;
    void **get_pointer_prev_block(void *target_ptr) const;
    void **get_pointer_next_block(void *target_ptr) const;
    size_t *get_size_block(void *target_ptr) const;
    void dump_allocator_state(bool is_allocate) const noexcept;
    logger *get_logger() const;

public:
    virtual void *allocate(size_t target_size) const override;

public:
    virtual void deallocate(void const *const target_to_dealloc) const override;

public:
    border_descriptors_memory(logger *log, memory *allocator, size_t size, allocate_mode method);

public:
    virtual ~border_descriptors_memory();
};

#endif // BORDER_DESCRIPTORS_MEMORY_H
