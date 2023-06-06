#ifndef FUNDAMENTAL_ALGO_DATABASE_SINGLETON_H
#define FUNDAMENTAL_ALGO_DATABASE_SINGLETON_H

#include "memory/memory.h"
#include "logger_singleton.h"
#include "logger/logger.h"
#include "comparers.h"
#include "allocator_type.h"
#include "tree_type.h"
#include "tree/binary_search_tree.h"
#include "tree/red_black_tree.h"
#include "allocators/list_memory.h"
#include "allocators/boundary_tags_allocator.h"
#include "allocators/memory_buddy_system.h"
#include "student_definition.h"

template <typename tkey, typename tvalue, typename tkey_comparer>
class database_singleton
{
private:
    class stdpair_int_int_comparer final
    {
    private:
        int_comparer _int_comparer;

    public:
        int operator()(std::pair<int, int> const &left, std::pair<int, int> const &right)
        {
            auto user_id_comparison_result = _int_comparer(left.first, right.first);
            if (user_id_comparison_result != 0)
            {
                return user_id_comparison_result;
            }

            return _int_comparer(left.second, right.second);
        }
    };

private:
    static database_singleton *_instance;

public:
    static database_singleton *get_instance()
    {
        if (_instance == nullptr)
        {
            _instance = new database_singleton();
        }

        return _instance;
    }

private:
    // associative_container<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, student_definition *> *> *> *, memory *>> *_database_entrypoint;
    associative_container<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, student_definition *> *> *> *, memory *>> *_database_entrypoint;

public:
    database_singleton()
    {
        // _database_entrypoint = new red_black_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, student_definition *> *> *> *, memory *>, stdstring_comparer>();
        _database_entrypoint = new red_black_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, student_definition *> *> *> *, memory *>, stdstring_comparer>();
    }

public:
    ~database_singleton()
    {
        delete _database_entrypoint;
    }
    database_singleton(database_singleton const &) = delete;
    database_singleton(database_singleton &&) = delete;
    database_singleton &operator=(database_singleton const &) = delete;
    database_singleton &operator=(database_singleton &&) = delete;

public:
    void add_pool(
        std::string const &pool_name,
        allocator_type pool_allocator_type,
        unsigned int pool_allocator_size,
        memory::allocate_mode pool_allocator_allocation_mode,
        tree_type tree_type)
    {
        memory *allocator = nullptr;
        switch (pool_allocator_type)
        {
        case allocator_type::global_heap:
            // allocator = new memory_from_global_heap(logger_singleton::get_instance()->get_logger());
            break;
        case allocator_type::list_memory:
            allocator = new list_memory(logger_singleton::get_instance()->get_logger(), nullptr,
                                        pool_allocator_size,
                                        pool_allocator_allocation_mode);
            break;
        case allocator_type::border_descriptors:
            allocator = new border_descriptors_memory(logger_singleton::get_instance()->get_logger(), nullptr,
                                                      pool_allocator_size,
                                                      pool_allocator_allocation_mode);
            break;
        case allocator_type::buddy_allocator:
            allocator = new memory_buddy_system(logger_singleton::get_instance()->get_logger(), nullptr,
                                                pool_allocator_size,
                                                pool_allocator_allocation_mode);
            break;
        }
        //_database_entrypoint->insert(pool_name, std::move(std::make_pair(new red_black_tree<std::string, associative_container<std::string, associative_container<std::pair<int, int>, student_definition *> *> *, stdstring_comparer>(), allocator)));

        _database_entrypoint->insert(pool_name, std::move(std::make_pair(new red_black_tree<std::string, associative_container<std::string, associative_container<std::pair<int, int>, student_definition *> *> *, stdstring_comparer>(), allocator)));

        switch (tree_type)
        {
        case tree_type::b_plus_tree:

            break;
        case tree_type::avl_tree:
            break;
        case tree_type::red_black_tree:
            break;
        case tree_type::b_tree:
            break;
        }
        // try
        // {

        // }
        // catch (search_tree<std::string, std::pair<associative_container<std::string, associative_container<std::string, associative_container<std::pair<int, int>, delivery_info *> *> *> *, memory *>, stdstring_comparer>::insertion_exception const &ex)
        // {
        //     // TODO: ?!
        // }
    }
};
#endif // FUNDAMENTAL_ALGO_DATABASE_SINGLETON_H
