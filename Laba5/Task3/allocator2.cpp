#include "allocator2.h"
#include <cstring>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"
#include "../Task2/memory_denis.h"

void *My_memory::get_pointer(void *target_ptr) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<size_t *>(target_ptr) + 1);
}

size_t My_memory::get_size_block(void *target_ptr) const
{
    return *reinterpret_cast<size_t *>(target_ptr);
}

// void *My_memory::get_pointer_block(void *target_ptr)
// {
//     return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(target_ptr) + sizeof(void *));
// }

void *My_memory::allocate(size_t target_size) const
{
    auto alloc_method = reinterpret_cast<mode *>(reinterpret_cast<unsigned char *>(_ptr) + sizeof(logger *));
    auto first_avail_block = reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(alloc_method) + sizeof(memory *) + sizeof(mode));

    void *prev_block = nullptr;
    void *curr_block = first_avail_block;

    void *prev_to_target_block = nullptr;
    void *target_block = nullptr;
    void *next_to_target_block = nullptr;

    size_t curr_size_block;
    size_t next_size_block;

    while (curr_block != nullptr)
    {
        void *next_block = get_pointer(curr_block);
        curr_size_block = get_size_block(curr_block);
        // std::cout << curr_size_block << std::endl;
        if (curr_size_block + sizeof(void *) >= target_size)
        {
            if (*alloc_method == mode::first_fit ||
                *alloc_method == mode::best_fit && (target_block == nullptr || get_size_block(target_block) > curr_size_block) ||
                *alloc_method == mode::worst_fit && (target_block == nullptr || get_size_block(target_block) < curr_size_block))
            {
                prev_to_target_block = prev_block;
                target_block = curr_block;
                next_to_target_block = next_block;
            }
        }

        if (*alloc_method == mode::first_fit)
        {
            break;
        }

        prev_block = curr_block;
        curr_block = next_block;
    }

    if (target_block == nullptr)
    {
        // TODO: throw an exception
    }

    return target_block;
}

void My_memory::deallocate(void const *const target_to_dealloc) const
{
}

void My_memory::debug_alloc(const void *target_ptr, size_t size, logger *log) const
{
    unsigned char *ptr = reinterpret_cast<unsigned char *>(const_cast<void *>(target_ptr));
    char str[20];
    memset(str, 0, 20);
    sprintf(str, "%p", target_ptr);
    std::string buff;
    for (int i = 0; i < size; i++)
    {
        unsigned short tmp = static_cast<unsigned short>(*ptr);
        buff.append(std::to_string(tmp) + ' ');
        ptr++;
        // buff.insert(i + 1, " ");
    }
    std::string add;
    add.append(str);
    std::string massege = "Block at address " + add + " state before deallocation:\n[ " + buff + "]";
    // Block at address 0x... state before deallocation: [ 49 50 51 52 53 54 55 56 57 48 0 ]
    log->log(massege, logger::severity::debug);
}

My_memory::My_memory(logger *log, memory *allocator, size_t size, mode method)
{
    // TODO: TRY

    _ptr = reinterpret_cast<void *>(allocator != nullptr ? allocator->allocate(size) : ::operator new(size));

    auto log_block = reinterpret_cast<logger **>(_ptr);
    *log_block = log;
    auto mode_block = reinterpret_cast<mode *>(log_block + 1);
    *mode_block = method;
    auto alloc_block = reinterpret_cast<memory **>(mode_block + 1);
    *alloc_block = allocator;
    auto first_avail_block_block = reinterpret_cast<void **>(alloc_block + 1);
    *first_avail_block_block = first_avail_block_block + 1;
    auto target_size = size - sizeof(logger *) - sizeof(mode) - sizeof(memory *) - 2 * sizeof(void *) - sizeof(size_t);
    auto *tmp = reinterpret_cast<size_t *>(*first_avail_block_block);
    *tmp = target_size;
    *reinterpret_cast<void **>(tmp + 1) = nullptr;

    std::cout << *tmp << std::endl;
}
My_memory::~My_memory()
{
    if (_ptr == nullptr)
        return;
    // auto log_block = reinterpret_cast<logger **>(_ptr);
    auto allocator_block = reinterpret_cast<memory **>(reinterpret_cast<unsigned char *>(_ptr) + sizeof(logger *) + sizeof(mode));
    // debug_alloc(_ptr, sizeof(int) * 100, *log_block);
    *allocator_block == nullptr ? ::operator delete(_ptr) : (*allocator_block)->deallocate(_ptr);
}
