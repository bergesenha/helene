#pragma once

#include <type_traits>
#include <vector>
#include <initializer_list>
#include <cstddef>
#include <algorithm>
#include <iterator>


namespace helene
{

template <class T, std::size_t StackBufferSize = sizeof(std::vector<T>)>
class small_vector
{
    static_assert(std::is_trivial<T>::value,
                  "small_vector is only compatible with trivial types");

public:
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::difference_type difference_type;
    typedef T* iterator;
    typedef const T* const_iterator;

private:
    static constexpr size_type at_least_size =
        std::max(StackBufferSize, sizeof(std::vector<T>*));
    static constexpr size_type max_stack_size_ = at_least_size / sizeof(T);

public:
    small_vector() : size_()
    {
        new(&storage_) T[max_stack_size_];
    }

    small_vector(std::initializer_list<T> init) : size_(init.size())
    {
        if(size_ > max_stack_size_)
        {
            new(&storage_) std::vector<T>(init);
        }
        else
        {
            new(&storage_) T[max_stack_size_];
            std::copy(
                init.begin(), init.end(), reinterpret_cast<T*>(&storage_));
        }
    }

    small_vector(size_type count) : size_(count)
    {
        if(size_ > max_stack_size_)
        {
            new(&storage_) std::vector<T>(count);
        }
        else
        {
            new(&storage_) T[max_stack_size_];
            std::fill_n(reinterpret_cast<T*>(&storage_), size_, T{});
        }
    }

    ~small_vector()
    {
        if(size_ > max_stack_size_)
        {
            reinterpret_cast<std::vector<T>*>(&storage_)->~vector();
        }
    }

public:
    void
    push_back(const T& value)
    {
        if(size_ > max_stack_size_)
        {
            reinterpret_cast<std::vector<T>*>(&storage_)->push_back(value);
        }
        else if(size_ == max_stack_size_)
        {
            T* buff = reinterpret_cast<T*>(&storage_);
            T temp[max_stack_size_];
            std::copy(buff, buff + size_, std::begin(temp));

            new(&storage_) std::vector<T>(std::begin(temp), std::end(temp));
            reinterpret_cast<std::vector<T>*>(&storage_)->push_back(value);
        }
        else
        {
            reinterpret_cast<T*>(&storage_)[size_] = value;
        }

        ++size_;
    }

    T& operator[](size_type n)
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->operator[](n);
        }
        else
        {
            return reinterpret_cast<T*>(&storage_)[n];
        }
    }

    const T& operator[](size_type n) const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->operator[](n);
        }
        else
        {
            return reinterpret_cast<T*>(&storage_)[n];
        }
    }


    iterator
    begin()
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->data();
        }
        else
        {
            return reinterpret_cast<T*>(&storage_);
        }
    }

    iterator
    end()
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<std::vector<T>*>(&storage_)->data() + size_;
        }
        else
        {
            return reinterpret_cast<T*>(&storage_) + size_;
        }
    }

    const_iterator
    cbegin() const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<const std::vector<T>*>(&storage_)->data();
        }
        else
        {
            return reinterpret_cast<const T*>(&storage_);
        }
    }

    const_iterator
    cend() const
    {
        if(size_ > max_stack_size_)
        {
            return reinterpret_cast<const std::vector<T>*>(&storage_)->data() +
                   size_;
        }
        else
        {
            return reinterpret_cast<const T*>(&storage_) + size_;
        }
    }

    size_type
    size() const
    {
        return size_;
    }

    static constexpr size_type
    max_stack_size()
    {
        return max_stack_size_;
    }

    bool
    on_stack() const
    {
        return size_ <= max_stack_size_;
    }

private:
    std::aligned_storage_t<at_least_size> storage_;
    size_type size_;
};

} // namespace helene
