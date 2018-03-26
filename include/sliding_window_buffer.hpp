#pragma once

#include <cstddef>

#include "circular_iterator.hpp"

namespace helene
{
template <class T, std::size_t Size>
class sliding_window_buffer
{
public:
    typedef std::size_t size_type;
    typedef circular_iterator<T*> iterator;
    typedef T& reference;
    typedef const T& const_reference;

public:
    sliding_window_buffer()
        : buffer_{},
          origin_(std::begin(buffer_), std::end(buffer_), std::begin(buffer_))
    {
    }

    template <class Iterator>
    sliding_window_buffer(Iterator first, Iterator last)
        : sliding_window_buffer()
    {
        std::copy(first, last, origin_);
    }

    T& operator[](size_type n)
    {
        return origin_[static_cast<typename iterator::difference_type>(n)];
    }

    const T& operator[](size_type n) const
    {
        return origin_[static_cast<typename iterator::difference_type>(n)];
    }

    void
    push_back(const T& value)
    {
        origin_[Size] = value;
        ++origin_;
    }

    void
    push_front(const T& value)
    {
        --origin_;
        *origin_ = value;
    }

    reference
    back()
    {
        return origin_[Size - 1];
    }

    const_reference
    back() const
    {
        return origin_[Size - 1];
    }

    reference
    front()
    {
        return *origin_;
    }

    const_reference
    front() const
    {
        return *origin_;
    }

    constexpr size_type
    size() const
    {
        return Size;
    }

    iterator
    begin()
    {
        return origin_;
    }

    iterator
    end()
    {
        return origin_ + Size;
    }

private:
    T buffer_[Size];
    iterator origin_;
};
} // namespace helene
