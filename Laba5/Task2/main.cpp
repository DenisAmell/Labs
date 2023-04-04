#include "allocator.h"
#include "memory_denis.h"
#include <iostream>
#include <cstring>
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_concrete.h"

int main()
{
    memory_denis allocator;

    logger_builder *builder = new logger_builder_concrete;

    logger *constructed_logger = builder
                                     ->add_stream("file1.txt", logger::severity::critical)
                                     ->add_stream("file2.txt", logger::severity::debug)
                                     ->add_stream("file3.txt", logger::severity::trace)
                                     ->construct();
    int size = 25;
    int *p = reinterpret_cast<int *>(allocator.allocate(sizeof(int) * size));
    std::string log = "Allocated " + std::to_string(sizeof(int) * size) + " bytes";
    constructed_logger->log(log, logger::severity::trace);

    // log = "Adress " + p;
    // constructed_logger->log(log, logger::severity::trace);

    for (int i = 0; i < size; i++)
    {
        p[i] = i;

        std::cout << p[i] << " " << p + i << std::endl;
    }

    unsigned char *byte = reinterpret_cast<unsigned char *>(p);

    allocator.debug_alloc(byte, sizeof(int) * size, constructed_logger);
    allocator.deallocate(p);

    constructed_logger->log("Dellocated array", logger::severity::trace);

    delete constructed_logger;
    delete builder;

    return 0;
}