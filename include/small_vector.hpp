#pragma once

#include <type_traits>
#include <vector>
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

private:
    static constexpr size_type at_least_size =
        std::max(StackBufferSize, sizeof(std::vector<T>*));
    static constexpr size_type max_stack_size_ = at_least_size / sizeof(T);

public:
    small_vector() : size_()
    {
        new(&storage_) T[max_stack_size_];
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

    size_type
    size() const
    {
        return size_;
    }

    constexpr size_type
    max_stack_size() const
    {
        return max_stack_size_;
    }

private:
    std::aligned_storage_t<at_least_size> storage_;
    size_type size_;
};

} // namespace helene
