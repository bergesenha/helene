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

public:
    static constexpr size_type max_stack_size = at_least_size / sizeof(T);

public:
    small_vector() : size_()
    {
        new(&storage_) T[max_stack_size];
    }

    ~small_vector()
    {
        if(size_ > max_stack_size)
        {
            reinterpret_cast<std::vector<T>*>(&storage_)->~vector();
        }
    }

private:
    std::aligned_storage_t<at_least_size> storage_;
    size_type size_;
};

} // namespace helene
