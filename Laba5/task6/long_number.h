#ifndef LONG_NUMBER_H
#define LONG_NUMBER_H

#define BASE (static_cast<unsigned long>(1) << (sizeof(int) << 3))
#define UPPER_BOUND (((static_cast<long>(1)) << ((sizeof(int) << 3) - 1)) - 1)
#define LOWER_BOUND (-1 * (static_cast<long>(1)) << ((sizeof(int) << 3) - 1))

#include "bigint.h"
#include "string_number.h"
#include "../memory/memory.h"
#include "../memory/memory_holder.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_holder.h"
#include <string>
#include <vector>

class long_number : public bigint, public string_number
{
private:
    void *_target_number;
    unsigned long int _base = (static_cast<unsigned long int>(1) << (sizeof(int) << 3));
    std::string _string_number;

public:
    bigint *add(bigint const *summand) override;
    bigint *sum(bigint const *summand) const override;

    string_number *add_string(
        const string_number *target) override;
    string_number *sum_string(
        const string_number *target) const override;

    bigint *subtract(bigint const *const subtrahend) override;
    bigint *subtraction(bigint const *const subtrahend) const override;

    string_number *subtract_string(
        const string_number *target) override;
    string_number *subtraction_string(
        const string_number *target) const override;

public:
    string_number *div(const unsigned long &divend) override;
    string_number *division(const unsigned long &divend) override;
    string_number *mod(const unsigned long &target) override;
    string_number *module(const unsigned long &target) override;

    bool lower_than_string(string_number const *other) const override;
    bool greater_than_string(string_number const *other) const override;

public:
    string_number *multiply(const string_number *target) override;

    string_number *multiplication(const string_number *target) const override;

    string_number *pow(unsigned long step) override;

    std::string karatsuba(
        std::string number,
        std::string multiplier);

    std::string naiv_mult(
        std::string number,
        std::string multiplier);

    size_t make_equal_length(
        std::string &number_1,
        std::string &number_2);

public:
    bool lower_than(bigint const *other) override;
    bool greater_than(bigint const *other) override;
    bool lower_than_or_equal_to(bigint const *other) override;
    bool greater_than_or_equal_to(bigint const *other) override;

public:
    bool equals(bigint const *other) override;
    bool not_equals(bigint const *other) override;

protected:
    bool is_validate(std::string const &number);
    void *to_base(std::string const &str_number, memory *allocator);
    bool gets_into_int(const std::string &number);
    void *get_number() const override;
    std::string get_string_number() const override;
    void *set_number(std::vector<unsigned long int> result, memory *allocator);
    std::vector<unsigned long int> get_vector(
        void *other) const;
    bool is_int_number(void *number);
    size_t get_size() const;
    unsigned long int *get_array_digits() const;

public:
    std::string to_ten();

public:
    explicit long_number(void *target_num);
    long_number(std::string const &str_number);

    friend std::ostream &operator<<(
        std::ostream &,
        long_number *);

    long_number(std::string const &str_number, memory *allocator);

    ~long_number() = default;
};

#endif // LONG_NUMBER_H