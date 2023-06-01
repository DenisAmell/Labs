#include "red_black_tree.h"
#include "../../Task5/memory_buddy_system.h"
#include "../../Task4/boundary_tags_allocator.h"
#include "../../Task3/list_memory.h"
#include <iostream>

class key_comparer
{
public:
    int operator()(int first, int second)
    {
        return first - second;
    }

    int operator()(std::string first, std::string second)
    {
        if (first > second)
            return 1;
        else if (first < second)
            return -1;
        else
            return 0;
    }
};

int main()
{
    logger_builder* builder = new logger_builder_concrete();
    logger* log = builder
        ->add_stream("file1.txt", logger::severity::trace)
        ->add_stream("file2.txt", logger::severity::debug)
        ->construct();

    memory* allocator = new border_descriptors_memory(log, nullptr, 10000, memory::allocate_mode::first_fit);

    auto* rb_tree = new red_black_tree<int, std::string, key_comparer>(allocator, log);

    //  rb_tree->insert(1, "a");

    delete rb_tree;
    delete allocator;
    delete log;
    delete builder;
}