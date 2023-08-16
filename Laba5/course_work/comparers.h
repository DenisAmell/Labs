
#ifndef FUNDAMENTAL_ALGO_COMPARERS_H
#define FUNDAMENTAL_ALGO_COMPARERS_H
#include <iostream>

#include "student_definition.h"

class stdstring_comparer final
{
public:
    int operator()(
        std::string const &x,
        std::string const &y)
    {
        auto x_iterator = x.begin();
        auto y_iterator = y.begin();
        auto const x_end_iterator = x.cend();
        auto const y_end_iterator = y.cend();

        while (true)
        {
            auto const diff = *x_iterator - *y_iterator;
            if (diff != 0)
            {
                return diff;
            }

            ++x_iterator;
            ++y_iterator;

            if (x_iterator == x_end_iterator && y_iterator == y_end_iterator)
            {
                return 0;
            }
            else if (x_iterator == x_end_iterator)
            {
                return *y_iterator;
            }
            else if (y_iterator == y_end_iterator)
            {
                return *x_iterator;
            }
        }
    }
};

class int_comparer final
{
public:
    int operator()(
        int left,
        int right)
    {
        return left - right;
    }
};

class key_comparer
{

private:
    stdstring_comparer _string_comparer;

public:
    int operator()(key *first, key *second)
    {
        int comparison_result = first->id_session - second->id_session;
        if (comparison_result != 0)
        {
            return comparison_result;
        }

        comparison_result = first->id_student - second->id_student;
        if (comparison_result != 0)
        {
            return comparison_result;
        }

        // comparison_result = first->format - second->format;
        comparison_result = 0; // TODO:
        if (comparison_result != 0)
        {
            return comparison_result;
        }

        return _string_comparer(first->subject, second->subject);
    }
};
#endif // FUNDAMENTAL_ALGO_COMPARERS_H