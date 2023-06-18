#ifndef LONG_NUMBER_H
#define LONG_NUMBER_H

#include "bigint.h"
#include "../memory/memory.h"
#include "../memory/memory_holder.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_builder.h"
#include "../logger/log2/logger_builder_concrete.h"
#include "../logger/log2/logger_holder.h"

class long_number final : public bigint
{
private:
    void *_target_number;
    // static auto _base = (static_cast<unsigned long>(1) << (sizeof(int) << 3));

public:
    bigint *add(bigint const *summand) override;
    bigint *subtract(bigint const *const subtrahend) override;

public:
    bool lower_than(bigint const *const other) const override;
    bool greater_than(bigint const *const other) const override;
    bool lower_than_or_equal_to(bigint const *const other) const override;
    bool greater_than_or_equal_to(bigint const *const other) const override;

public:
    bool equals(bigint const *const other) const override;
    bool not_equals(bigint const *const other) const override;

protected:
    // bool is_validate(std::string &number);
    void *to_base(std::string const &str_number, memory *allocator);

public:
    long_number(std::string const &str_number, memory *allocator);

    ~long_number() = default;
};

#endif // LONG_NUMBER_H