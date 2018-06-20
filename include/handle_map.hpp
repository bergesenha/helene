#pragma once


#include <vector>

namespace helene
{
template <class T, class Allocator = std::allocator<T>>
class handle_map
{
public:
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<size_type>::size_type handle_type;

public:
    class iterator
    {
    public:
    private:
        std::vector<T>* dense_ref_;
        std::vector<size_type>* sparse_ref_;
        handle_type current_;
    };

    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

public:
    handle_type
    insert(const T& value)
    {
        // always insert at end of dense storage
        dense_.push_back(value);
        // find available handle
        if(free_.empty())
        {
            // no free handle slots, push back new slot
            sparse_.push_back(dense_.size() - 1);
            return sparse_.size() - 1;
        }

        // slot available
        handle_type available = free_.back();
        sparse_[available] = dense_.size() - 1;
        free_.pop_back();
        return available;
    }

    size_type
    size() const
    {
        return dense_.size();
    }

    T& operator[](handle_type n)
    {
        return dense_[sparse_[n]];
    }

    const T& operator[](handle_type n) const
    {
        return dense_[sparse_[n]];
    }

private:
    std::vector<T> dense_;
    std::vector<size_type> sparse_;
    std::vector<handle_type> reverse_;
    std::vector<handle_type> free_;
};
} // namespace helene
