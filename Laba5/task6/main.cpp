#include "long_number.h"
#include <iostream>

#include "../Task5/memory_buddy_system.h"
#include "../task4/boundary_tags_allocator.h"
#include "../task3/list_memory.h"

int main()
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
                      ->add_stream("file1.txt", logger::severity::trace)
                      ->add_stream("file2.txt", logger::severity::debug)
                      ->construct();

    memory *allocator = new border_descriptors_memory(log, nullptr, 1000000, memory::allocate_mode::first_fit);
    bigint *num = new long_number("10", allocator);

    delete num;
}
