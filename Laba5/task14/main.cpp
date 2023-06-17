#include "../Task5/memory_buddy_system.h"
#include "../task4/boundary_tags_allocator.h"
#include "../task3/list_memory.h"
#include "../Task11/binary_search_tree.h"
#include "splay_tree.h"
#include <iostream>

class functor_int
{
public:
    int operator()(int first, int second)
    {
        return first - second;
    }
};

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
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
                      ->add_stream("file1.txt", logger::severity::trace)
                      ->add_stream("file2.txt", logger::severity::debug)
                      ->construct();

    memory *allocator = new border_descriptors_memory(log, nullptr, 100000, memory::allocate_mode::first_fit);

    associative_container<int, std::string> *splay = new splay_tree<int, std::string, key_comparer>(allocator, log);

    splay->insert(1, std::move(std::string("a")));
    std::cout << "insert complated" << std::endl;
    splay->insert(2, std::move(std::string("b")));
    std::cout << "insert complated" << std::endl;
    splay->insert(15, std::move(std::string("o")));
    std::cout << "insert complated" << std::endl;
    splay->insert(3, std::move(std::string("b")));
    std::cout << "insert complated" << std::endl;
    splay->insert(4, std::move(std::string("a")));
    std::cout << "insert complated" << std::endl;

    // auto ins = associative_container<int, std::string>::key_and_value_pair{6, std::move("c")};
    // *bst += ins;

    splay->remove(15);
    // avl->remove(2);
    auto tree = *reinterpret_cast<binary_search_tree<int, std::string, key_comparer> *>(splay);

    auto end_infix = tree.end_infix();

    for (auto it = tree.begin_infix(); it != end_infix; ++it)
    {
        for (auto i = 0; i < std::get<0>(*it); i++)
        {
            std::cout << "  ";
        }
        std::cout << "Key = \"" << std::get<1>(*it) << "\", Value = \"" << std::get<2>(*it) << "\"" << std::endl;
        // std::cout << "lol" << std::endl;
        // for (int i = 0; i < std::get<0>(*it); i++)
        // {
        //     std::cout << "  ";
        // }
        // std::cout << std::get<2>(*it) << " " << std::endl;
    }
    std::cout << std::endl;

    delete splay;

    return 0;
}