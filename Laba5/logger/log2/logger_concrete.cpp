#include "logger_concrete.h"
#include <iostream>
#include <fstream>
#include <ctime>

std::map<std::string, std::pair<std::ofstream *, size_t>> logger_concrete::_streams =
    std::map<std::string, std::pair<std::ofstream *, size_t>>();

logger_concrete::logger_concrete(
    std::map<std::string, logger::severity> const &targets)
{
    for (auto &target : targets)
    {
        auto global_stream = _streams.find(target.first);
        std::ofstream *stream = nullptr;

        if (global_stream == _streams.end())
        {
            if (target.first != "console")
            {
                stream = new std::ofstream;
                stream->open(target.first);
            }

            _streams.insert(std::make_pair(target.first, std::make_pair(stream, 1)));
        }
        else
        {
            stream = global_stream->second.first;
            global_stream->second.second++;
        }

        _logger_streams.insert(std::make_pair(target.first, std::make_pair(stream, target.second)));
    }
}

logger_concrete::~logger_concrete()
{
    for (auto &logger_stream : _logger_streams)
    {
        auto global_stream = _streams.find(logger_stream.first);
        if (--(global_stream->second.second) == 0)
        {
            if (global_stream->second.first != nullptr)
            {
                global_stream->second.first->flush();
                global_stream->second.first->close();
                delete global_stream->second.first;
            }

            _streams.erase(global_stream);
        }
    }
}

std::string currentDateTime()
{
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    char buffer[128];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y %X", now);
    return buffer;
}

logger const *
logger_concrete::log(
    const std::string &to_log,
    logger::severity severity) const
{

    for (auto &logger_stream : _logger_streams)
    {
        time_t now = time(0);
        tm *datetime = localtime(&now);
        if (logger_stream.second.second > severity)
        {
            continue;
        }

        std::string curr_sev;
        switch (severity)
        {
        case logger::severity::critical:
            curr_sev = "critical";
            break;
        case logger::severity::error:
            curr_sev = "error";
            break;
        case logger::severity::warning:
            curr_sev = "warning";
            break;
        case logger::severity::trace:
            curr_sev = "trace";
            break;
        case logger::severity::information:
            curr_sev = "information";
            break;
        case logger::severity::debug:
            curr_sev = "debug";
            break;
        }

        if (logger_stream.second.first == nullptr)
        {

            std::cout << "[" << curr_sev << "]"
                      << "[" << currentDateTime() << "]"
                      << " " << to_log << std::endl;
        }
        else
        {
            (*logger_stream.second.first) << "[" << curr_sev << "]"
                                          << "[" << currentDateTime() << "]"
                                          << " " << to_log << std::endl;
        }
    }

    return this;
}
