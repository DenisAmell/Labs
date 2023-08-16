#ifndef FUNDAMENTAL_ALGO_COMMAND_ADD_COLLECTION_H
#define FUNDAMENTAL_ALGO_COMMAND_ADD_COLLECTION_H

#include "command.h"
#include "../database_singleton.h"
#include "../logger_singleton.h"

class command_add_collection final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;

public:
    bool can_execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_collection::can_execute(std::string const &request) called", logger::severity::trace);
        // if (!prefix_validation("ADD_COLLECTION", request))
        //     return false;

        if (request.starts_with("ADD_COLLECTION"))
        {

            std::vector<std::string> result_parsed_strings = validation(request, ' ');
            if (result_parsed_strings.size() == 4)
            {
                _pool_name = std::move(result_parsed_strings[1]);
                _scheme_name = std::move(result_parsed_strings[2]);
                _collection_name = std::move(result_parsed_strings[3]);
                std::cout << "gg" << std::endl;
                return true;
            }
        }
        return false;
    }

    void execute(std::string const &request) noexcept final
    {
        database_singleton::get_instance()->add_collection(_pool_name, _scheme_name, _collection_name);
        logger_singleton::get_instance()->get_logger()->log("command_add_collection::execute(std::string const &request) called", logger::severity::trace);
    }
};
#endif // FUNDAMENTAL_ALGO_COMMAND_ADD_COLLECTION_H