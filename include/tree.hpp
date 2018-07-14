#pragma once

#include <vector>
#include <utility>

#include "handle_map.hpp"
#include "small_vector.hpp"


namespace helene
{
template <class T>
class tree
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef typename handle_map<T>::handle_type node_tag_type;

    typedef typename small_vector<node_tag_type>::iterator child_tag_iterator;

private:
    static constexpr inline const node_tag_type inactive_tag =
        std::numeric_limits<node_tag_type>::max();

public:
    tree() : nodes_(), parents_(), children_()
    {
        nodes_.insert(T{});
        parents_.push_back(inactive_tag);
        children_.resize(1);
    }

    tree(const T& root_node) : nodes_(), parents_(), children_()
    {
        nodes_.insert(root_node);
        parents_.push_back(inactive_tag);
        children_.resize(1);
    }

    node_tag_type
    insert_node(const T& value, node_tag_type parent)
    {
        const auto new_tag = nodes_.insert(value);

        if(new_tag >= parents_.size())
        {
            parents_.resize(new_tag + 1, inactive_tag);
            children_.resize(new_tag + 1);
        }

        parents_[new_tag] = parent;
        children_[parent].push_back(new_tag);
        return new_tag;
    }

    node_tag_type
    parent_tag(node_tag_type node)
    {
        return parents_[node];
    }

    node_tag_type
    root_tag() const
    {
        return node_tag_type{0};
    }

    std::pair<child_tag_iterator, child_tag_iterator>
    children(node_tag_type node)
    {
        return std::make_pair(children_[node].begin(), children_[node].end());
    }

    reference operator[](node_tag_type n)
    {
        return nodes_[n];
    }

    const_reference operator[](node_tag_type n) const
    {
        return nodes_[n];
    }

private:
    handle_map<T> nodes_;
    std::vector<node_tag_type> parents_;
    std::vector<small_vector<node_tag_type>> children_;
};
} // namespace helene
