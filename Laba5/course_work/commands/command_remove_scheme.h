#ifndef FUNDAMENTAL_ALGO_COMMAND_REMOVE_SCHEME_H
#define FUNDAMENTAL_ALGO_COMMAND_REMOVE_SCHEME_H

#include <iostream>
#include "command.h"
#include <vector>
class command_remove_scheme final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;

public:
    bool can_execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_remove_scheme::can_execute(std::string const &request) called", logger::severity::trace);
        if (!prefix_validation("REMOVE_SCHEME", request))
            return false;
        std::vector<std::string> result_parsed_strings = validation("REMOVE_SCHEME", request);
        if (result_parsed_strings.size() == 3)
        {
            _pool_name = result_parsed_strings[0];
            _scheme_name = result_parsed_strings[1];
            return true;
        }
        return false;
    }

    void execute(std::string const &request) noexcept final
    {
        database_singleton::get_instance()->remove_scheme(_pool_name, _scheme_name);
        logger_singleton::get_instance()
            ->get_logger()
            ->log("command_remove_scheme::execute(std::string const &request) called", logger::severity::trace);
    }
};

#endif // FUNDAMENTAL_ALGO_COMMAND_REMOVE_SCHEME_H