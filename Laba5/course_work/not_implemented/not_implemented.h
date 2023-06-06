#ifndef NOT_IMPLEMENTED
#define NOT_IMPLEMENTED

#include <iostream>

class not_implemented final : public std::exception
{
private:
    std::string _what;

public:
    not_implemented(std::string const &method_name)
        : _what("Method " + method_name + " not implemented")
    {
    }

    const char *what() const noexcept override
    {
        return _what.c_str();
    }
};

#endif //  NOT_IMPLEMENTED