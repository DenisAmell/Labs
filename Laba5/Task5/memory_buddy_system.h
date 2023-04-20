#ifndef MEMORY_BUDDY_SYSTEM_H
#define MEMORY_BUUDDY_SYSTEM_H

#include "memory.h"

class memory_buddy_system final : public memory
{

private:
    void *_all_memory;

public:
    virtual void *allocate(size_t target_size) const override;

public:
    virtual void deallocate(void const *const target_to_dealloc) const override;

public:
    virtual ~memory_buddy_system();
};

#include // MEMORY_BUUDDY_SYSTEM_H