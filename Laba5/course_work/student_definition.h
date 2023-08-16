#ifndef STUDENT_DEFINITION
#define STUDENT_DEFINITION
#include <iostream>
//     session_id ///student_id  ///subject_name for tuple

enum class reporting_format
{
    course_project,
    credit,
    exam
};
struct key
{
    unsigned int id_session;
    unsigned int id_student;
    reporting_format format;
    std::string subject;
};

struct values
{
    std::string student_name;
    std::string student_surname;
    std::string student_patronymic;
    std::string event_date;
    std::string event_time;
    unsigned int mark;
    // std::string &tutor_name;
    // std::string &tutor_surname;
    // std::string &tutor_patronymic;
};
#endif // STUDENT_DEFINITION