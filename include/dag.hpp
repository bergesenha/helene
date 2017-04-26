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
    typedef typename std::vector<NodeType>::const_iterator const_iterator;


    typedef EdgeType edge_value_type;
    typedef EdgeType& edge_reference;
    typedef const EdgeType& const_edge_reference;

    typedef typename std::vector<EdgeType>::size_type edge_size_type;
    typedef
        typename std::vector<EdgeType>::difference_type edge_difference_type;

    typedef typename std::vector<EdgeType>::iterator edge_iterator;
    typedef typename std::vector<EdgeType>::const_iterator edge_const_iterator;

private:
    ////////////////////////////////////////////////////////////////////////////
    // internal classes

    // indices to properties of the associated edge
    struct edge
    {
        edge(size_type from, size_type to, edge_size_type edge_index)
            : from_property(from), to_property(to), edge_property(edge_index)
        {
        }

        size_type from_property;
        size_type to_property;
        edge_size_type edge_property;

        bool
        operator==(const edge& other) const
        {
            return (from_property == other.from_property) &&
                   (to_property == other.to_property) &&
                   (edge_property == other.edge_property);
        }
    };

public:
    ////////////////////////////////////////////////////////////////////////////
    // Container concept member functions

    dag() = default;
    dag(const dag&) = default;

    dag& operator=(const dag&) = default;

    iterator
    begin()
    {
        return node_properties_.begin();
    }

    iterator
    end()
    {
        return node_properties_.end();
    }

    const_iterator
    cbegin() const
    {
        return node_properties_.cbegin();
    }

    const_iterator
    cend() const
    {
        return node_properties_.cend();
    }

    edge_iterator
    edge_begin()
    {
        return edge_properties_.begin();
    }

    edge_iterator
    edge_end()
    {
        return edge_properties_.end();
    }

    edge_const_iterator
    edge_cbegin() const
    {
        return edge_properties_.cbegin();
    }

    edge_const_iterator
    edge_cend() const
    {
        return edge_properties_.cend();
    }


    bool
    operator==(const dag& other) const
    {
        return (node_properties_ == other.node_properties_) &&
               (edge_properties_ == other.edge_properties_) &&
               (edges_ == other.edges_);
    }


    bool
    operator!=(const dag& other) const
    {
        return !(*this == other);
    }

public:
    ////////////////////////////////////////////////////////////////////////////
    // dag specific member functions


    // Add node
    iterator
    add_node(const NodeType& prop)
    {
        node_properties_.push_back(prop);
        return std::prev(node_properties_.end());
    }


    // Add edge between nodes.
    // If edge introduces cycle edge will not be inserted.
    // Returns iterator to edge property or edge_end() if edge was prevented
    // from being inserted.
    edge_iterator
    add_edge(const_iterator from, const_iterator to, const EdgeType& prop)
    {
        // TODO: check for cycle
        edges_.emplace_back(from - cbegin(), to - cbegin(), edges_.size());
        edge_properties_.push_back(prop);
        return std::prev(edge_properties_.end());
    }

private:
    std::vector<NodeType> node_properties_;
    std::vector<EdgeType> edge_properties_;

    std::vector<edge> edges_;
};
}
