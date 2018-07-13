#pragma once

#include <type_traits>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <iterator>


namespace helene
{

template <class T, std::size_t StackBufferSize = sizeof(std::vector<T>*)>
class small_vector
{
public:
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::difference_type difference_type;

    static constexpr size_type max_stack_size = StackBufferSize / sizeof(T);

public:
private:
    std::aligned_storage_t<StackBufferSize> storage_;
    size_type size_;
};

} // namespace helene
