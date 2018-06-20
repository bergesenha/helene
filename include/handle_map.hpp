#pragma once


#include <vector>

namespace helene
{
template <class T, class Allocator = std::allocator<T>>
class handle_map
{
public:
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<size_type>::size_type index_type;

public:
    index_type
    insert(const T& value)
    {
        dense_.push_back(value);
        sparse_.push_back(dense_.size() - 1);
        return sparse_.size() - 1;
    }

    size_type
    size() const
    {
        return dense_.size();
    }

    T& operator[](index_type n)
    {
        return dense_[sparse_[n]];
    }

    const T& operator[](index_type n) const
    {
        return dense_[sparse_[n]];
    }

private:
    std::vector<T> dense_;
    std::vector<size_type> sparse_;
};
} // namespace helene
