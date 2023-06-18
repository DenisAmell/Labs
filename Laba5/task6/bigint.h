#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>

class bigint
{
public:
    virtual bigint *add(bigint const *summand) = 0;
    bigint *sum(bigint const *summand) const;

    bigint *operator+=(bigint const *summand);
    bigint *operator+(bigint const *summand);

public:
    virtual bigint *subtract(bigint const *const subtrahend) = 0;
    bigint *subtraction(bigint const *const subtrahend) const;

    bigint *operator-=(bigint const *subtrahend);
    bigint *operator-(bigint const *subtrahend);

public:
    virtual bool lower_than(bigint const *const other) const = 0;
    virtual bool greater_than(bigint const *const other) const = 0;
    virtual bool lower_than_or_equal_to(bigint const *const other) const = 0;
    virtual bool greater_than_or_equal_to(bigint const *const other) const = 0;

    bool operator<(bigint const *const other) const;
    bool operator>(bigint const *const other) const;
    bool operator<=(bigint const *const other) const;
    bool operator>=(bigint const *const other) const;

public:
    virtual bool equals(bigint const *const other) const = 0;
    virtual bool not_equals(bigint const *const other) const = 0;

    bool operator==(bigint const *const other) const;
    bool operator!=(bigint const *const other) const;

public:
    virtual ~bigint() = default;
};

#endif // BIGINT_H