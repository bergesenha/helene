#pragma once

#include <cstddef>
#include <memory>

#include <circular_iterator.hpp>


namespace helene
{
template <class T,
          std::size_t Size,
          template <class, class, std::size_t> class StoragePolicy>
class sliding_window
    : public StoragePolicy<sliding_window<T, Size, StoragePolicy>, T, Size>
{
public:
    static const std::size_t size = Size;
    typedef T value_type;
    typedef std::size_t size_type;
    typedef circular_iterator<T*> iterator;
    typedef circular_iterator<const T*> const_iterator;

public:
    sliding_window() = default;

    void
    push_back(const T& value)
    {
    }

    void
    push_front(const T& value)
    {
    }

private:
    iterator head_;
    iterator tail_;
};

template <class Derived, class T, std::size_t Size>
class stack_storage
{
public:
    stack_storage() : buffer_{}
    {
    }

protected:
    T buffer_[Size];
};

template <class Derived, class T, std::size_t Size>
class static_heap_storage
{
public:
    static_heap_storage() : buffer_(new T[Size]{})
    {
    }

protected:
    std::unique_ptr<T[]> buffer_;
};


template <class T, std::size_t Size>
using stack_sliding_window = sliding_window<T, Size, stack_storage>;

template <class T, std::size_t Size>
using static_heap_sliding_window = sliding_window<T, Size, static_heap_storage>;
} // namespace helene
