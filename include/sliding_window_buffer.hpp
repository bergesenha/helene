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

public:
    sliding_window_buffer()
        : buffer_{},
          origin_(std::begin(buffer_), std::end(buffer_), std::begin(buffer_))
    {
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

    constexpr size_type
    size() const
    {
        return Size;
    }

private:
    T buffer_[Size];
    iterator origin_;
};
} // namespace helene
