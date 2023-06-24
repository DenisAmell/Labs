#ifndef STRING_NUMBER_H
#define STRING_NUMBER_H
#include <string>
class string_number
{

public:
    virtual string_number *add_string(
        const string_number *target) = 0;

    virtual string_number *sum_string(
        const string_number *target) const = 0;

    string_number *operator+=(
        const string_number *other);

    string_number *operator+(
        const string_number *other) const;

public:
    virtual string_number *subtract_string(
        const string_number *target) = 0;

    virtual string_number *subtraction_string(
        const string_number *target) const = 0;

    string_number *operator-=(
        const string_number *other);

    string_number *operator-(
        const string_number *other) const;

public:
    virtual string_number *div(const unsigned long &divend) = 0;
    virtual string_number *division(const unsigned long &divend) = 0;

    string_number *operator/=(const unsigned long &divend);
    string_number *operator/(const unsigned long &divend);

    virtual string_number *mod(
        const unsigned long &target) = 0;

    string_number *operator%=(const unsigned long &target);

    virtual string_number *module(const unsigned long &target) = 0;

    string_number *operator%(const unsigned long &target);

    virtual std::string get_string_number() const = 0;

public:
    virtual string_number *multiply(const string_number *target) = 0;

    string_number *operator*=(const string_number *other);

    virtual string_number *multiplication(const string_number *target) const = 0;

    string_number *operator*(const string_number *other) const;

    virtual string_number *pow(unsigned long step) = 0;

    string_number *operator^(unsigned long other);

public:
    virtual bool lower_than_string(string_number const *other) const = 0;
    virtual bool greater_than_string(string_number const *other) const = 0;
    bool operator<(string_number const *other) const;
    bool operator>(string_number const *other) const;

    friend std::ostream &operator<<(
        std::ostream &,
        string_number *);
    virtual ~string_number() = default;
};

#endif // STRING_NUMBER_H