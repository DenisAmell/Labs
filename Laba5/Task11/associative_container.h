#ifndef ASSOCIATIVE_CONTAINER_H
#define ASSOCIATIVE_CONTAINER_H

template <
    typename tkey,
    typename tvalue>
class associative_container
{

public:
    struct key_and_value_pair
    {
        tkey key;
        tvalue value;
    };

public:
    virtual ~associative_container() = default;

public:
    virtual void insert(
        tkey const &key,
        tvalue &&value) = 0;

    virtual bool find(
        typename associative_container<tkey, tvalue>::key_and_value_pair *target_key_and_result_value) = 0;

    virtual tvalue &&remove(
        tkey const &key) = 0;
};

#endif // ASSOCIATIVE_CONTAINER_H