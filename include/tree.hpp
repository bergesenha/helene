#pragma once

#include <vector>

#include "handle_map.hpp"
#include "small_vector.hpp"


namespace helene
{
template <class T>
class tree
{
public:
    typedef typename handle_map<T>::handle_type node_tag_type;

public:
    tree() = default;

private:
    handle_map<T> nodes_;
    handle_map<node_tag_type> parents_;
    handle_map<small_vector<node_tag_type>> children_;
};
} // namespace helene
