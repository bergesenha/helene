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
    typedef StoragePolicy<sliding_window<T, Size, StoragePolicy>, T, Size>
        storage_policy;

public:
    static const std::size_t size = Size;

    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef circular_iterator<T*> iterator;
    typedef circular_iterator<const T*> const_iterator;

public:
    sliding_window()
        : head_(storage_policy::begin(),
                storage_policy::end(),
                storage_policy::begin()),
          tail_(storage_policy::begin(),
                storage_policy::end(),
                storage_policy::end())
    {
    }

    void
    push_back(const T& value)
    {
        *tail_ = value;
        ++tail_;
        ++head_;
    }

    void
    push_front(const T& value)
    {
        --head_;
        *head_ = value;
        --tail_;
    }

    reference
    front()
    {
        return *head_;
    }

    reference
    back()
    {
        return *(tail_ - 1);
    }

    reference operator[](size_type n)
    {
        return head_[n];
    }

    const_reference operator[](size_type n) const
    {
        return head_[n];
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

    T*
    begin()
    {
        return buffer_;
    }

    T*
    end()
    {
        return buffer_ + Size;
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

    T*
    begin()
    {
        return buffer_.get();
    }

    T*
    end()
    {
        return buffer_.get() + Size;
    }

protected:
    std::unique_ptr<T[]> buffer_;
};


template <class T, std::size_t Size>
using stack_sliding_window = sliding_window<T, Size, stack_storage>;

template <class T, std::size_t Size>
using static_heap_sliding_window = sliding_window<T, Size, static_heap_storage>;
} // namespace helene
