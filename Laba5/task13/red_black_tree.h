#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include "../Task11/binary_search_tree.h"

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
class red_black_tree : public binary_search_tree<tkey, tvalue, tkey_comparer>
{
protected:
    enum class color_node
    {
        RED,
        BLACK
    };

    struct red_black_node : public binary_search_tree<tkey, tvalue, tkey_comparer>::node
    {
        color_node color;
    };

protected:
    class red_black_insertion_template_method : public binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method
    {
    private:
        enum class balance_status
        {
            NO_BALANCE_NEEDED,
            BALANCE_AT_GP,
            BALANCE_AT_P
        };

        enum class rise_status
        {
            FROM_LEFT_SUBTREE,
            FROM_RIGHT_SUBTREE,
            NOTHING
        };

    private:
        red_black_tree<tkey, tvalue, tkey_comparer> *_tree;
        balance_status _balance_status;
        rise_status _rise_further_status;
        rise_status _rise_near_status;

    private:
        size_t get_node_size() const override;

    protected:
        void before_insert() override
        {
            _balance_status = balance_status::BALANCE_AT_GP;
            _rise_further_status = rise_status::NOTHING;
            _rise_near_status = rise_status::NOTHING;
        };

        void after_insert_inner(
            tkey const &key,
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
            std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    public:
        explicit red_black_insertion_template_method(
            red_black_tree<tkey, tvalue, tkey_comparer> *tree);
    };

protected:
    class red_black_removing_template_method : public binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method
    {
    private:
        red_black_tree<tkey, tvalue, tkey_comparer> *_tree;

    protected:
        void after_remove_inner(
            tkey const &key,
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
            std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive) override;

    public:
        explicit red_black_removing_template_method(
            red_black_tree<tkey, tvalue, tkey_comparer> *tree);
    };

public:
    explicit red_black_tree(memory *allocator = nullptr,
                            logger *logger = nullptr);

    ~red_black_tree() final = default;

private:
    color_node get_color(red_black_node *current_node);

    // protected:
    //     void left_rotation(binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root) const override;
    //     void right_rotation(binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root) const override;
};

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
size_t red_black_tree<tkey, tvalue, tkey_comparer>::red_black_insertion_template_method::get_node_size() const
{
    return sizeof(red_black_node);
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename red_black_tree<tkey, tvalue, tkey_comparer>::color_node red_black_tree<tkey, tvalue, tkey_comparer>::get_color(
    red_black_node *current_node)
{
    return current_node == nullptr ? color_node::BLACK : current_node->color;
}

// begin region insertion template method

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_insertion_template_method::after_insert_inner(
    tkey const &key,
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    switch (_balance_status)
    {
    case balance_status::NO_BALANCE_NEEDED:
        return;
    case balance_status::BALANCE_AT_GP:
        _balance_status = balance_status::BALANCE_AT_P;
        _rise_further_status = _rise_near_status;
        _rise_near_status = (*path_to_subtree_root_exclusive.top())->left_subtree_address == subtree_root_address
                                ? rise_status::FROM_LEFT_SUBTREE
                                : rise_status::FROM_RIGHT_SUBTREE;
        return;
    }

    if (path_to_subtree_root_exclusive.empty())
    {
        reinterpret_cast<red_black_node *>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        return;
    }
    else if (subtree_root_address->left_subtree_address == nullptr && subtree_root_address->right_subtree_address == nullptr)
    {
        reinterpret_cast<red_black_node *>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;

        if (path_to_subtree_root_exclusive.size() == 1)
        {
            return;
        }
    }

    red_black_node **parent = nullptr;
    red_black_node *grandson = nullptr;
    red_black_node *uncle = nullptr;

    // parent = reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node **>(path_to_subtree_root_exclusive.top());
    // path_to_subtree_root_exclusive.pop();
    // grand_parent = reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node **>(path_to_subtree_root_exclusive.top());
    path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(parent));
    // //uncle = reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node *>((*grand_parent)->left_subtree_address == *parent
    //                                                                                                      ? (*grand_parent)->right_subtree_address
    //                                                                                                      : (*grand_parent)->left_subtree_address);

    uncle = reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node *>(_rise_near_status == rise_status::FROM_LEFT_SUBTREE
                                                                                                         ? subtree_root_address->right_subtree_address
                                                                                                         : subtree_root_address->left_subtree_address);

    parent = reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node **>(_rise_near_status == rise_status::FROM_LEFT_SUBTREE
                                                                                                           ? &subtree_root_address->right_subtree_address
                                                                                                           : &subtree_root_address->left_subtree_address);

    grandson = reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node *>(_rise_further_status == rise_status::FROM_LEFT_SUBTREE
                                                                                                            ? (*parent)->right_subtree_address
                                                                                                            : (*parent)->left_subtree_address);

    if (_tree->get_color(grandson) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK ||
        _tree->get_color(*parent) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
    {
        _balance_status = balance_status::NO_BALANCE_NEEDED;
        return;
    }

    if (_rise_further_status != _rise_near_status)
    {
        if (_rise_near_status == rise_status::FROM_LEFT_SUBTREE)
        {
            _tree->left_rotation(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(parent));
            grandson = reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node *>((*parent)->left_subtree_address);
        }
        else
        {
            _tree->right_rotation(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(parent));
            grandson = reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node *>((*parent)->right_subtree_address);
        }
    }

    if (_tree->get_color(uncle) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
    {
        (*reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node **>(subtree_root_address))->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;

        if (_rise_near_status == rise_status::FROM_LEFT_SUBTREE)
        {
            _tree->right_rotation(&subtree_root_address);
        }
        else
        {
            _tree->left_rotation(&subtree_root_address);
        }

        (*reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node **>(subtree_root_address))->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;

        _balance_status = balance_status::NO_BALANCE_NEEDED;
    }
    else
    {
        uncle->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        (*parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        (*reinterpret_cast<typename red_black_tree<tkey, tvalue, tkey_comparer>::red_black_node **>(subtree_root_address))->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;

        _balance_status = balance_status::BALANCE_AT_GP;
    }
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_insertion_template_method::red_black_insertion_template_method(
    red_black_tree<tkey, tvalue, tkey_comparer> *tree)
    : _tree(tree),
      binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method(tree)
{
}

// end region insertion template method

// begin region removing template method

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_removing_template_method::after_remove_inner(
    tkey const &key,
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_removing_template_method::red_black_removing_template_method(
    red_black_tree<tkey, tvalue, tkey_comparer> *tree)
    : _tree(tree),
      binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method(tree)
{
}

// end region removing template method

// begin region constructor

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree(memory *allocator, logger *logger)
    : binary_search_tree<tkey, tvalue, tkey_comparer>(
          new red_black_insertion_template_method(this),
          new typename binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method(this),
          new red_black_removing_template_method(this),
          allocator,
          logger)
{

    std::cout << "gg" << std::endl;
}

// end region constructor

#endif // RED_BLACK_TREE_H
