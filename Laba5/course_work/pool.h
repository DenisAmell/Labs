#ifndef POOL_H
#define POOL_H

#include "scheme.h"

class pool
{

private:
    memory *_allocator;
    associative_container<std::string, scheme> *_pool;

public:
    pool(memory *allocator = nullptr) : _pool(new red_black_tree<std::string, scheme, stdstring_comparer>()), _allocator(allocator)
    {
    }

    ~pool()
    {
        delete _pool;
    }

public:
    void add(std::string const &key, scheme &&target)
    {
        _pool->insert(key, std::move(target));
    }

    void remove(std::string const &key)
    {
        _pool->remove(key);
    }

    scheme &find(std::string const &scheme_name) const
    {
        return _pool->find(scheme_name);
    }

    bool find_to_string(std::string const &scheme_name) const
    {
        return _pool->find_to_string(scheme_name);
    }

    memory *get_allocator() const
    {
        return _allocator;
    }

public:
    pool(const pool &other)
        : _pool(new red_black_tree<std::string, scheme, stdstring_comparer>(*reinterpret_cast<red_black_tree<std::string, scheme, stdstring_comparer> *>(other._pool))), _allocator(other._allocator)
    {
    }

    pool(pool &&other) noexcept
    {
        this->_pool = other._pool;
        this->_allocator = other._allocator;

        other._pool = nullptr;
        other._allocator = nullptr;
    }

    pool &operator=(const pool &other)
    {
        if (this == &other)
        {
            return *this;
        }

        delete this->_pool;

        if (this->_allocator != other._allocator)
        {
            delete this->_allocator;
            this->_allocator = other._allocator;
        }

        this->_pool = new red_black_tree<std::string, scheme, stdstring_comparer>(*reinterpret_cast<red_black_tree<std::string, scheme, stdstring_comparer> *>(other._pool));

        return *this;
    }

    pool &operator=(pool &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        delete this->_pool;
        delete this->_allocator;

        this->_pool = other._pool;
        this->_allocator = other._allocator;

        other._pool = nullptr;
        other._allocator = nullptr;

        return *this;
    }
};

#endif // POOL_H