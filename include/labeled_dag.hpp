#pragma once


#include <vector>
#include <utility>


namespace helene
{


template <class LabelType, class NodeType>
class labeled_dag
{
    ////////////////////////////////////////////////////////////////////////////
    // internal classes
    struct node_impl_
    {
        LabelType label;
        NodeType property;
    };


    struct edge_impl_
    {
        edge_impl_(const LabelType& from, const LabelType& to)
            : from_(from), to_(to)
        {
        }

        LabelType from_;
        LabelType to_;
    };

public:
    ////////////////////////////////////////////////////////////////////////////
    // public typedefs


public:
private:
    std::vector<node_impl_> nodes_;
    std::vector<edge_impl_> edges_;
};
}
