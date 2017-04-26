#pragma once


#include <vector>


namespace helene
{
template <class NodeType, class EdgeType> class dag
{
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
