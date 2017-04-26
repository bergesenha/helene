#pragma once


#include <vector>
#include <iterator>


namespace helene
{


template <class NodeType, class EdgeType>
class dag
{
public:
    ////////////////////////////////////////////////////////////////////////////
    // public typedefs

    typedef NodeType value_type;
    typedef NodeType& reference;
    typedef const NodeType& const_reference;

    typedef typename std::vector<NodeType>::difference_type difference_type;
    typedef typename std::vector<NodeType>::size_type size_type;

    typedef typename std::vector<NodeType>::iterator iterator;


    typedef EdgeType edge_value_type;
    typedef EdgeType& edge_reference;
    typedef const EdgeType& const_edge_reference;

    typedef typename std::vector<EdgeType>::size_type edge_size_type;
    typedef
        typename std::vector<EdgeType>::difference_type edge_difference_type;

private:
    ////////////////////////////////////////////////////////////////////////////
    // internal classes

    // indices to properties of the associated edge
    struct edge
    {
        size_type from_property;
        size_type to_property;
        edge_size_type edge_property;
    };

public:
    ////////////////////////////////////////////////////////////////////////////
    // Container concept member functions

    dag() = default;


public:
    ////////////////////////////////////////////////////////////////////////////
    // dag specific member functions

    iterator
    add_node(const NodeType& prop)
    {
        node_properties_.push_back(prop);
        return std::prev(node_properties_.end());
    }

private:
    std::vector<NodeType> node_properties_;
    std::vector<EdgeType> edge_properties_;

    std::vector<edge> edges_;
};
}
