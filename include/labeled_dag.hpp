#pragma once


#include <vector>
#include <utility>


namespace helene
{


template <class MemberType,
          class StructType,
          template <class...> class Container,
          MemberType StructType::*Ptr>
class member_iterator
{
private:
    typedef typename Container<StructType>::iterator base_iterator;

public:
private:
    base_iterator current_;
};

template <class MemberType,
          class StructType,
          template <class...> class Container,
          MemberType StructType::*Ptr>
class const_member_iterator
{
private:
    typedef typename Container<StructType>::const_iterator base_iterator;

public:
private:
    base_iterator current_;
};


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
    typedef member_iterator<NodeType,
                            node_impl_,
                            std::vector,
                            &node_impl_::property>
        iterator;

    typedef const_member_iterator<NodeType,
                                  node_impl_,
                                  std::vector,
                                  &node_impl_::property>
        const_iterator;

public:
private:
    std::vector<node_impl_> nodes_;
    std::vector<edge_impl_> edges_;
};
}
