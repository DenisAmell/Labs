#include "allocator.h"
#include <iostream>
#include <cstring>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"
#include "memory_denis.h"

memory_denis::memory_denis(
    logger *log)
    : _log(log)
{
}

void *memory_denis::allocate(size_t target_size) const
{
    auto *result = ::operator new(sizeof(size_t) + target_size);
    auto *result_size = reinterpret_cast<size_t *>(result);
    *result_size = target_size;
    return result_size + 1;
}

void memory_denis::deallocate(void const *const target_to_dealloc) const
{
    auto *block_size_addr = reinterpret_cast<size_t *>(const_cast<void *>(target_to_dealloc)) - 1;
    auto block_size = *block_size_addr;
    debug_alloc(target_to_dealloc, block_size, _log);
    ::operator delete(block_size_addr);
}

void memory_denis::debug_alloc(const void *target_ptr, size_t size, logger *log) const
{
    // std::stringstream ss;
    // ss << static_cast<void *>(target_ptr);
    // std::string add(ss.str());
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

memory::~memory()
{
}