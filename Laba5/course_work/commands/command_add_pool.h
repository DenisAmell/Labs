#ifndef FUNDAMENTAL_ALGO_COMMAND_ADD_POOL_H
#define FUNDAMENTAL_ALGO_COMMAND_ADD_POOL_H

#include "command.h"
#include "../database_singleton.h"

#include "../parse.h"
#include <string>
#include <string_view>

class database_singleton;

class command_add_pool final : public command<std::string>
{
private:
    std::string _pool_name;
    allocator_type _pool_allocator_type;
    unsigned int _pool_allocator_size;
    memory::allocate_mode _pool_allocator_allocation_mode;

public:
    bool can_execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_pool::can_execute(std::string const &request) called", logger::severity::trace);
        // // if (!prefix_validation("ADD_POOL", request))
        // //     return false;
        // std::vector<std::string> result_parsed_strings = validation("ADD_POOL", request);

        if (request.starts_with("ADD_POOL"))
        {

            auto result_parsed_strings = validation(request, ' ');
            if (result_parsed_strings.size() == 5)
            {
                _pool_name = std::move(result_parsed_strings[1]);

                std::cout << _pool_name << std::endl;
                try
                {
                    _pool_allocator_type = string_to_allocator_type(result_parsed_strings[2]);
                }
                catch (doesnt_exist(&))
                {
                    return false;
                }

                try
                {
                    _pool_allocator_allocation_mode = allocation_mode_convert(result_parsed_strings[3]);
                }
                catch (doesnt_exist(&))
                {
                    return false;
                }

                try
                {
                    _pool_allocator_size = is_unsigned_with_convert(result_parsed_strings[4]);
                }
                catch (doesnt_exist(&))
                {
                    return false;
                }

                return true;
            }
        }
        return false;
    }

    void execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_pool::execute(std::string const &request) called", logger::severity::trace);
        database_singleton::get_instance()->add_pool(_pool_name, _pool_allocator_type, _pool_allocator_size, _pool_allocator_allocation_mode);
    }
};
#endif // FUNDAMENTAL_ALGO_COMMAND_ADD_POOL_H