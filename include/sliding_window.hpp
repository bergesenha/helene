#pragma once

#include <cstddef>
#include <memory>
#include <functional>
#include <ratio>
#include <iterator>
#include <initializer_list>

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
    typedef std::ptrdiff_t difference_type;
    typedef circular_iterator<T*> iterator;
    typedef circular_iterator<const T*> const_iterator;

public:
    sliding_window()
        : storage_policy(),
          head_(storage_policy::begin(),
                storage_policy::end(),
                storage_policy::begin())
    {
    }

    template <class Iterator>
    sliding_window(Iterator first, Iterator last) : sliding_window()
    {
        for(; first != last; ++first, ++head_)
        {
            *head_ = *first;
        }
    }

    sliding_window(std::initializer_list<T> il)
        : sliding_window(il.begin(), il.end())
    {
    }

    void
    push_back(const T& value)
    {
        *head_ = value;
        ++head_;
    }

    void
    push_back(const T& value, size_type n)
    {
        std::fill_n(head_, n, value);
        head_ += n;
    }

    void
    push_front(const T& value)
    {
        --head_;
        *head_ = value;
    }

    void
    push_front(const T& value, size_type n)
    {
        std::fill_n(std::make_reverse_iterator(head_), n, value);
        head_ -= n;
    }

    reference
    front()
    {
        return *head_;
    }

    const_reference
    front() const
    {
        return *head_;
    }

    reference
    back()
    {
        return *(head_ + Size - 1);
    }

    const_reference
    back() const
    {
        return *(head_ + Size - 1);
    }

    reference operator[](size_type n)
    {
        return head_[n];
    }

    const_reference operator[](size_type n) const
    {
        return head_[n];
    }

    iterator
    begin()
    {
        return head_;
    }

    iterator
    end()
    {
        return head_ + Size;
    }

private:
    iterator head_;
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


namespace detail
{
struct runtime_ratio
{
    runtime_ratio() : num(1), den(1)
    {
    }

    template <std::intmax_t Num, std::intmax_t Denom>
    constexpr runtime_ratio(std::ratio<Num, Denom>) : num(Num), den(Denom)
    {
    }

    std::intmax_t num;
    std::intmax_t den;
};

template <class Numeric>
Numeric operator*(const runtime_ratio& lhs, Numeric rhs)
{
    return rhs * static_cast<Numeric>(lhs.num) / static_cast<Numeric>(lhs.den);
}

template <class Numeric>
Numeric operator*(Numeric lhs, const runtime_ratio& rhs)
{
    return rhs * lhs;
}


template <class Numeric>
Numeric
operator/(Numeric lhs, const runtime_ratio& rhs)
{
    return lhs * static_cast<Numeric>(rhs.den) / static_cast<Numeric>(rhs.num);
}
} // namespace detail


template <class KeyType,
          class ValueType,
          std::size_t Size,
          class Compare = std::less<KeyType>>
class sliding_window_map
{
public:
    sliding_window_map() = default;
    template <std::intmax_t Num, std::intmax_t Denom>
    sliding_window_map(std::ratio<Num, Denom>)
        : sliding_buffer_(), origin_(), precision_(std::ratio<Num, Denom>())
    {
    }

    std::pair<KeyType, KeyType>
    extent() const
    {
        return std::make_pair(origin_, origin_ + Size * precision_);
    }

    ////////////////////////////////////////////////////////////////////////////
    // map like interface
    ValueType&
    at(KeyType k)
    {
        const auto index = index_of_key(k);
        const Compare c;
        if(c(index, 0) || (!c(index, Size)))
        {
            throw std::out_of_range("Key outside of current window");
        }

        return sliding_buffer_[index];
    }


private:
    std::size_t
    index_of_key(KeyType k)
    {
        return (k - origin_) / precision_;
    }

private:
    static_heap_sliding_window<ValueType, Size> sliding_buffer_;
    KeyType origin_;
    detail::runtime_ratio precision_;
};
} // namespace helene
