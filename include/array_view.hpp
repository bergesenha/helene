#pragma once

#include <cstddef>
#include <iterator>


namespace helene
{
template <class T>
class array_view
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef typename std::iterator_traits<iterator>::difference_type
        difference_type;
    typedef size_t size_type;

public:
    array_view() : decayed_(nullptr), size_(0)
    {
    }

    template <std::size_t N>
    array_view(T (&arr)[N]) : decayed_(arr), size_(N)
    {
    }

    size_type
    size() const
    {
        return size_;
    }

    bool
    empty() const
    {
        return size_ == 0;
    }

    iterator
    begin()
    {
        return decayed_;
    }

    iterator
    end()
    {
        return decayed_ + size_;
    }

    const_iterator
    cbegin() const
    {
        return decayed_;
    }

    const_iterator
    cend() const
    {
        return decayed_ + size_;
    }

    reference operator[](size_t n)
    {
        return decayed_[n];
    }

    const_reference operator[](size_t n) const
    {
        return decayed_[n];
    }

private:
    T* decayed_;
    std::size_t size_;
};
}
