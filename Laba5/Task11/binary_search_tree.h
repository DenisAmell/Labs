#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stack>
#include "associative_container.h"
#include "../logger/log2/logger.h"
#include "../logger/log2/logger_holder.h"
#include "../logger/log2/logger_concrete.h"
#include "../memory/memory.h"
#include "../memory/memory_holder.h"
#include <vector>

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
class binary_search_tree : public associative_container<tkey, tvalue>,
                           private logger_holder,
                           protected memory_holder
{

protected:
    struct node
    {
        tkey key;

        tvalue value;

        node *left_subtree_address;

        node *right_subtree_address;
    };

public:
    class prefix_iterator final
    {

    private:
        node *_tree_root;
        std::stack<node *> _way;

    public:
        explicit prefix_iterator(
            node *tree_root);

    public:
        bool operator==(
            prefix_iterator const &other) const;

        prefix_iterator &operator++();

        prefix_iterator operator++(
            int not_used);

        std::tuple<unsigned int, tkey const &, tvalue const &> operator*() const;
    };

    class infix_iterator final
    {

    private:
        binary_search_tree<tkey, tvalue, tkey_comparer>::node *_current_node;
        std::stack<node *> _way;

    public:
        explicit infix_iterator(
            binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree);

    public:
        bool operator==(
            infix_iterator const &other) const;

        infix_iterator &operator++();

        bool operator!=(infix_iterator const &other) const;

        infix_iterator operator++(
            int not_used);

        std::tuple<unsigned int, tkey const &, tvalue const &> operator*() const;
    };

    class postfix_iterator final
    {

    private:
        node *_tree_root;
        std::stack<node *> _way;

    public:
        explicit postfix_iterator(
            node *tree_root);

    public:
        bool operator==(
            postfix_iterator const &other) const;

        postfix_iterator &operator++();

        postfix_iterator operator++(
            int not_used);

        std::tuple<unsigned int, tkey const &, tvalue const &> operator*() const;
    };

protected:
    class insertion_template_method : protected memory_holder, private logger_holder
    {

    public:
        explicit insertion_template_method(binary_search_tree<tkey, tvalue, tkey_comparer> *tree)
            : _tree(tree)
        {
        }

        virtual ~insertion_template_method() = default;

    public:
        void insert(tkey const &key, tvalue &&value, node *&tree_root_address);

    private:
        void insert_inner(
            tkey const &key,
            tvalue &&value,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    protected:
        virtual void before_insert_inner(
            tkey const &key,
            tvalue &&value,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_insert_inner(
            tkey const &key,
            tvalue const &value,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

        virtual size_t get_node_size() const;

        virtual void call_constructor_node(node *mem, tkey const &key, tvalue &&value) const;

    private:
        memory *get_memory() const override;

        logger *get_logger() const override;

    private:
        binary_search_tree<tkey, tvalue, tkey_comparer> *_tree;
    };

    // friend class insertion_template_method;

    class reading_template_method
    {

    public:
        virtual ~reading_template_method() = default;

    public:
        tvalue const &read(
            tkey const &key,
            node *&tree_root_address);

    private:
        tvalue const &read_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    protected:
        virtual void before_read_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_read_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);
    };

    class removing_template_method
    {

    public:
        virtual ~removing_template_method() = default;

    public:
        tvalue &&remove(
            tkey const &key,
            node *&tree_root_address);

    private:
        tvalue &&remove_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    protected:
        virtual void before_remove_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_remove_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);
    };

private:
    tkey_comparer _comparator;
    node *_root;
    memory *_allocator;
    logger *_logger;
    insertion_template_method *_insertion;
    reading_template_method *_reading;
    removing_template_method *_removing;

protected:
    explicit binary_search_tree(
        insertion_template_method *insertion,
        reading_template_method *reading,
        removing_template_method *removing,
        memory *allocator = nullptr,
        logger *logger = nullptr);

public:
    explicit binary_search_tree(
        memory *allocator = nullptr,
        logger *logger = nullptr);

    binary_search_tree(
        binary_search_tree const &other);

    binary_search_tree(
        binary_search_tree &&other) noexcept;

    binary_search_tree &operator=(
        binary_search_tree const &other);

    binary_search_tree &operator=(
        binary_search_tree &&other) noexcept;

    ~binary_search_tree();

public:
    void insert(
        tkey const &key,
        tvalue &&value) final;

    tvalue const &get(
        tkey const &key) final;

    tvalue &&remove(
        tkey const &key) final;

private:
    logger *get_logger() const override;
    memory *get_memory() const override;

public:
    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;

protected:
    void left_rotation(node *&subtree_root) const;
    void right_rotation(node *&subtree_root) const;
};

// region iterators implementation

// region prefix_iterator implementation

// --------------------------------------------------------------------------------------------------------------------------

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::prefix_iterator(
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator==(
    binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator const &other) const
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator++()
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator++(
    int not_used)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &> binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator*() const
{
}

// endregion prefix_iterator implementation

// region infix_iterator implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::infix_iterator(
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree)
{
    _current_node = tree;
    while (_current_node && _current_node->left_subtree_address)
    {
        _way.push(_current_node);
        _current_node = _current_node->left_subtree_address;
    }
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator==(
    binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator const &other) const
{

    if (_way == other._way && _current_node == other._current_node)
    {
        return true;
    }
    return false;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator!=(binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator const &other) const
{
    return !(*this == other);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++()
{

    if (_current_node == nullptr)
    {
        return *this;
    }

    if (_current_node->right_subtree_address != nullptr)
    {
        _way.push(_current_node);
        _current_node = _current_node->right_subtree_address;

        while (_current_node->left_subtree_address != nullptr)
        {
            _way.push(_current_node);
            _current_node = _current_node->left_subtree_address;
        }
    }
    else
    {
        if (!_way.empty())
        {
            if (_way.top()->left_subtree_address == _current_node)
            {
                _current_node = _way.top();
                _way.pop();
            }
            else
            {
                while (!_way.empty() && _way.top()->right_subtree_address == _current_node)
                {
                    _current_node = _way.top();
                    _way.pop();
                }

                if (_way.empty())
                {
                    _current_node = nullptr;
                    return *this;
                }

                _current_node = _way.top();
                _way.pop();
            }
        }
        else
        {
            _current_node = nullptr;
        }
    }
    return *this;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++(
    int not_used)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator prev_state = *this;

    ++(*this);

    return prev_state;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &> binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator*() const
{
    return std::tuple<unsigned int, tkey const &, tvalue const &>(_way.size(), _current_node->key, _current_node->value);
}

// endregion infix_iterator implementation

// region postfix_iterator implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::postfix_iterator(
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator==(
    binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator const &other) const
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator++()
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator++(
    int not_used)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &> binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator*() const
{
}

// endregion prefix_iterator implementation

// endregion iterators implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_compare>
void binary_search_tree<tkey, tvalue, tkey_compare>::left_rotation(binary_search_tree<tkey, tvalue, tkey_compare>::node *&subtree_root) const
{
    node *temp = subtree_root;
    subtree_root = subtree_root->right_subtree_address;
    temp->right_subtree_address = subtree_root->left_subtree_address;
    subtree_root->lef_subtree_address = temp;
}

// region template methods implementation

// region insertion implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_root_address)
{
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> path_to_subtree_root_exclusive;
    insert_inner(key, std::move(value), tree_root_address, path_to_subtree_root_exclusive);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
size_t binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::get_node_size() const
{
    return sizeof(binary_search_tree<tkey, tvalue, tkey_comparer>::node);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::call_constructor_node(node *mem, tkey const &key, tvalue &&value) const
{
    new (mem) binary_search_tree<tkey, tvalue, tkey_comparer>::node{key, std::move(value), nullptr, nullptr};
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert_inner(
    tkey const &key,
    tvalue &&value,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

    if (subtree_root_address == nullptr)
    {
        // std::cout << "gg" << std::endl;
        subtree_root_address = reinterpret_cast<node *>(allocate_with_guard(get_node_size()));

        this->trace_with_guard("Allocated root");
        // TODO: logs

        call_constructor_node(subtree_root_address, key, std::move(value));
    }
    else
    {

        int res_functor = _tree->_comparator(key, subtree_root_address->key);
        if (res_functor == 0)
        {
            // TODO: ALL_MY_EXCEPTION_TYPE
            throw std::exception();
        }
        path_to_subtree_root_exclusive.push(&subtree_root_address);

        insert_inner(key, std::move(value), res_functor > 0 ? subtree_root_address->right_subtree_address : subtree_root_address->left_subtree_address, path_to_subtree_root_exclusive);
        path_to_subtree_root_exclusive.pop();
    }
    after_insert_inner(key, value, subtree_root_address, path_to_subtree_root_exclusive);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::before_insert_inner(
    tkey const &key,
    tvalue &&value,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::after_insert_inner(
    tkey const &key,
    tvalue const &value,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
memory *binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::get_memory() const
{
    return _tree->get_memory();
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::get_logger() const
{
    return _tree->get_logger();
}

// endregion insertion implementation

// region reading implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue const &binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read(
    tkey const &key,
    node *&tree_root_address)
{
    std::stack<binary_search_tree::node **> path_to_subtree_root_exclusive;
    return read_inner(key, tree_root_address, path_to_subtree_root_exclusive);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue const &binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read_inner(
    tkey const &key,
    node *&subtree_root_address,
    std::stack<node **> &path_to_subtree_root_exclusive)
{
    before_read_inner(key, subtree_root_address, path_to_subtree_root_exclusive);

    after_read_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::before_read_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::after_read_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

// endregion reading implementation

// region removing implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue &&binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_root_address)
{
    std::stack<binary_search_tree::node **> path_to_subtree_root_exclusive;
    return std::move(remove_inner(key, tree_root_address, path_to_subtree_root_exclusive));
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue &&binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

    if (subtree_root_address == nullptr)
    {
        this->warning_with_guard("Tree is empty");
    }

    auto current_node = &subtree_root_address;

    node **removed_node = nullptr;

    while (*current_node != nullptr)
    {
        int compare_result = _tree->_comparator(key, (*current_node)->key);

        if (compare_result == 0)
        {
            removed_node = current_node;
            break;
        }
        else
        {
            path_to_subtree_root_exclusive.push_back(current_node);
            current_node = &(compare_result > 0 ? (*current_node)->right_subtree_address : (*current_node)->left_subtree_address);
        }
    }

    if (removed_node == nullptr)
    {
        this->warning_with_guard("Key not found");
    }

    tvalue &&remuved_value = std::move((*removed_node)->value);

    auto removal_complated = false;

    if ((*removed_node)->left_subtree_address != nullptr && (*removed_node)->right_subtree_address != nullptr)
    {
        auto succesor = (*removed_node)->right_subtree_address;
        auto right_subtree_root = &(*removed_node)->right_subtree_address;

        if (succesor->left_subtree_address == nullptr)
        {
            succesor->left_subtree_address = (*removed_node)->left_subtree_address;
            (*removed_node)->~node();
            deallocate_with_guard(*removed_node);
            *removed_node = succesor;
            removal_complated = true;
        }
        else
        {
        }
    }

    before_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);

    after_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::before_remove_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::after_remove_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

// endregion implementation

// endregion template methods

// region construction, assignment, destruction implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
    binary_search_tree::insertion_template_method *insertion,
    binary_search_tree::reading_template_method *reading,
    binary_search_tree::removing_template_method *removing,
    memory *allocator,
    logger *logger)
    : _insertion(insertion),
      _reading(reading),
      _removing(removing),
      _allocator(allocator),
      _logger(logger),
      _comparator()
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
    memory *allocator,
    logger *logger) : _insertion(new insertion_template_method(this)),
                      _reading(new reading_template_method()),
                      _removing(new removing_template_method()),
                      _allocator(allocator),
                      _logger(logger),
                      _root(nullptr)
{
    this->trace_with_guard("The tree has been created.");
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
    const binary_search_tree &other)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
    binary_search_tree &&other) noexcept
{
    move(std::move(other));
    this->trace_with_guard("The tree has been moved.");
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer> &binary_search_tree<tkey, tvalue, tkey_comparer>::operator=(
    const binary_search_tree &other)
{
    if (this == &other)
    {
        return *this;
    }

    return *this;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer> &binary_search_tree<tkey, tvalue, tkey_comparer>::operator=(
    binary_search_tree &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    return *this;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::~binary_search_tree()
{
    // TODO: remove all elements

    delete _insertion;
    delete _reading;
    delete _removing;
}

// endregion construction, assignment, destruction implementation

// region associative_container contract implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insert(
    tkey const &key,
    tvalue &&value)
{
    _insertion->insert(key, std::move(value), _root);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue const &binary_search_tree<tkey, tvalue, tkey_comparer>::get(
    tkey const &key)
{
    return _reading->read(key, _root);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue &&binary_search_tree<tkey, tvalue, tkey_comparer>::remove(
    tkey const &key)
{
    return std::move(_removing->remove(key, _root));
}

// endregion associative_container contract implementation

// region logger_holder contract implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::get_logger() const
{
    return _logger;
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
memory *binary_search_tree<tkey, tvalue, tkey_comparer>::get_memory() const
{
    return _allocator;
}

// endregion logger_holder contract implementation

// region iterators requesting implementation

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_prefix() const noexcept
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_prefix() const noexcept
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_infix() const noexcept
{
    return infix_iterator(_root);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_infix() const noexcept
{
    return infix_iterator(nullptr);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_postfix() const noexcept
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_postfix() const noexcept
{
}

#endif // BINARY_SEARCH_TREE_H