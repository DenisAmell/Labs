#ifndef FUNDAMENTAL_ALGO_COMMAND_ADD_POOL_H
#define FUNDAMENTAL_ALGO_COMMAND_ADD_POOL_H
#include "command.h"
#include "logger_singleton.h"
#include "../logger/logger.h"
#include "../memory/memory.h"
#include "database_singleton.h"
#include "parse.h"

class command_add_pool final : public command<std::string>
{
private:
    std::string _pool_name;
    allocator_type _pool_allocator_type;
    unsigned int _pool_allocator_size;
    memory::allocate_mode _pool_allocator_allocation_mode;

public:
    bool can_execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_pool::can_execute(std::string const &request) called", logger::severity::trace);
        if (request.starts_with("ADD_POOL(") && request.ends_with(")"))
        {
            std::string *non_const_request = const_cast<std::string *>(&request);

            std::vector<std::string> chopped_string = chop_string_to_vector(
                non_const_request->begin() + std::string("ADD_POOL").length(),
                non_const_request->end(),
                ' ');
            if (chopped_string.size() != 3)
                return false;
            _pool_name = chopped_string[0];
            _pool_allocator_allocation_mode
                _collection_name = chopped_string[2];
            return true;
        }
        return false;
    }

    void execute(std::string const &request) const noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_pool::execute(std::string const &request) called", logger::severity::trace);
        database_singleton::get_instance()->add_pool(_pool_name, _pool_allocator_type, _pool_allocator_size, _pool_allocator_allocation_mode);
    }
};
#endif // FUNDAMENTAL_ALGO_COMMAND_ADD_POOL_H
