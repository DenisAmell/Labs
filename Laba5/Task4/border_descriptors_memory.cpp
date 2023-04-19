#include "border_descriptors_memory.h"

size_t border_descriptors_memory::get_service_part_allocator_size() const
{
    return sizeof(size_t) + sizeof(logger *) + sizeof(allocate_mode) + sizeof(memory *) + sizeof(void *);
}

void *border_descriptors_memory::get_first_occup_block() const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_all_memory) + get_service_part_allocator_size() - sizeof(void *));
}

size_t border_descriptors_memory::get_allocator_size() const;
{
    return *reinterpret_cast<size_t *>(_all_memory);
}

size_t border_descriptors_memory::get_serv_occup_block() const
{
    return sizeof(size_t) + 2 * sizeof(void *);
}

void **border_descriptors_memory::get_pointer_prev_block(void *target_ptr) const
{
    return reinterpret_cast<void **>(reinterpret_cast<size_t *>(target_ptr) + 1);
}

void **border_descriptors_memory::get_pointer_next_block(void *target_ptr) const
{
    return reinterpret_cast<void **>(reinterpret_cast<size_t *>(target_ptr) + 1) + 1;
}

size_t *border_descriptors_memory::get_size_block(void *target_ptr) const
{
    return reinterpret_cast<size_t *>(target_ptr);
}

logger *border_descriptors_memory::get_logger() const
{
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t));
}

void *border_descriptors_memory::allocate(size_t requested_block_size) const
{
    // auto overridden_block_size = target_size;

    // if (overridden_block_size < sizeof(void *))
    // {
    //     overridden_block_size = sizeof(void *);
    //     // TODO: logs;
    // }

    auto alloc_method = *reinterpret_cast<allocate_mode *>(reinterpret_cast<unsigned char *>(_all_memory) + sizeof(size_t) + sizeof(logger *));

    void *prev_block = nullptr;
    void *first_occup_block = get_first_occup_block();
    void *curr_block = first_occup_block;

    void *prev_to_target_block = nullptr;
    void *target_block = nullptr;
    size_t target_block_size = 0;
    void *next_to_target_block = nullptr;

    size_t serv_occup_block_size = get_serv_occup_block();
    size_t allocator_size = get_allocator_size();
    size_t service_part_allocator_size = get_service_part_allocator_size();

    if (curr_block == nullptr && requested_block_size + serv_occup_block_size < allocator_size)
    {
        curr_block = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size);
        *get_size_block(curr_block) = requested_block_size;
        *get_pointer_prev_block(curr_block) = nullptr;
        *get_pointer_next_block(curr_block) = nullptr;
        return reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(curr_block) + serv_occup_block_size);
    }

    while (true)
    {
        size_t block_size = 0;
        void *block_address = nullptr;
        auto prev_block_size = prev_block == nullptr
                                   ? 0
                                   : *get_size_block(prev_block);
        auto curr_block_size = curr_block == nullptr
                                   ? 0
                                   : *get_size_block(curr_block);

        if ((prev_block == nullptr && curr_block != nullptr) && (reinterpret_cast<unsigned char *>(curr_block) - (reinterpret_cast<unsigned char *>(_all_memory) + serv_occup_block_size) >= requested_block_size + serv_occup_block_size))
        {
            block_address = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size);
            block_size = reinterpret_cast<unsigned char *>(curr_block) - block_address;
        }
        else if ((curr_block == nullptr && ((reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size + allocator_size) - (reinterpret_cast<unsigned char *>(prev_block) + serv_occup_block_size + prev_block_size) >= requested_block_size + serv_occup_block_size)))
        {
            block_address = reinterpret_cast<unsigned char *>(prev_block) + serv_occup_block_size + prev_block_size;
            block_size = reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size + allocator_size - block_address;
        }
        else
        {
            block_address = reinterpret_cast<unsigned char *>(prev_block) + serv_occup_block_size + prev_block_size;
            block_size = reinterpret_cast<unsigned char *>(curr_block) - block_address;
        }

        if (block_size >= requested_block_size + serv_occup_block_size)
        {
            if (alloc_method == allocate_mode::first_fit ||
                alloc_method == allocate_mode::best_fit && (target_block == nullptr || target_block_size > block_size) ||
                alloc_method == allocate_mode::worst_fit && (target_block == nullptr || target_block_size < block_size))
            {
                prev_to_target_block = prev_block;
                target_block = block_address;
                target_block_size = block_size;
                next_to_target_block = curr_block;
            }

            if (alloc_method == allocate_mode::first_fit)
            {
                break;
            }
        }

        if (curr_block == nullptr)
        {
            break;
        }

        prev_block = curr_block;
        curr_block = *get_pointer_next_block(curr_block);
    }

    if (target_block == nullptr)
    {
        throw std::bad_alloc();
    }

    auto target_avail_ptr_block = next_to_target_block;

    if (target_block_size - requested_block_size - serv_occup_block_size < serv_occup_block_size)
    {
        requested_block_size = target_block_size - serv_occup_block_size;
        // TODO: logs
    }

    *reinterpret_cast<size_t *>(target_block) = requested_block_size;
    if (prev_to_target_block == nullptr)
    {
        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_all_memory) + service_part_allocator_size - sizeof(void *)) = target_block;
        *get_pointer_prev_block(target_block) = nullptr;
    }
    else
    {
        *get_pointer_next_block(prev_to_target_block) = target_block;
        *get_pointer_prev_block(target_block) = prev_to_target_block;
    }

    *get_pointer_next_block(target_block) = next_to_target_block;

    if (next_to_target_block != nullptr)
    {
        *get_pointer_prev_block(next_to_target_block) = target_block;
    }

    dump_allocator_state(true);
    return reinterpret_cast<void *>(reinterpret_cast<void **>(reinterpret_cast<size_t *>(target_block) + 1) + 2);
}

void border_descriptors_memory::dump_allocator_state(bool is_allocate) const noexcept
{
    auto *log = get_logger();
    if (log == nullptr)
    {

        return;
    }

    auto allocator_memory_size = *reinterpret_cast<size_t *>(_ptr_to_memory);

    std::string constructed_state_string("|");
    auto *current_aviable_block = get_first_avail_block();
    auto *current_any_block = reinterpret_cast<unsigned char *>(_ptr_to_memory) + get_service_part_allocator_size();

    while (current_any_block - reinterpret_cast<unsigned char *>(_ptr_to_memory) < allocator_memory_size)
    {
        size_t any_block_size = 0;
        if (current_any_block == current_aviable_block)
        {
            constructed_state_string += "avlbl " + std::to_string(any_block_size = get_size_block(current_any_block)) + '|';
            any_block_size += get_service_part_avail_block_size();
            current_aviable_block = get_pointer_block(current_aviable_block);
        }
        else
        {
            constructed_state_string += "occpd " + std::to_string(any_block_size = get_size_block(current_any_block)) + '|';
            any_block_size += sizeof(size_t);
        }

        current_any_block += any_block_size;
    }

    log->log(std::string("Memory state after ") + (is_allocate ? "" : "de") + "allocation operation: begin" + constructed_state_string + "end", logger::severity::trace);
}

border_descriptors_memory::border_descriptors_memory(logger *log, memory *allocator, size_t size, allocate_mode method)
{

    auto target_size = size + get_service_part_allocator_size();
    _all_memory = reinterpret_cast<void *>(allocator != nullptr ? allocator->allocate(target_size) : ::operator new(target_size));

    auto total_size = reinterpret_cast<size_t *>(_all_memory);
    *total_size = size;
    auto log_block = reinterpret_cast<logger **>(total_size + 1);
    *log_block = log;
    auto mode_block = reinterpret_cast<allocate_mode *>(log_block + 1);
    *mode_block = method;
    auto alloc_block = reinterpret_cast<memory **>(mode_block + 1);
    *alloc_block = allocator;
    auto first_occup_next_block = reinterpret_cast<void **>(alloc_block + 1);
    *first_occup_next_block = nullptr;
}