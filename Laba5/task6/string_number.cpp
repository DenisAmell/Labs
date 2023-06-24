#include "string_number.h"

bool string_number::operator<(string_number const *other) const
{
    return lower_than_string(other);
}

bool string_number::operator>(string_number const *other) const
{
    return greater_than_string(other);
}

string_number *string_number::operator/=(const unsigned long &divend)
{
    return div(divend);
}
string_number *string_number::operator/(const unsigned long &divend)
{
    return division(divend);
}

string_number *string_number::operator%=(const unsigned long &target)
{
    return mod(target);
}

string_number *string_number::operator%(
    const unsigned long &target)
{
    return module(target);
}

string_number *string_number::operator*=(const string_number *other)
{
    return multiply(other);
}

string_number *string_number::operator*(const string_number *other) const
{
    return multiplication(other);
}

string_number *string_number::operator^(unsigned long other)
{
    return pow(other);
}

string_number *string_number::operator+=(
    const string_number *other)
{
    return add_string(other);
}

string_number *string_number::operator+(
    const string_number *other) const
{
    return sum_string(other);
}

string_number *string_number::operator-=(
    const string_number *other)
{
    return subtract_string(other);
}

string_number *string_number::operator-(
    const string_number *other) const
{
    return subtraction_string(other);
}