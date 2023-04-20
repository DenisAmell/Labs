#include <iostream>
#include <cmath>
#include "memory_buddy_system.h"

/*
    TODO: get_serv_avail_block;
    TODO: get_serv_occup_block;
    TODO: get_size_occup_blcok;
    TODO: get_size_avail_block;
    TODO: get_first_avail_block;
    TODO: get_pointer_prev_block;
    TODO: get_pointer_next_block;
    TODO: get_avail_block_next_block;
    TODO: get_trusted_memory_size;
    TODO: свободный или не свободный

*/

void *memory_buddy_system::allocate(size_t target_size) const
{

    auto alloc_method = *reinterpret_cast<mode *>(reinterpret_cast<unsigned char *>(_ptr_to_memory) + sizeof(size_t) + sizeof(logger *));

    void *prev_block = nullptr;
    void *curr_block = get_first_avail_block();

    void *prev_to_target_block = nullptr;
    void *target_block = nullptr;
    void *next_to_target_block = nullptr;

    size_t curr_size_block;

    while (curr_block != nullptr)
    {
        void *next_block = get_pointer_block(curr_block);
        curr_size_block = get_size_block(curr_block);

        if (curr_size_block + sizeof(void *) >= overridden_block_size)
        {
            if (alloc_method == mode::first_fit ||
                alloc_method == mode::best_fit && (target_block == nullptr || get_size_block(target_block) > curr_size_block) ||
                alloc_method == mode::worst_fit && (target_block == nullptr || get_size_block(target_block) < curr_size_block))
            {
                prev_to_target_block = prev_block;
                target_block = curr_block;
                next_to_target_block = next_block;
            }

            if (alloc_method == mode::first_fit)
            {
                break;
            }
        }

        prev_block = curr_block;
        curr_block = next_block;
    }

    if (target_block == nullptr)
    {
        throw std::bad_alloc();
    }

    auto avail_block_serv_part_size = get_service_part_avail_block_size();
    auto occup_block_serv_part_size = sizeof(size_t);
    size_t target_block_size = get_size_block(target_block);

    if (target_block_size + sizeof(void *) - overridden_block_size < avail_block_serv_part_size)
    {
        overridden_block_size = target_block_size + sizeof(void *);
        // TODO: logs
    }

    if (overridden_block_size != target_size)
    {
        // TODO: logs size has changed
        // this->debug_with_guard()
        target_size = overridden_block_size;
    }

    void *next_for_prev;

    if (target_block_size + avail_block_serv_part_size == target_size + occup_block_serv_part_size)
    {
        next_for_prev = next_to_target_block;
    }
    else
    {
        next_for_prev = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(target_block) + target_size + sizeof(size_t));
        *reinterpret_cast<size_t *>(next_for_prev) = target_block_size - target_size - sizeof(size_t);
        *reinterpret_cast<void **>(reinterpret_cast<size_t *>(next_for_prev) + 1) = next_to_target_block;
    }

    if (prev_to_target_block == nullptr)
    {
        *get_first_avail_block_ptr() = next_for_prev;
    }
    else
    {
        *reinterpret_cast<void **>(reinterpret_cast<size_t *>(prev_to_target_block) + 1) = next_for_prev;
    }

    *reinterpret_cast<size_t *>(target_block) = target_size;

    dump_allocator_state(true);

    // ----------------------------

    while ((curr_block_size >> 1) >= target_block_size + occup_block_serv_size && ((curr_block_size >> 1) >= target_block_size + occup_block_serv_size))
    {
        auto *const left_buddy_address = reinterpret_cast<unsigned char *>(target_block);
        auto *const right_buddy_address = left_buddy_address + (curr_size_block >>= 1);

        *right_buddy_address = *left_buddy_address = ((*left_buddy_address >> 1) - 1) << 1;

        *(reinterpret_cast<void **>(right_buddy_address + 1) + 1) = get_avail_block_next_block(left_buddy_address);
        *(reinterpret_cast<void **>(left_buddy_address + 1) + 1) = get_avail_block_next_block(right_buddy_address);
        *reinterpret_cast<void **>(right_buddy_address + 1) = reinterpret_cast<void *>(left_buddy_address);
    }

    auto *const target_block_size_space = *reinterpret_cast<unsigned char *>(target_block);
    *target_block_size_space = compact_block_size_and_block_occup(static_cast<unsigned char>(std::log2(curr_size_block)), true);

    return reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(target_block) + 1);
}