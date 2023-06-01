#ifndef LIST_MEMORY_H
#define LIST_MEMORY_H

#include "../memory/memory.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_concrete.h"
#include "../logger/log2/logger_holder.h"
#include <iostream>

class list_memory final : public memory, private logger_holder
{
private:
    void *_ptr_to_memory;

public:
    class dealloc_fail final : public std::exception
    {
    public:
        char const *what() const noexcept override
        {
            return "Failed to deallocate";
        }
    };

public:
    list_memory(list_memory const &other) = delete;
    list_memory &operator=(list_memory const &other) = delete;
    list_memory(list_memory &&) = delete;
    list_memory &operator=(list_memory &&other) = delete;

public:
    virtual void *allocate(size_t requested_block_size) const override;

protected:
    void *get_pointer_block(void *target_ptr) const override;
    size_t get_size_block(void *target_ptr) const override;
    void *get_first_avail_block() const override;
    void **get_first_avail_block_ptr() const override;
    size_t get_service_part_allocator_size() const override;
    size_t get_service_part_avail_block_size() const override;
    logger *get_logger() const noexcept override;
    void dump_allocator_state(bool is_allocate) const noexcept override;
    void *get_right_border_of_memory_ptr() const override;
    std::string get_pointer_address(void *target_ptr) const override;
    void debug_alloc(const void *target_ptr) const override;
    std::string get_allocate_mode_string(allocate_mode method) const override;

public:
    virtual void deallocate(void *block_to_deallocate_address) const override;

public:
    list_memory(logger *log, memory *allocator, size_t size, memory::allocate_mode method);

public:
    ~list_memory();
};

#endif // LIST_MEMORY_H