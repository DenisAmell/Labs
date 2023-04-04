#include "allocator.h"
#include <iostream>
#include <cstring>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"
#include "memory_denis.h"

void *memory_denis::allocate(size_t target_size) const
{
    return ::operator new(target_size);
}

void memory_denis::deallocate(void const *const target_to_dealloc) const
{
    ::operator delete(const_cast<void *>(target_to_dealloc));
}

void memory_denis::debug_alloc(unsigned char *target_ptr, size_t size, logger *log)
{
    // std::stringstream ss;
    // ss << static_cast<void *>(target_ptr);
    // std::string add(ss.str());

    char str[20];
    memset(str, 0, 20);
    sprintf(str, "%p", target_ptr);
    std::string buff;
    for (int i = 0; i < size; i++)
    {
        int tmp = *target_ptr;
        buff.append(std::to_string(tmp) + ' ');
        target_ptr++;
        // buff.insert(i + 1, " ");
    }
    std::string add;
    add.append(str);
    std::string massege = "Block at address " + add + " state before deallocation:\n[ " + buff + "]";
    // Block at address 0x... state before deallocation: [ 49 50 51 52 53 54 55 56 57 48 0 ]
    log->log(massege, logger::severity::debug);
}

memory::~memory()
{
}