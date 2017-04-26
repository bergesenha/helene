#pragma once


#include <vector>


namespace helene
{
template <class NodeType, class EdgeType> class dag
{
public:
    ////////////////////////////////////////////////////////////////////////////
    // public typedefs

    typedef NodeType value_type;
    typedef NodeType& reference;
    typedef const NodeType& const_reference;

    typedef typename std::vector<NodeType>::difference_type difference_type;
    typedef typename std::vector<NodeType>::size_type size_type;


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
    dag() = default;

private:
    std::vector<NodeType> node_properties_;
    std::vector<EdgeType> edge_properties_;
};
}
