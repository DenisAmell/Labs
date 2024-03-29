
#ifndef FUNDAMENTAL_ALGO_REQUEST_HANDLER_WITH_COMMAND_H
#define FUNDAMENTAL_ALGO_REQUEST_HANDLER_WITH_COMMAND_H

#include "abstract_handler.h"
#include "commands/command.h"

template <typename trequest>
class request_handler_with_command : public abstract_handler<trequest>
{
private:
    command<trequest> *_target_action;

public:
    request_handler_with_command(command<trequest> *target_action) : _target_action(target_action)
    {
    }

    ~request_handler_with_command()
    {
        delete _target_action;
    }

    request_handler_with_command(request_handler_with_command<trequest> const &) = delete;
    request_handler_with_command(request_handler_with_command<trequest> &&) = delete;
    request_handler_with_command<trequest> &operator=(request_handler_with_command<trequest> const &) = delete;
    request_handler_with_command<trequest> &operator=(request_handler_with_command<trequest> &&) = delete;

public:
    bool handle(trequest const &request) const noexcept
    {

        if (!_target_action->can_execute(request))
        {
            if (abstract_handler<trequest>::_next_handler == nullptr)
            {
                return false;
            }

            return abstract_handler<trequest>::_next_handler->handle(request);
        }

        _target_action->execute(request);
        return true;
    }
};

#endif // FUNDAMENTAL_ALGO_REQUEST_HANDLER_WITH_COMMAND_H