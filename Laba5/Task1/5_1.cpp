#include <iostream>
#include <string>

using namespace std;

class logger
{
public:
    enum class severity
    {
        trace,
        debug,
        information,
        warning,
        error,
        critical,
    };

    virtual logger *logger::log(const std::string &target, severity level) const = 0;
};

int main()
{

    return 0;
}