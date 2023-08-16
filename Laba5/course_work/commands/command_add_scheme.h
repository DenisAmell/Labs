#ifndef FUNDAMENTAL_ALGO_COMMAND_ADD_SCHEME_H
#define FUNDAMENTAL_ALGO_COMMAND_ADD_SCHEME_H
#include <iostream>

#include "command.h"
#include "../logger_singleton.h"
#include "../database_singleton.h"

class command_add_scheme final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;

public:
    bool can_execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_scheme::can_execute(std::string const &request) called", logger::severity::trace);

        if (request.starts_with("ADD_SCHEME"))
        {

            std::vector<std::string> result_parsed_strings = validation(request, ' ');
            std::cout << result_parsed_strings.size() << std::endl;
            if (result_parsed_strings.size() == 3)
            {
                _pool_name = std::move(result_parsed_strings[1]);
                _scheme_name = std::move(result_parsed_strings[2]);

                return true;
            }
        }
        return false;
    }

    void execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_scheme::execute(std::string const &request) called", logger::severity::trace);
        database_singleton::get_instance()->add_scheme(_pool_name, _scheme_name);
    }
};

#endif // FUNDAMENTAL_ALGO_COMMAND_ADD_SCHEME_H