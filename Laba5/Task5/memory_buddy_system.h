#ifndef MEMORY_BUDDY_SYSTEM_H
#define MEMORY_BUUDDY_SYSTEM_H

#include "memory.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_concrete.h"
#include "../logger/log2/logger_holder.h"

class memory_buddy_system final : public memory, private logger_holder
{

private:
    void *_all_memory;

private:
    size_t get_block_avail_size(void *target_ptr) const override;
    size_t get_occup_block_serv_size() const override;
    size_t get_avail_block_serv_size() const override;
    void *get_pointer_avail_prev_block(void *target_ptr) const override;
    void *get_pointer_avail_next_block(void *target_ptr) const override;
    void *get_first_avail_block() const override;
    void **get_first_avail_block_ptr() const override;
    size_t get_service_part_allocator_size() const override;
    void dump_allocator_state(bool is_allocate) const noexcept override;
    std::string get_pointer_address(void *target_ptr) const override;
    void debug_alloc(const void *target_ptr) const override;
    std::string get_allocate_mode_string(allocate_mode method) const override;
    logger *get_logger() const noexcept override;
    unsigned char compact_block_size_and_block_occup(size_t size_as_power_of_2, bool block_occup) const override;
    void *get_buddies(void *target_ptr) const override;
    bool is_free_block(void *target_ptr) const override;

public:
    virtual void *allocate(size_t target_size) const override;

public:
    virtual void deallocate(void const *const target_to_dealloc) const override;

public:
    memory_buddy_system(logger *log, memory *allocator, size_t size_as_power_of_2, allocate_mode method);
    ~memory_buddy_system();
};

#endif // MEMORY_BUUDDY_SYSTEM_H