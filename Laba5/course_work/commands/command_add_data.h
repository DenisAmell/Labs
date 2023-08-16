#ifndef FUNDAMENTAL_ALGO_COMMAND_ADD_DATA_H
#define FUNDAMENTAL_ALGO_COMMAND_ADD_DATA_H

#include "command.h"
#include "student_definition.h"

class command_add_data final : public command<std::string>
{
private:
    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    unsigned int _id_session;
    unsigned int _id_student;
    reporting_format _format;
    std::string _subject;
    std::string _student_name;
    std::string _student_surname;
    std::string _student_patronymic;
    std::string _event_date;
    std::string _event_time;
    unsigned int _mark;

public:
    bool can_execute(std::string const &request) noexcept final
    {
        logger_singleton::get_instance()->get_logger()->log("command_add_data::can_execute(std::string const &request) called", logger::severity::trace);
        if (!prefix_validation("ADD_DATA", request))
            return false;
        std::vector<std::string> result_parsed_strings = validation("ADD_DATA", request);
        if (result_parsed_strings.size() == 13)
        {
            _pool_name = std::move(result_parsed_strings[0]);
            _scheme_name = std::move(result_parsed_strings[1]);
            _collection_name = std::move(result_parsed_strings[2]);

            _id_session = is_unsigned_with_convert(result_parsed_strings[3]); // session id
            _id_student = is_unsigned_with_convert(result_parsed_strings[4]); // student id
            _format = reporting_format_convert(result_parsed_strings[5]);
            _subject = std::move(result_parsed_strings[6]);

            _student_name = std::move(result_parsed_strings[7]);
            _student_surname = std::move(result_parsed_strings[8]);
            _student_patronymic = std::move(result_parsed_strings[9]);

            // _current_student._values.tutor_name = std::move(result_parsed_strings[6]);
            // _current_student._values.tutor_surname = std::move(result_parsed_strings[7]);
            // _current_student._values.tutor_patronymic = std::move(result_parsed_strings[8]);
            _event_date = result_parsed_strings[10]; // TODO: validate
            _event_time = result_parsed_strings[11]; // TODO: validate
            _mark = is_valid_grade(_format, result_parsed_strings[12]);

            return true;
        }
        return false;
    }

    void execute(std::string const &request) noexcept final
    {
        database_singleton::get_instance()->add_data(_pool_name, _scheme_name, _collection_name, _id_session, _id_student, _format, _subject, _student_surname, _student_name, _student_patronymic, _event_date, _event_time, _mark);
        logger_singleton::get_instance()->get_logger()->log("command_add_data::execute(std::string const &request) called", logger::severity::trace);
    }
};
#endif // FUNDAMENTAL_ALGO_COMMAND_ADD_DATA_H