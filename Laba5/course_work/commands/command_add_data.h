#ifndef FUNDAMENTAL_ALGO_COMMAND_ADD_DATA_H
#define FUNDAMENTAL_ALGO_COMMAND_ADD_DATA_H
#include "logger_singleton.h"
#include "command.h"
#include <iostream>
#include "parse.h"
class command_add_data final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;

public:
    bool can_execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_data::can_execute(std::string const &request) called", logger::severity::trace);
        std::vector<std::string> result_state = validation("ADD_DATA", request, 3);

        if (request.starts_with("ADD_DATA(") && request.ends_with(")"))
        {
            std::string *non_const_request = const_cast<std::string *>(&request);

            std::vector<std::string> chopped_string = chop_string_to_vector(
                non_const_request->begin() + std::string("ADD_DATA").length(),
                non_const_request->end(),
                ' ');
            if (chopped_string.size() != 3)
                return false;
            _pool_name = chopped_string[0];
            _scheme_name = chopped_string[1];
            _collection_name = chopped_string[2];
            return true;
        }
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_data::execute(std::string const &request) called", logger::severity::trace);
    }
};
#endif // FUNDAMENTAL_ALGO_COMMAND_ADD_DATA_H
