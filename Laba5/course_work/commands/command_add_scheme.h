#ifndef FUNDAMENTAL_ALGO_COMMAND_ADD_SCHEME_H
#define FUNDAMENTAL_ALGO_COMMAND_ADD_SCHEME_H
#include <iostream>
#include "command.h"
#include "logger_singleton.h"
#include "parse.h"
class command_add_scheme final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;

public:
    bool can_execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_scheme::can_execute(std::string const &request) called", logger::severity::trace);
        logger_singleton::get_instance()->get_logger()->log("command_add_collection::can_execute(std::string const &request) called", logger::severity::trace);
        if (request.starts_with("ADD_SCHEME(") && request.ends_with(")"))
        {
            std::string *non_const_request = const_cast<std::string *>(&request);

            std::vector<std::string> chopped_string = chop_string_to_vector(
                non_const_request->begin() + std::string("ADD_SCHEME").length(),
                non_const_request->end(),
                ' ');
            if (chopped_string.size() != 2)
                return false;
            _pool_name = chopped_string[0];
            _scheme_name = chopped_string[1];
            return true;
        }
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_scheme::execute(std::string const &request) called", logger::severity::trace);
    }
};
#endif // FUNDAMENTAL_ALGO_COMMAND_ADD_SCHEME_H
