#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <list>

class bigint
{
public:
    virtual bigint *add(bigint const *summand) = 0;
    virtual bigint *sum(bigint const *summand) const;

    bigint *operator+=(bigint const *summand);
    bigint *operator+(bigint const *summand);

public:
    virtual bigint *subtract(bigint const *const subtrahend) = 0;
    virtual bigint *subtraction(bigint const *const subtrahend) const;

    bigint *operator-=(bigint const *subtrahend);
    bigint *operator-(bigint const *subtrahend);

public:
    virtual bool lower_than(bigint const *other) = 0;
    virtual bool greater_than(bigint const *other) = 0;
    virtual bool lower_than_or_equal_to(bigint const *other) = 0;
    virtual bool greater_than_or_equal_to(bigint const *other) = 0;

    bool operator<(bigint const *other);
    bool operator>(bigint const *other);
    bool operator<=(bigint const *other);
    bool operator>=(bigint const *other);

public:
    virtual bool equals(bigint const *other) = 0;
    virtual bool not_equals(bigint const *other) = 0;

    bool operator==(bigint const *other);
    bool operator!=(bigint const *other);

    friend std::ostream &operator<<(
        std::ostream &,
        bigint *);

public:
    virtual void *get_number() const = 0;
    // virtual void *set_number(std::vector<unsigned long int> result, memory *allocator) = 0;

public:
    virtual ~bigint() = default;
};

#endif // BIGINT_H