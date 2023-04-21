#include <iostream>
#include "list_memory.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"
#include "memory.h"
#include <cstring>
#include <list>

void allocator_demo_2()
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
                      ->add_stream("file1.txt", logger::severity::trace)
                      ->add_stream("file2.txt", logger::severity::debug)
                      ->construct();

    memory *allocator = new list_memory(log, nullptr, 1000000, list_memory::allocate_mode::first_fit);

    auto *ptr = allocator->allocate(100);
    auto *ptr2 = allocator->allocate(100);
    allocator->deallocate(ptr);
    ptr = allocator->allocate(99);

    delete allocator;
    delete log;
    delete builder;
}

int main()
{
    // logger_builder *builder = new logger_builder_concrete();
    // logger *log = builder
    //                   ->add_stream("file1.txt", logger::severity::trace)
    //                   ->add_stream("file2.txt", logger::severity::debug)
    //                   //->add_stream("console", logger::severity::trace)
    //                   ->construct();
    // void *ptr;
    // memory *all2 = new list_memory(log, nullptr, 1000000, My_memory::mode::first_fit);

    // // std::list<void *> allocated_blocks;
    // // srand((unsigned)time(nullptr));

    // // for (int i = 0; i < 1000; i++)
    // // {
    // //     // std::cout << "Iteration #" << i << ":" << std::endl;
    // //     switch (rand() % 2)
    // //     {
    // //     case 0:
    // //         try
    // //         {
    // //             allocated_blocks.push_back(all2->allocate(rand() % 201 + 150));
    // //         }
    // //         catch (std::bad_alloc const &ex)
    // //         {
    // //             std::cout << i << " allocate failed" << std::endl;
    // //         }
    // //         break;
    // //     case 1:
    // //         try
    // //         {
    // //             if (allocated_blocks.empty())
    // //             {
    // //                 // std::cout << i << " nothing to deallocate" << std::endl;
    // //                 continue;
    // //             }

    // //             auto ptr_to_deallocate_index = rand() % allocated_blocks.size();
    // //             auto it = allocated_blocks.begin();
    // //             std::advance(it, ptr_to_deallocate_index);
    // //             all2->deallocate(*it);
    // //             allocated_blocks.erase(it);
    // //         }
    // //         catch (My_memory::dealloc_fail const &ex)
    // //         {
    // //             std::cout << i << " deallocate failed" << std::endl;
    // //         }
    // //         break;
    // //     }
    // // }

    // // while (!allocated_blocks.empty())
    // // {
    // //     auto ptr_to_deallocate_index = rand() % allocated_blocks.size();
    // //     auto it = allocated_blocks.begin();
    // //     std::advance(it, ptr_to_deallocate_index);
    // //     all2->deallocate(*it);
    // //     allocated_blocks.erase(it);
    // // }

    // int *p2 = reinterpret_cast<int *>(all2->allocate(sizeof(int) * 25));
    // int k = 25;
    // for (int i = 0; i < 25; i++)
    // {
    //     p2[i] = k;
    //     k++;
    //     std::cout << p2[i] << std::endl;
    // }

    // all2->deallocate(p2);

    // char *text = reinterpret_cast<char *>(all2->allocate(sizeof(char) * 11));
    // strcpy(text, "1234567890");

    // all2->deallocate(text);
    // delete all2;
    // delete log;
    // delete builder;

    allocator_demo_2();
}