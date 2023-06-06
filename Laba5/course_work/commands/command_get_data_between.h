

#ifndef FUNDAMENTAL_ALGO_COMMAND_GET_DATA_BETWEEN_H
#define FUNDAMENTAL_ALGO_COMMAND_GET_DATA_BETWEEN_H
#include "../logger/logger.h"
#include "logger_singleton.h"
#include <iostream>
#include "command.h"
class command_get_data_between final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;

public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_get_data_between::can_execute(std::string const &request) called", logger::severity::trace);
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_get_data_between::execute(std::string const &request) called", logger::severity::trace);
    }
};
#endif // FUNDAMENTAL_ALGO_COMMAND_GET_DATA_BETWEEN_H
