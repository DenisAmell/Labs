#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include "../../Task11/binary_search_tree.h"

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
        red_black_tree<tkey, tvalue, tkey_comparer>* _tree;

    private:
        size_t get_node_size() const override;

    protected:
        void after_insert_inner(
            tkey const& key,
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*& subtree_root_address,
            std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>& path_to_subtree_root_exclusive) override;

    public:
        explicit red_black_insertion_template_method(
            red_black_tree<tkey, tvalue, tkey_comparer>* tree);
    };

protected:
    class red_black_removing_template_method : public binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method
    {
    private:
        red_black_tree<tkey, tvalue, tkey_comparer>* _tree;

    protected:
        void after_remove_inner(
            tkey const& key,
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*& subtree_root_address,
            std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>& path_to_subtree_root_exclusive) override;

    public:
        explicit red_black_removing_template_method(
            red_black_tree<tkey, tvalue, tkey_comparer>* tree);
    };

public:
    explicit red_black_tree(memory* allocator = nullptr,
        logger* logger = nullptr);

    ~red_black_tree() final = default;

private:
    color_node get_color(red_black_node* current_node);

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
        red_black_node* current_node)
{
    return current_node == nullptr ? color_node::BLACK : current_node->color;
}

// begin region insertion template method

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
    void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_insertion_template_method::after_insert_inner(
        tkey const& key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*& subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>& path_to_subtree_root_exclusive)
{
    if (path_to_subtree_root_exclusive.empty())
    {
        reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        return;
    }
    else
    {
        if (subtree_root_address->left_subtree_address == nullptr && subtree_root_address->right_subtree_address == nullptr)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
        }
    }

    red_black_node** parent = nullptr;
    red_black_node** uncle = nullptr;
    red_black_node** grand_parent = nullptr;
    red_black_node** great_grand_parent = nullptr;

    if (!path_to_subtree_root_exclusive.empty())
    {
        parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
        path_to_subtree_root_exclusive.pop();

        if (!path_to_subtree_root_exclusive.empty())
        {
            grand_parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
            path_to_subtree_root_exclusive.pop();

            if ((*grand_parent)->left_subtree_address == *parent)
            {
                uncle = reinterpret_cast<red_black_node**>(&((*grand_parent)->right_subtree_address));
            }
            else
            {
                uncle = reinterpret_cast<red_black_node**>(&((*grand_parent)->left_subtree_address));
            }
            if (!path_to_subtree_root_exclusive.empty())
            {
                great_grand_parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
                path_to_subtree_root_exclusive.pop();
            }
        }
    }

    if (parent != nullptr && _tree->get_color(*parent) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
    {
        if (great_grand_parent != nullptr)
        {
            path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(great_grand_parent));
        }

        if (grand_parent != nullptr)
        {
            path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent));
        }

        path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(parent));

        return;
    }
    else
    {
        if (parent != nullptr && grand_parent != nullptr && _tree->get_color(*parent) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
        {
            if ((*parent)->right_subtree_address == subtree_root_address && *parent == (*grand_parent)->left_subtree_address)
            {
                _tree->left_rotation(*reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(parent));
                // _tree->left_rotation(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(parent), reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(grand_parent));
                subtree_root_address = (*parent)->left_subtree_address;
            }
            else
            {
                if (subtree_root_address == (*parent)->left_subtree_address && *parent == (*grand_parent)->right_subtree_address)
                {
                    _tree->right_rotation(*reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(parent));
                    // _tree->right_rotation(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(parent), reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(grand_parent));
                    subtree_root_address = (*parent)->right_subtree_address;
                }
            }

            if (_tree->get_color(*uncle) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
            {
                (*parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*grand_parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;

                if (subtree_root_address == (*parent)->left_subtree_address && *parent == (*grand_parent)->left_subtree_address)
                {
                    _tree->right_rotation(*reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent));
                    //  _tree->right_rotation(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(grand_parent), reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(great_grand_parent));
                    *parent = reinterpret_cast<red_black_node*>((*grand_parent)->right_subtree_address);
                }
                else
                {

                    _tree->left_rotation(*reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent));
                    // _tree->left_rotation(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(grand_parent), reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node **>(great_grand_parent));
                    *parent = reinterpret_cast<red_black_node*>((*grand_parent)->left_subtree_address);
                }

                if (great_grand_parent != nullptr)
                {
                    path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(great_grand_parent));
                }

                path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent));
            }
            else
            {
                if (great_grand_parent != nullptr)
                {
                    path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(great_grand_parent));
                }

                (*uncle)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*grand_parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;

                auto** binary_grand_parent = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(grand_parent);

                after_insert_inner(key, *binary_grand_parent, path_to_subtree_root_exclusive);

                path_to_subtree_root_exclusive.push(binary_grand_parent);
                path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(parent));
            }
        }
    }
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
    red_black_tree<tkey, tvalue, tkey_comparer>::red_black_insertion_template_method::red_black_insertion_template_method(
        red_black_tree<tkey, tvalue, tkey_comparer>* tree)
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
        tkey const& key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*& subtree_root_address,
        std::list<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>& path_to_subtree_root_exclusive)
{
}

template <
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
    red_black_tree<tkey, tvalue, tkey_comparer>::red_black_removing_template_method::red_black_removing_template_method(
        red_black_tree<tkey, tvalue, tkey_comparer>* tree)
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
    red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree(memory* allocator, logger* logger)
    : binary_search_tree<tkey, tvalue, tkey_comparer>(
        new red_black_insertion_template_method(this),
        new typename binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method(this),
        new red_black_removing_template_method(this),
        allocator,
        logger)
{
}

// end region constructor

#endif // RED_BLACK_TREE_H