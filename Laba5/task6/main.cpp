#include "long_number.h"
#include <iostream>
#include "string_number.h"

#include "../Task5/memory_buddy_system.h"
#include "../task4/boundary_tags_allocator.h"
#include "../task3/list_memory.h"

std::ostream &operator<<(
    std::ostream &stream,
    string_number *target_print)
{
    std::string to_print = target_print->get_string_number();
    int counter = 0;

    if (to_print[0] == 1 || to_print[0] == 0)
    {
        if (to_print[0] == 1)
        {
            stream << "-";
        }

        counter++;
    }

    for (; counter < to_print.size(); counter++)
    {
        stream << to_print[counter];
    }

    return stream;
}

int main()
{
    logger_builder *builder = new logger_builder_concrete();
    logger *log = builder
                      ->add_stream("file1.txt", logger::severity::trace)
                      ->add_stream("file2.txt", logger::severity::debug)
                      ->construct();

    memory *allocator = new border_descriptors_memory(log, nullptr, 1000000000, memory::allocate_mode::first_fit);
    string_number *res = new long_number("12000000000000000000000000");

    string_number *num2 = new long_number("7000000000000000");

    *res *= num2;

    int index = 1;
    std::cout << res << std::endl;
    // for (int i = 1; i <= 100; i++)
    // {
    //     bigint *temp = new long_number(std::to_string(i), allocator);

    //     *res += temp;
    //     delete temp;
    // }
    // std::cout << res << std::endl;
    delete res;
}
