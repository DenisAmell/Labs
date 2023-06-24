#include "long_number.h"
#include <cstring>

void *long_number::get_number() const
{
    return _target_number;
}

std::string long_number::get_string_number() const
{
    return _string_number;
}

size_t long_number::get_size() const
{
    return *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *));
}

unsigned long int *long_number::get_array_digits() const
{
    return reinterpret_cast<unsigned long int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t) + sizeof(int));
}

bool long_number::is_validate(std::string const &number)
{
    std::string temp = number;

    if (number.length() == 0 ||
        number[0] == '+')
    {
        return false;
    }

    if (number[0] == '-')
    {
        temp = temp.substr(1, temp.length() - 1);
    }

    for (char symbol : temp)
    {
        if (!isdigit(symbol))
        {
            return false;
        }
    }

    return true;
}

bool long_number::gets_into_int(
    const std::string &number)
{

    string_number *upper_bound = new long_number(std::to_string(UPPER_BOUND));
    string_number *lower_bound = new long_number(std::to_string(LOWER_BOUND));
    string_number *this_number = new long_number(number);

    if ((*this_number < upper_bound) && (*this_number > lower_bound))
    {

        delete upper_bound;
        delete lower_bound;
        delete this_number;

        return true;
    }

    delete upper_bound;
    delete lower_bound;
    delete this_number;

    return false;
}

std::string long_number::to_ten()
{
    int sign_this = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t));
    size_t len_number = get_size();

    string_number *string_base = new long_number(std::to_string(_base));
    string_number *result = new long_number("0");

    if (is_int_number(_target_number))
    {
        delete string_base;
        delete result;
        std::string gg = std::to_string(sign_this);
        // std::cout << gg[2] << std::endl;
        return std::to_string(sign_this);
    }

    unsigned long int *array_digits = get_array_digits();
    size_t size = get_size();
    std::cout << size << std::endl;
    for (int i = 0; i < size; i++)
    {
        string_number *temp = new long_number(std::to_string(array_digits[i]));
        string_number *pow_res = *string_base ^ (size - i - 1);
        string_number *res = *pow_res * temp;

        *result += res;

        std::cout << result << std::endl;
        delete temp;
        delete pow_res;
        delete res;
    }

    std::string str_result = sign_this == 1 ? "-" + result->get_string_number() : get_string_number();

    delete string_base;
    delete result;
    return str_result;
}

void *long_number::to_base(std::string const &str_number, memory *allocator)
{

    unsigned char *number_base = nullptr;

    if (gets_into_int(str_number))
    {

        number_base = reinterpret_cast<unsigned char *>(allocator->allocate(sizeof(memory *) + sizeof(size_t) + sizeof(int)));
        *reinterpret_cast<memory **>(number_base) = allocator;
        *reinterpret_cast<size_t *>(number_base + sizeof(memory *)) = 1;
        *reinterpret_cast<int *>(number_base + sizeof(memory *) + sizeof(size_t)) = std::atoi(str_number.c_str());

        return reinterpret_cast<void *>(number_base);
    }

    int sign = 0;
    unsigned int remainder = 0;
    size_t size_number = 1;
    std::string target_number_str = str_number;

    if (target_number_str[0] == '-')
    {
        sign = 1;
        target_number_str = target_number_str.substr(1, target_number_str.length() - 1);
    }

    string_number *string_base = new long_number(std::to_string(_base));
    string_number *temp = new long_number(str_number);

    while (*temp > string_base)
    {
        *temp /= _base;
        size_number++;
    }

    delete temp;
    number_base = reinterpret_cast<unsigned char *>(allocator->allocate(sizeof(memory *) + sizeof(size_t) + sizeof(int) + sizeof(unsigned long int) * size_number));
    *reinterpret_cast<memory **>(number_base) = allocator;
    *reinterpret_cast<size_t *>(number_base + sizeof(memory *)) = size_number;
    *reinterpret_cast<int *>(number_base + sizeof(memory *) + sizeof(size_t)) = sign;

    unsigned int counter = 1;
    auto *array_digits = reinterpret_cast<unsigned long int *>(number_base + sizeof(memory *) + sizeof(size_t) + sizeof(int));

    string_number *this_number = new long_number(str_number);

    while (*this_number > string_base)
    {
        string_number *mod_res = *this_number % _base;

        remainder = static_cast<unsigned int>(std::atol((mod_res)->get_string_number().c_str()));
        *this_number /= _base;
        array_digits[size_number - counter++] = remainder;

        delete mod_res;
    }

    array_digits[size_number - counter] = static_cast<unsigned int>(std::atol(this_number->get_string_number().c_str()));

    // std::cout << array_digits[0] << std::endl;

    // *reinterpret_cast<unsigned int **>(reinterpret_cast<unsigned char *>(number_base) + sizeof(memory *) + sizeof(size_t) + sizeof(int)) = array_digits;

    delete this_number;
    delete string_base;
    return number_base;
}

long_number::long_number(std::string const &str_number, memory *allocator)
{
    //_target_number = to_base(str_number, allocator);

    // std::cout << *reinterpret_cast<unsigned long int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t)) << std::endl;
    if (is_validate(str_number))
    {
        _target_number = to_base(str_number, allocator);
    }
    else
    {
        throw std::invalid_argument("Invalid number");
    }
}

long_number::long_number(std::string const &str_number)
{
    _string_number = str_number;
}

long_number::long_number(void *target_num)
{
    memory *alc = *reinterpret_cast<memory **>(target_num);

    if (is_int_number(target_num))
    {
        size_t to_allocate = sizeof(memory *) + sizeof(size_t) + sizeof(int);
        _target_number = alc->allocate(to_allocate);
        memcpy(_target_number, target_num, to_allocate);
    }
    else
    {
        size_t len_number = *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(target_num) + sizeof(memory *));
        size_t to_allocate = sizeof(memory *) + sizeof(size_t) + sizeof(int) + len_number * sizeof(unsigned int);
        _target_number = alc->allocate(to_allocate);
        memcpy(_target_number, target_num, to_allocate);
    }
}

void *long_number::set_number(std::vector<unsigned long int> result, memory *allocator)
{
    void *new_number = nullptr;
    std::string str_number = (result[0] == 1) ? ("-" + std::to_string(result[1])) : (std::to_string(result[1]));

    if (result.size() == 2 && gets_into_int(str_number))
    {
        new_number = allocator->allocate(sizeof(memory *) + sizeof(size_t) + sizeof(int));
        *reinterpret_cast<memory **>(new_number) = allocator;
        *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(new_number) + sizeof(memory *)) = result.size() - 1;
        *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(new_number) + sizeof(memory *) + sizeof(size_t)) = std::atoi(str_number.c_str());
    }
    else
    {
        new_number = allocator->allocate(sizeof(memory *) + sizeof(size_t) + sizeof(int) + sizeof(unsigned int) * (result.size() - 1));
        *reinterpret_cast<memory **>(new_number) = allocator;
        *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(new_number) + sizeof(memory *)) = result.size() - 1;
        *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(new_number) + sizeof(memory *) + sizeof(size_t)) = result[0];
        auto *array_digits = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(new_number) + sizeof(memory *) + sizeof(size_t) + sizeof(int));

        for (int i = 1; i < result.size(); i++)
        {
            array_digits[i - 1] = result[i];
        }
    }

    return new_number;
}

bool long_number::is_int_number(void *number)
{
    return *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(number) - 2 * sizeof(void *) - sizeof(size_t)) == sizeof(memory *) + sizeof(size_t) + sizeof(int);
}

std::vector<unsigned long int> long_number::get_vector(
    void *other) const
{
    auto *begin = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(other) + sizeof(memory *) + sizeof(size_t));
    auto *end = begin + get_size() + 1;
    std::vector<unsigned long int> result(begin, end);

    return result;
}

bigint *long_number::add(bigint const *summand)
{
    int sign_this = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t));
    int sign_other = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(summand->get_number()) + sizeof(memory *) + sizeof(size_t));
    std::vector<unsigned long int> result;
    std::vector<unsigned long int> number_representation;
    std::vector<unsigned long int> added;
    unsigned int remainder = 0;

    if (is_int_number(_target_number))
    {
        number_representation.push_back(sign_this < 0);
        number_representation.push_back(static_cast<unsigned long int>(abs(sign_this)));
    }
    else
    {

        number_representation = get_vector(_target_number);

        std::cout << number_representation[1] << std::endl;
    }

    if (is_int_number(summand->get_number()))
    {
        added.push_back(sign_other < 0);
        added.push_back(static_cast<unsigned long int>(abs(sign_other)));
    }
    else
    {
        added = get_vector(summand->get_number());
    }

    if (added[0] == 1 && number_representation[0] == 1)
    {
        result.push_back(1);
    }
    else
    {
        if (added[0] == 0 && number_representation[0] == 0)
        {
            result.push_back(0);
        }
        else
        {
            bigint *copy_number = new long_number(summand->get_number());
            memory *alc = *reinterpret_cast<memory **>(summand->get_number());
            void *number = copy_number->get_number();
            size_t len_number = *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(number) + sizeof(memory *));
            int sign = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(number) + sizeof(memory *) + sizeof(size_t));

            if (is_int_number(number))
            {
                *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(number) + sizeof(memory *) + sizeof(size_t)) = sign > 0 ? -sign : abs(sign);
            }
            else
            {
                *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(number) + sizeof(memory *) + sizeof(size_t)) = sign == 0 ? 1 : 0;
            }

            bigint *result = subtract(copy_number);
            delete copy_number;

            return result;
        }
    }
    if (number_representation.size() < added.size())
    {
        size_t count_zeros = abs(number_representation.size() - added.size());

        for (int i = 0; i < count_zeros; i++)
        {
            number_representation.insert(number_representation.begin() + 1, 0);
        }
    }

    if (added.size() < number_representation.size())
    {
        size_t count_zeros = abs(number_representation.size() - added.size());

        for (int i = 0; i < count_zeros; i++)
        {
            added.insert(added.begin() + 1, 0);
        }
    }

    for (int i = added.size() - 1; i >= 1; i--)
    {
        result.insert(result.begin() + 1, (static_cast<unsigned long int>(number_representation[i]) + static_cast<unsigned long int>(added[i]) + remainder) % _base);
        remainder = (static_cast<unsigned long int>(number_representation[i]) + static_cast<unsigned long int>(added[i]) + remainder) / _base;
    }

    if (remainder)
    {
        result.insert(result.begin() + 1, remainder);
    }

    memory *aloc = *reinterpret_cast<memory **>(_target_number);
    aloc->deallocate(_target_number);
    _target_number = set_number(result, aloc);

    auto *arr = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t) + sizeof(int));
    return this;
}

bigint *long_number::sum(bigint const *summand) const
{
    bigint *new_obj = new long_number(_target_number);
    return new_obj->add(summand);
}

string_number *long_number::add_string(
    const string_number *target)
{
    int remainder = 0;
    std::string result;
    std::string target_add = target->get_string_number();
    std::string number_str = get_string_number();
    std::string sign;

    if (number_str[0] == '-' && target_add[0] != '-')
    {
        target_add = "-" + target_add;
        string_number *temp = new long_number(target_add);
        string_number *result = subtract_string(temp);

        delete temp;

        return result;
    }

    if (number_str[0] != '-' && target_add[0] == '-')
    {
        target_add = target_add.substr(1, target_add.length() - 1);
        string_number *temp = new long_number(target_add);
        string_number *result = subtract_string(temp);

        delete temp;

        return result;
    }

    if (number_str[0] == '-' && target_add[0] == '-')
    {
        number_str = number_str.substr(1, number_str.length() - 1);
        target_add = target_add.substr(1, target_add.length() - 1);
        sign = "-";
    }

    make_equal_length(number_str, target_add);

    for (int i = target_add.length() - 1; i >= 0; i--)
    {
        result.insert(0, std::to_string(((number_str[i] - '0') + (target_add[i] - '0') + remainder) % 10));
        remainder = ((number_str[i] - '0') + (target_add[i] - '0') + remainder) / 10;
    }

    if (remainder)
    {
        result.insert(0, std::to_string(remainder));
    }

    while (!result.empty() && result[0] == '0')
    {
        result = result.substr(1, result.length() - 1);
    }

    if (result.empty())
    {
        _string_number = "0";
    }
    else
    {
        _string_number = sign + result;
    }

    return this;
}

string_number *long_number::sum_string(
    const string_number *target) const
{
    string_number *temp = new long_number(_string_number);

    return temp->add_string(target);
}

string_number *long_number::subtract_string(
    const string_number *target)
{
    std::string number_str = get_string_number();
    std::string target_sub = target->get_string_number();
    std::string result;
    std::string sign;
    int difference = 0;

    if (number_str[0] == '-' && target_sub[0] != '-')
    {
        target_sub = "-" + target_sub;
        string_number *temp = new long_number(target_sub);
        string_number *result = add_string(temp);

        delete temp;

        return result;
    }

    if (number_str[0] != '-' && target_sub[0] == '-')
    {
        target_sub = target_sub.substr(1, target_sub.length() - 1);
        string_number *temp = new long_number(target_sub);
        string_number *result = add_string(temp);

        delete temp;

        return result;
    }

    if ((number_str[0] == '-' && target_sub[0] == '-') ||
        (number_str[0] != '-' && target_sub[0] != '-'))
    {
        sign = this < target ? "-" : "";
        // std::cout << sign << std::endl;
    }

    if (number_str[0] == '-' && target_sub[0] == '-')
    {
        if (this > target)
        {
            number_str.swap(target_sub);
        }
    }
    else
    {
        if (this < target)
        {
            number_str.swap(target_sub);
        }
    }

    if (number_str[0] == '-')
    {
        number_str = number_str.substr(1, number_str.length() - 1);
    }

    if (target_sub[0] == '-')
    {
        target_sub = target_sub.substr(1, target_sub.length() - 1);
    }

    make_equal_length(number_str, target_sub);

    for (int i = number_str.length() - 1; i >= 0; i--)
    {
        difference = (number_str[i] - '0') - (target_sub[i] - '0');

        if (difference >= 0)
        {
            result.insert(0, std::to_string(difference));
        }
        else
        {
            int j = i - 1;

            while (j >= 0)
            {
                number_str[j] = number_str[j] - '0' ? ((number_str[j] - '0') - 1) % 10 + '0' : '9';

                if (number_str[j] != '9')
                {
                    break;
                }
                else
                {
                    j--;
                }
            }

            result.insert(0, std::to_string(difference + 10));
        }
    }

    while (!result.empty() && result[0] == '0')
    {
        result = result.substr(1, result.length() - 1);
    }

    if (result.empty())
    {
        _string_number = "0";
    }
    else
    {
        _string_number = sign + result;
    }

    return this;
}

string_number *long_number::subtraction_string(
    const string_number *target) const
{
    string_number *temp = new long_number(_string_number);

    return temp->subtract_string(target);
}

bigint *long_number::subtract(bigint const *const subtrahend)
{
    int sign_this = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t));
    int sign_other = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(subtrahend->get_number()) + sizeof(memory *) + sizeof(size_t));
    std::vector<unsigned long int> result;
    std::vector<unsigned long int> number_representation;
    std::vector<unsigned long int> substracted;
    short sign_minuend = 0;
    short sign_subtrahend = 0;

    if (is_int_number(_target_number))
    {
        number_representation.push_back(sign_this < 0);
        number_representation.push_back(abs(sign_this));
        sign_minuend = sign_this < 0 ? -1 : 1;
    }
    else
    {
        number_representation = get_vector(_target_number);
        sign_minuend = number_representation[0] == 1 ? -1 : 1;
    }

    if (is_int_number(subtrahend->get_number()))
    {
        substracted.push_back(sign_other < 0);
        substracted.push_back(abs(sign_other));
        sign_subtrahend = sign_other < 0 ? -1 : 1;
    }
    else
    {
        substracted = get_vector(subtrahend->get_number());
        sign_subtrahend = substracted[0] == 1 ? -1 : 1;
    }

    if ((sign_minuend == -1 && sign_subtrahend == -1) ||
        (sign_minuend == 1 && sign_subtrahend == 1))
    {
        result.push_back(lower_than(subtrahend));
    }
    else
    {
        bigint *copy_number = new long_number(subtrahend->get_number());
        memory *alc = *reinterpret_cast<memory **>(subtrahend->get_number());
        void *number = copy_number->get_number();
        size_t len_number = *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(number) + sizeof(memory *));
        int sign = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(number) + sizeof(memory *) + sizeof(size_t));

        if (is_int_number(number))
        {
            *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(number) + sizeof(memory *) + sizeof(size_t)) = sign > 0 ? -sign : abs(sign);
        }
        else
        {
            *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(number) + sizeof(memory *) + sizeof(size_t)) = sign == 0 ? 1 : 0;
        }

        bigint *result = add(copy_number);
        delete copy_number;
        return result;
    }

    if (number_representation.size() < substracted.size())
    {
        size_t count_zeros = abs(number_representation.size() - substracted.size());

        for (int i = 0; i < count_zeros; i++)
        {
            number_representation.insert(number_representation.begin() + 1, 0);
        }
    }

    if (substracted.size() < number_representation.size())
    {
        size_t count_zeros = abs(number_representation.size() - substracted.size());

        for (int i = 0; i < count_zeros; i++)
        {
            substracted.insert(substracted.begin() + 1, 0);
        }
    }

    if (sign_minuend == -1 && sign_subtrahend == -1)
    {
        if (this < subtrahend)
        {
            substracted.swap(number_representation);
        }
    }
    else
    {
        if (this > subtrahend)
        {
            substracted.swap(number_representation);
        }
    }

    // std::cout << number_representation[1] << " " << substracted[1] << std::endl;

    for (size_t i = substracted.size() - 1; i >= 1; i--)
    {
        if ((substracted[i] < number_representation[i]) && i > 2)
        {
            substracted[i - 1]--;
            int sign = sign_subtrahend * (static_cast<long long>(substracted[i]) + _base) - sign_minuend * (static_cast<long long>(number_representation[i])) < 0 ? -1 : 1;
            result.insert(result.begin() + 1, sign * (sign_subtrahend * (static_cast<long long>(substracted[i]) + _base) - sign_minuend * (static_cast<long long>(number_representation[i]))));
        }
        else
        {
            int sign = sign_subtrahend * (static_cast<long long>(substracted[i])) - sign_minuend * (static_cast<long long>(number_representation[i])) < 0 ? -1 : 1;
            result.insert(result.begin() + 1, sign * (sign_subtrahend * (static_cast<long long>(substracted[i])) - sign_minuend * (static_cast<long long>(number_representation[i]))));
        }
    }

    memory *alc = *reinterpret_cast<memory **>(_target_number);
    alc->deallocate(_target_number);
    _target_number = set_number(result, alc);

    // std::cout << *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t)) << std::endl;

    return this;
}

bigint *long_number::subtraction(bigint const *const subtrahend) const
{
    bigint *new_obj = new long_number(_target_number);

    return new_obj->subtract(subtrahend);
}

string_number *long_number::div(const unsigned long &divend)
{
    std::string srt_number = get_string_number();
    unsigned long temp = 0;
    std::string answer;
    unsigned long index = 0;
    std::string sign;

    if (srt_number[0] == '-')
    {
        srt_number = srt_number.substr(1, srt_number.length() - 1);
        sign = "-";
    }

    while (temp < divend && index < srt_number.length())
    {
        temp = temp * 10 + (srt_number[index++] - '0');
    }

    while (index <= srt_number.length())
    {
        answer += std::to_string(temp / divend);
        temp -= divend * (temp / divend);

        if (index != srt_number.length())
        {
            temp = temp * 10 + (srt_number[index++] - '0');
        }
        else
        {
            index++;
        }
    }

    if (answer.length() > 1)
    {
        while (answer[0] == '0')
        {
            answer = answer.substr(1, answer.length() - 1);
        }
    }

    if (answer != "0")
    {
        _string_number = sign + answer;
    }
    else
    {
        _string_number = answer;
    }

    return this;
}

string_number *long_number::division(const unsigned long &divend)
{
    string_number *temp = new long_number(get_string_number());

    return temp->div(divend);
}

string_number *long_number::mod(const unsigned long &target)
{
    std::string number_str = get_string_number();
    unsigned long temp = 0;
    std::string answer;
    int index = 0;
    std::string sign;

    if (number_str[0] == '-')
    {
        number_str = number_str.substr(1, number_str.length() - 1);
        sign = "-";
    }

    while (temp < target && index < number_str.length())
    {
        temp = temp * 10 + (number_str[index++] - '0');
    }

    while (index <= number_str.length())
    {
        answer += std::to_string(temp / target);
        temp -= target * (temp / target);

        if (index != number_str.length())
        {
            temp = temp * 10 + (number_str[index++] - '0');
        }
        else
        {
            index++;
        }
    }

    if (temp != 0)
    {
        _string_number = sign + std::to_string(temp);
    }
    else
    {
        _string_number = std::to_string(temp);
    }

    return this;
}

string_number *long_number::module(const unsigned long &target)
{
    string_number *temp = new long_number(get_string_number());

    return temp->mod(target);
}

size_t long_number::make_equal_length(
    std::string &number_1,
    std::string &number_2)
{
    unsigned long len = std::max(number_1.length(), number_2.length());

    number_1 = std::string(len - number_1.length(), '0') + number_1;
    number_2 = std::string(len - number_2.length(), '0') + number_2;

    return len;
}

std::string long_number::naiv_mult(
    std::string number,
    std::string multiplier)
{
    int remainder = 0;
    std::string to_addition;
    std::string addition_result;
    int shift = 0;

    if (number == "0" || multiplier == "0")
    {
        return "0";
    }

    for (int i = multiplier.length() - 1; i >= 0; i--)
    {
        for (int j = number.length() - 1; j >= 0; j--)
        {
            to_addition.insert(0, std::to_string(((number[j] - '0') * (multiplier[i] - '0') + remainder) % 10));
            remainder = ((number[j] - '0') * (multiplier[i] - '0') + remainder) / 10;
        }

        if (remainder)
        {
            to_addition.insert(0, std::to_string(remainder));
        }

        to_addition.append(std::string(shift++, '0'));
        addition_result.insert(0, std::string(abs(addition_result.length() - to_addition.length()), '0'));

        string_number *temp_number_1 = new long_number(to_addition);
        string_number *temp_number_2 = new long_number(addition_result);

        *temp_number_1 += temp_number_2;

        addition_result = temp_number_1->get_string_number();

        delete temp_number_1;
        delete temp_number_2;

        to_addition = "";
        remainder = 0;
    }

    while (!addition_result.empty() && addition_result[0] == '0')
    {
        addition_result.erase(addition_result.begin());
    }

    if (addition_result.empty())
    {
        return "0";
    }

    return addition_result;
}

std::string long_number::karatsuba(
    std::string number,
    std::string multiplier)
{
    int max_length = std::max(number.length(), multiplier.length());

    if (max_length < 10)
    {
        return naiv_mult(number, multiplier);
    }

    size_t number_length = make_equal_length(number, multiplier);

    number = std::string(number_length % 2, '0') + number;
    multiplier = std::string(number_length % 2, '0') + multiplier;
    number_length += number_length % 2;

    std::string left_part_number = number.substr(0, number_length / 2);
    std::string right_part_number = number.substr(number_length / 2, number_length / 2);
    std::string left_part_multiplier = multiplier.substr(0, number_length / 2);
    std::string right_part_multiplier = multiplier.substr(number_length / 2, number_length / 2);

    std::string first_addendum = karatsuba(left_part_number, left_part_multiplier);
    std::string second_addendum = karatsuba(right_part_number, right_part_multiplier);

    string_number *first_temp_left_part = new long_number(left_part_number);
    string_number *first_temp_right_part = new long_number(right_part_number);
    string_number *second_temp_left_part = new long_number(left_part_multiplier);
    string_number *second_temp_right_part = new long_number(right_part_multiplier);

    string_number *to_multiply_1 = (*first_temp_left_part + first_temp_right_part);
    string_number *to_multiply_2 = (*second_temp_left_part + second_temp_right_part);

    std::string third_addendum = karatsuba(to_multiply_1->get_string_number(), to_multiply_2->get_string_number());

    delete first_temp_left_part;
    delete first_temp_right_part;
    delete second_temp_left_part;
    delete second_temp_right_part;
    delete to_multiply_1;
    delete to_multiply_2;

    string_number *temp_first_addendum = new long_number(first_addendum);
    string_number *temp_second_addendum = new long_number(second_addendum);
    string_number *temp_third_addendum = new long_number(third_addendum);

    string_number *diff_1 = (*temp_third_addendum - temp_first_addendum);
    string_number *diff_2 = (*diff_1 - temp_second_addendum);

    third_addendum = diff_2->get_string_number();

    delete temp_first_addendum;
    delete temp_second_addendum;
    delete temp_third_addendum;
    delete diff_1;
    delete diff_2;

    first_addendum += std::string(number_length, '0');
    third_addendum += std::string(number_length / 2, '0');

    string_number *temp_1 = new long_number(first_addendum);
    string_number *temp_2 = new long_number(second_addendum);
    string_number *temp_3 = new long_number(third_addendum);

    string_number *sum_1 = (*temp_1 + temp_2);
    string_number *sum_2 = (*sum_1 + temp_3);

    std::string result = sum_2->get_string_number();

    delete temp_1;
    delete temp_2;
    delete temp_3;
    delete sum_1;
    delete sum_2;

    while (!result.empty() && result[0] == '0')
    {
        result.erase(result.begin());
    }

    if (result.empty())
    {
        return "0";
    }

    return result;
}

string_number *long_number::multiply(const string_number *target)
{
    std::string number_str = get_string_number();
    std::string target_mult = target->get_string_number();
    std::string sign;

    if ((number_str[0] == '-' && target_mult[0] != '-') ||
        (number_str[0] != '-' && target_mult[0] == '-'))
    {
        sign = "-";
    }

    if (number_str[0] == '-')
    {
        number_str = number_str.substr(1, number_str.length() - 1);
    }

    if (target_mult[0] == '-')
    {
        target_mult = target_mult.substr(1, target_mult.length() - 1);
    }

    _string_number = sign + karatsuba(number_str, target_mult);

    return this;
}

string_number *long_number::multiplication(const string_number *target) const
{
    string_number *temp = new long_number(get_string_number());

    return temp->multiply(target);
}

string_number *long_number::pow(unsigned long step)
{
    string_number *number_to_pow = new long_number(_string_number);
    string_number *result = new long_number("1");

    while (step)
    {
        if (step % 2 == 0)
        {
            step /= 2;
            *number_to_pow *= number_to_pow;
        }
        else
        {
            step--;
            *result *= number_to_pow;
        }
    }

    delete number_to_pow;

    return result;
}

bool long_number::lower_than_string(string_number const *other) const
{
    std::string number_str = _string_number;
    std::string to_cmp = other->get_string_number();
    bool is_negative = false;

    if (number_str[0] == '-' && to_cmp[0] == '-')
    {
        is_negative = true;
    }

    if (number_str[0] == '-' && to_cmp[0] != '-')
    {
        return true;
    }

    if (number_str[0] != '-' && to_cmp[0] == '-')
    {
        return false;
    }

    if (number_str.length() < to_cmp.length())
    {
        if (is_negative)
        {
            return false;
        }

        return true;
    }

    if (number_str.length() > to_cmp.length())
    {
        if (is_negative)
        {
            return true;
        }

        return false;
    }

    if (number_str[0] == '-')
    {
        number_str.erase(number_str.begin());
    }

    if (to_cmp[0] == '-')
    {
        to_cmp.erase(to_cmp.begin());
    }

    for (int i = 0; i < number_str.length(); i++)
    {
        if (number_str[i] - '0' < to_cmp[i] - '0')
        {
            if (is_negative)
            {
                return false;
            }

            return true;
        }
        else
        {
            if (number_str[i] - '0' > to_cmp[i] - '0')
            {
                if (is_negative)
                {
                    return true;
                }

                return false;
            }
        }
    }

    return false;
}

bool long_number::greater_than_string(string_number const *other) const
{
    return !lower_than_string(other);
}

bool long_number::lower_than(bigint const *other)
{
    int sign_this = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t));
    int sign_other = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(other->get_number()) + sizeof(memory *) + sizeof(size_t));
    std::vector<unsigned long int> number_representation;
    std::vector<unsigned long int> to_compare;

    if (is_int_number(_target_number))
    {
        number_representation.push_back(sign_this < 0);
        number_representation.push_back(abs(sign_this));
    }
    else
    {
        number_representation = get_vector(_target_number);
    }

    if (is_int_number(other->get_number()))
    {
        to_compare.push_back(sign_other < 0);
        to_compare.push_back(abs(sign_other));
    }
    else
    {
        to_compare = get_vector(other->get_number());
    }

    if (number_representation[0] == 1 && to_compare[0] == 0)
    {
        return true;
    }

    if (number_representation[0] == 0 && to_compare[0] == 1)
    {
        return false;
    }

    if (number_representation.size() < to_compare.size())
    {
        return !(number_representation[0] == 1 && to_compare[0] == 1);
    }

    if (number_representation.size() > to_compare.size())
    {
        return number_representation[0] == 1 && to_compare[0] == 1;
    }

    for (int i = 1; i < number_representation.size(); i++)
    {
        if (number_representation[i] < to_compare[i])
        {
            if (number_representation[0] == 1)
            {
                return false;
            }

            return true;
        }
        else
        {
            if (number_representation[i] > to_compare[i])
            {
                if (number_representation[0] == 1)
                {
                    return true;
                }

                return false;
            }
        }
    }

    return false;
}
bool long_number::greater_than(bigint const *other)
{
    return !lower_than(other);
}
bool long_number::lower_than_or_equal_to(bigint const *other)
{
    return lower_than(other) || equals(other);
}
bool long_number::greater_than_or_equal_to(bigint const *other)
{
    return greater_than(other) || equals(other);
}
bool long_number::equals(bigint const *other)
{
    int sign_this = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(_target_number) + sizeof(memory *) + sizeof(size_t));
    int sign_other = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(other->get_number()) + sizeof(memory *) + sizeof(size_t));
    std::vector<unsigned long int> number_representation;
    std::vector<unsigned long int> to_compare;

    if (sign_this != 1 && sign_this != 0)
    {
        number_representation.push_back(sign_this < 0);
        number_representation.push_back(sign_this);
    }
    else
    {
        number_representation = get_vector(_target_number);
    }

    if (sign_other != 1 && sign_other != 0)
    {
        to_compare.push_back(sign_other < 0);
        to_compare.push_back(sign_other);
    }
    else
    {
        to_compare = get_vector(other->get_number());
    }

    if (number_representation[0] != to_compare[0] ||
        number_representation.size() != to_compare.size())
    {
        return false;
    }

    for (int i = 1; i < number_representation.size(); i++)
    {
        if (number_representation[i] != to_compare[i])
        {
            return false;
        }
    }

    return true;
}
bool long_number::not_equals(bigint const *other)
{
    return !equals(other);
}

// long_number::~long_number()
// {
//     memory *alc = *reinterpret_cast<memory **>(_target_number);
//     alc->deallocate(_target_number);
// }

std::ostream &operator<<(
    std::ostream &stream,
    long_number *item)
{
    std::cout << "gg" << std::endl;
    return stream << item->to_ten();
}

std::ostream &operator<<(
    std::ostream &stream,
    bigint *item)
{
    std::cout << "gg1" << std::endl;
    return stream << reinterpret_cast<long_number *>(item);
}