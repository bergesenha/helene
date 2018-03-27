#pragma once

#include <cstddef>

#include "circular_iterator.hpp"

namespace helene
{
template <class T, std::size_t Size>
class sliding_window_buffer
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef circular_iterator<T*> iterator;
    typedef circular_iterator<const T*> const_iterator;
    typedef std::size_t size_type;

public:
    /**
     * \brief default constructor
     */
    sliding_window_buffer()
        : buffer_{},
          origin_(std::begin(buffer_), std::end(buffer_), std::begin(buffer_))
    {
    }

    /**
     * \brief constructor taking range, if std::distance(first, last) exceeds
     * Size, the resulting sliding_window_buffer constructed will contain the
     * Size last elements from range.
     */
    template <class Iterator>
    sliding_window_buffer(Iterator first, Iterator last)
        : sliding_window_buffer()
    {
        std::copy(first, last, origin_);
    }


    sliding_window_buffer(const sliding_window_buffer& other)
        : buffer_{},
          origin_(std::begin(buffer_), std::end(buffer_), std::begin(buffer_))
    {
        // copy buffer over
        std::copy(other.cbegin(), other.cend(), std::begin(buffer_));
    }


    sliding_window_buffer&
    operator=(const sliding_window_buffer& other)
    {
        // copy values
        std::copy(other.cbegin(), other.cend(), std::begin(buffer_));
        // reset origin_
        origin_ = iterator(
            std::begin(buffer_), std::end(buffer_), std::begin(buffer_));

        return *this;
    }

public:
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

    const_iterator
    cbegin() const
    {
        return origin_;
    }

    const_iterator
    cend() const
    {
        return origin_ + Size;
    }

    bool
    operator==(const sliding_window_buffer& other)
    {
        return std::equal(cbegin(), cend(), other.cbegin(), other.cend());
    }

    bool
    operator!=(const sliding_window_buffer& other)
    {
        return !operator==(other);
    }

    void
    swap(sliding_window_buffer& other)
    {
        using std::swap;

        swap(*this, other);
    }

    constexpr bool
    empty() const
    {
        return !Size;
    }

private:
    T buffer_[Size];
    iterator origin_;
};
} // namespace helene
