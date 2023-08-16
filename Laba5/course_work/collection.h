#ifndef FUNDAMENTAL_ALGO_COLLECTION_H
#define FUNDAMENTAL_ALGO_COLLECTION_H

#include "student_definition.h"
#include <tuple>
#include <iostream>

class collection final : protected memory_holder
{
private:
    memory *_allocator;
    associative_container<key *, values *> *_data;

public:
    collection(memory *allocator = nullptr) : _data(new red_black_tree<key *, values *, key_comparer>(allocator, nullptr)), _allocator(allocator)
    {
    }

    ~collection()
    {
        delete _data;
    }

public:
    void add(
        unsigned int id_session,
        unsigned int id_student,
        reporting_format format,
        const std::string &subject,
        const std::string &surname,
        const std::string &name,
        const std::string &patronymic,
        const std::string &date,
        const std::string &time,
        unsigned int mark)
    {
        key *data_key = reinterpret_cast<key *>(allocate_with_guard(sizeof(key)));
        new (data_key) key;

        data_key->id_session = id_session;
        data_key->id_student = id_student;
        data_key->format = format;
        data_key->subject = subject;

        values *data_value = reinterpret_cast<values *>(allocate_with_guard(sizeof(values)));
        new (data_value) values;

        data_value->student_surname = surname;
        data_value->student_name = name;
        data_value->student_patronymic = patronymic;
        data_value->event_date = date;
        data_value->event_time = time;
        data_value->mark = mark;

        _data->insert(data_key, std::move(data_value));
    }

    bool find_in(key *const &data_key) const
    {
        return _data->find_in(data_key);
    }

    values remove(key *data_key)
    {
        auto returned_node = _data->remove_node(data_key);

        values result = *(std::get<1>(returned_node));

        std::get<0>(returned_node)->~key();
        deallocate_with_guard(std::get<0>(returned_node));

        std::get<1>(returned_node)->~values();
        deallocate_with_guard(std::get<1>(returned_node));

        return result;
    }

    values *get(key *const &data_key)
    {
        return _data->get_value(data_key);
    }

    std::vector<values *> get_between_keys(key *const &min_key, key *const &max_key)
    {
        return _data->find_in_range(min_key, max_key);
    }

    void update(
        key *const &data_key,
        const std::string &surname,
        const std::string &name,
        const std::string &patronymic,
        const std::string &date,
        const std::string &time,
        unsigned int mark)
    {
        values *data_value = reinterpret_cast<values *>(allocate_with_guard(sizeof(values)));
        new (data_value) values;

        data_value->student_surname = surname;
        data_value->student_name = name;
        data_value->student_patronymic = patronymic;
        data_value->event_date = date;
        data_value->event_time = time;
        data_value->mark = mark;

        _data->update(data_key, std::move(data_value));
    }

public:
    collection(collection const &other) : _data(new red_black_tree<key *, values *, key_comparer>(*reinterpret_cast<red_black_tree<key *, values *, key_comparer> *>(other._data))), _allocator(other._allocator)
    {
    }

    collection &operator=(collection const &other)
    {
        if (this == &other)
        {
            return *this;
        }

        delete this->_data;

        if (this->_allocator != other._allocator)
        {
            delete this->_allocator;
            this->_allocator = other._allocator;
        }

        this->_data = new red_black_tree<key *, values *, key_comparer>(*reinterpret_cast<red_black_tree<key *, values *, key_comparer> *>(other._data));

        return *this;
    }

    collection(collection &&other) noexcept
    {
        this->_data = other._data;
        this->_allocator = other._allocator;

        other._data = nullptr;
        other._allocator = nullptr;
    }

    collection &operator=(collection &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        delete this->_data;
        delete this->_allocator;

        this->_data = other._data;
        this->_allocator = other._allocator;

        other._data = nullptr;
        other._allocator = nullptr;

        return *this;
    }

private:
    memory *get_memory() const override
    {
        return _allocator;
    }
};
#endif // FUNDAMENTAL_ALGO_COLLECTION_H