#include "allocator.h"
#include <iostream>
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
memory::~memory()
{
}