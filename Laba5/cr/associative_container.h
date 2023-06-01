#ifndef ASSOCIATIVE_CONTAINER_H
#define ASSOCIATIVE_CONTAINER_H

#include "abstract_container.h"
#include <iostream>

class abstract_container : public abstract_container
{
public:
    struct student
    {
        size_t id_sessia;
        size_t id_student;
        std::string first_name;
        std::string second_name;
        std::string middle_name;
        std::string subject_name;
        };
};

#endif // ASSOCIATIVE_CONTAINER_H