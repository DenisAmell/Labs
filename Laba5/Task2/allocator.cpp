#include "allocator.h"
#include <iostream>

void *memory::allocate(size_t target_size) const
{
    int *p = new int[target_size];
}

memory::~memory()
{
}