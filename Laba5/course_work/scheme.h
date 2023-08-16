#ifndef SCHEME_H
#define SCHEME_H

#include <iostream>

#include "collection.h"

class scheme final
{

    friend class database_singlton;

private:
    memory *_allocator;
    associative_container<std::string, collection> *_scheme;

public:
    scheme() : _scheme(new red_black_tree<std::string, collection, stdstring_comparer>())
    {
    }

    ~scheme()
    {
        delete _scheme;
    }

public:
    void add(std::string const &collection_name, collection &&target)
    {
        return _scheme->insert(collection_name, std::move(target));
    }

    void remove(std::string const &key)
    {
    }

    collection &find(std::string const &collection_name) const
    {
        return _scheme->find(collection_name);
    }

    bool find_to_string(std::string const &collection_name) const
    {
        return _scheme->find_to_string(collection_name);
    }

public:
    scheme(scheme const &other) : _scheme(new red_black_tree<std::string, collection, stdstring_comparer>(*reinterpret_cast<red_black_tree<std::string, collection, stdstring_comparer> *>(other._scheme)))
    {
    }

    scheme &operator=(scheme const &other)
    {
        if (this == &other)
        {
            return *this;
        }

        delete this->_scheme;

        this->_scheme = new red_black_tree<std::string, collection, stdstring_comparer>(*reinterpret_cast<red_black_tree<std::string, collection, stdstring_comparer> *>(other._scheme));

        return *this;
    }

    scheme(scheme &&other) noexcept
    {
        this->_scheme = other._scheme;
        other._scheme = nullptr;
    }

    scheme &operator=(scheme &&other) noexcept
    {

        if (this == &other)
        {
            return *this;
        }

        delete this->_scheme;

        this->_scheme = other._scheme;

        other._scheme = nullptr;

        return *this;
    }
};

#endif // SCHEME_H