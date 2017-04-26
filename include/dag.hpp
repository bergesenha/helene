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

private:
    ////////////////////////////////////////////////////////////////////////////
    // internal classes


public:
    dag() = default;

private:
    std::vector<NodeType> node_properties_;
    std::vector<EdgeType> edge_properties_;
};
}
