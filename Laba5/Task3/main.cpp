#include <iostream>
#include "allocator2.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"
#include "../Task2/memory_denis.h"
#include "../Task2/allocator.h"
#include <cstring>
int main()
{
    logger_builder *builder = new logger_builder_concrete;
    logger *log = builder->add_stream("file1.txt", logger::severity::debug)->construct();
    void *ptr;
    memory *allocator = new memory_denis(log);
    memory *allocator2 = new My_memory(log, allocator, 1000, My_memory::mode::first_fit);

    int *p = reinterpret_cast<int *>(allocator2->allocate(sizeof(int) * 25));

    for (int i = 0; i < 25; i++)
    {
        p[i] = i;
        std::cout << p[i] << std::endl;
    }

    // int *p2 = reinterpret_cast<int *>(allocator2->allocate(sizeof(int) * 25));
    // int k = 25;
    // for (int i = 0; i < 25; i++)
    // {
    //     p2[i] = k;
    //     k++;
    //     std::cout << p2[i] << std::endl;
    // }

    char *text = reinterpret_cast<char *>(allocator2->allocate(sizeof(char) * 11));
    strcpy(text, "1234567890");
    delete allocator2;
    delete allocator;
    delete log;
    delete builder;
}