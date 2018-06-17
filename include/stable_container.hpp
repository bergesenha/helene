#pragma once

#include <vector>
#include <algorithm>


namespace helene
{

namespace detail
{
}

template <class T, class Allocator = std::allocator<T>>
class stable_vector
{
    typedef std::vector<T, Allocator> vector_type;

    enum class element_state
    {
        occupied,
        vacant
    };


public:
    typedef typename vector_type::size_type size_type;

public:
    void
    add(const T& value)
    {
        // find next vacant position
        auto found =
            std::find(index_.begin(), index_.end(), element_state::vacant);

        if(found != index_.end())
        {
            const auto elm_index = found - index_.begin();

            new(&data_[elm_index]) T(value);

            *found = element_state::occupied;
        }
        else
        {
            data_.push_back(value);
            index_.push_back(element_state::occupied);
        }
    }

    T& operator[](size_type n)
    {
        return data_[n];
    }


private:
    vector_type data_;
    std::vector<element_state, Allocator> index_;
};
} // namespace helene
