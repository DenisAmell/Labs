#include "long_number.h"
#include <string>
#include <sstream>
void *long_number::to_base(std::string const &str_number, memory *allocator)
{
    unsigned char *number_base = nullptr;

    number_base = reinterpret_cast<unsigned char *>(allocator->allocate(sizeof(memory *) + 2 * sizeof(int)));
    *reinterpret_cast<memory **>(number_base) = allocator;
    *reinterpret_cast<int *>(number_base + sizeof(memory *)) = 1;
    *reinterpret_cast<int *>(number_base + sizeof(memory *) + sizeof(int)) = std::atoi(str_number.c_str());

    return reinterpret_cast<void *>(number_base);
}

long_number::long_number(std::string const &str_number, memory *allocator)
{
    _target_number = to_base(str_number, allocator);
    // if (is_validate(str_number))
    // {
    //     _target_number = to_base(str_number, allocator);
    // }
    // else
    // {
    //     throw std::invalid_argument("Invalid number");
    // }
}

bigint *bigint::add(bigint const *summand)
{
    return this;
}
bigint *bigint::subtract(bigint const *const subtrahend)
{
    return this;
}
bool bigint::lower_than(bigint const *const other) const
{
    return false;
}
bool bigint::greater_than(bigint const *const other) const
{
    return false;
}
bool bigint::lower_than_or_equal_to(bigint const *const other) const
{
    return false;
}
bool bigint::greater_than_or_equal_to(bigint const *const other) const
{
    return false;
}
bool bigint::equals(bigint const *const other) const
{
    return false;
}
bool bigint::not_equals(bigint const *const other) const
{
    return false;
}
