
#ifndef FUNDAMENTAL_ALGO_COMMAND_REMOVE_DATA_H
#define FUNDAMENTAL_ALGO_COMMAND_REMOVE_DATA_H
#include <iostream>
#include "command.h"
#include "logger_singleton.h"
class command_remove_data final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;

public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_data::can_execute(std::string const &request) called", logger::severity::trace);
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->trace("command_remove_data::execute(std::string const &request) called", logger::severity::trace);
    }
};
#endif // FUNDAMENTAL_ALGO_COMMAND_REMOVE_DATA_H
