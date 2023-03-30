#include "logger_builder_concrete.h"
#include "logger_concrete.h"
#include "../nlohmann-json/json-3.11.2/single_include/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

logger_builder *logger_builder_concrete::add_stream(
    std::string const &path,
    logger::severity severity)
{
    _construction_info[path] = severity;

    return this;
}

logger_builder *logger_builder_concrete::from_json(std::string const &path)
{
    std::ifstream f(path);
    json data = json::parse(f);
    std::string curr;
    logger::severity severity;
    for (auto &i : data.items())
    {
        if (i.value() == "error")
        {
            severity = logger::severity::error;
        }
        if (i.value() == "warning")
        {
            severity = logger::severity::warning;
        }
        if (i.value() == "critical")
        {
            severity = logger::severity::critical;
        }
        if (i.value() == "trace")
        {
            severity = logger::severity::trace;
        }
        if (i.value() == "debug")
        {
            severity = logger::severity::debug;
        }
        if (i.value() == "information")
        {
            severity = logger::severity::information;
        }
        _construction_info[i.key()] = severity;
    }

    return this;
}

logger *logger_builder_concrete::construct() const
{
    return new logger_concrete(_construction_info);
}
