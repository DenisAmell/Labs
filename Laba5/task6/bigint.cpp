#include "bigint.h"

bigint *bigint::sum(bigint const *summand) const
{
    throw std::exception();
}

bigint *bigint::subtraction(bigint const *const subtrahend) const
{
    throw std::exception();
}
bigint *bigint::operator+=(bigint const *summand)
{
    return add(summand);
}

bigint *bigint::operator+(bigint const *summand)
{
    return sum(summand);
}

bigint *bigint::operator-=(bigint const *subtrahend)
{
    return subtract(subtrahend);
}

bigint *bigint::operator-(bigint const *subtrahend)
{
    return subtraction(subtrahend);
}

bool bigint::operator<(bigint const *other)
{
    return lower_than(other);
}

bool bigint::operator>(bigint const *other)
{
    return greater_than(other);
}

bool bigint::operator<=(bigint const *other)
{
    return lower_than_or_equal_to(other);
}

bool bigint::operator>=(bigint const *other)
{
    return greater_than_or_equal_to(other);
}

bool bigint::operator==(bigint const *other)
{
    return equals(other);
}

bool bigint::operator!=(bigint const *other)
{
    return not_equals(other);
}
