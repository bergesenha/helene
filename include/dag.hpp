#pragma once


#include <vector>
#include <iterator>
#include <functional>


namespace helene
{

template <class NodeType, class EdgeType>
class dag;

template <class NodeType, class EdgeType>
class persistent_iterator_
{
public:
    typedef typename dag<NodeType, EdgeType>::size_type size_type;

    typedef typename std::vector<NodeType>::difference_type difference_type;
    typedef NodeType value_type;
    typedef NodeType* pointer;
    typedef NodeType& reference;

public:
    persistent_iterator_(size_type current_index, std::vector<NodeType>& nodes)
        : current_index_(current_index), nodes_ref_(nodes)
    {
    }

public:
    // CopyConstructible
    persistent_iterator_(const persistent_iterator_& other)
        : current_index_(other.current_index_), nodes_ref_(other.nodes_ref_)
    {
    }

    // Iterator concept member functions
    reference operator*()
    {
        return nodes_ref_.get()[current_index_];
    }

    persistent_iterator_& operator++()
    {
        ++current_index_;
        return *this;
    }

private:
    std::reference_wrapper<std::vector<NodeType>> nodes_ref_;
    size_type current_index_;
};


class start_node_iterator_
{
};

template <class NodeType, class EdgeType>
class dag
{
public:
    ////////////////////////////////////////////////////////////////////////////
    // public typedefs

    // node typedefs
    typedef NodeType value_type;
    typedef NodeType& reference;
    typedef const NodeType& const_reference;

    typedef typename std::vector<NodeType>::difference_type difference_type;
    typedef typename std::vector<NodeType>::size_type size_type;

    typedef typename std::vector<NodeType>::iterator iterator;
    typedef typename std::vector<NodeType>::const_iterator const_iterator;


    // edge typedefs
    typedef EdgeType edge_value_type;
    typedef EdgeType& edge_reference;
    typedef const EdgeType& const_edge_reference;

    typedef typename std::vector<EdgeType>::size_type edge_size_type;
    typedef
        typename std::vector<EdgeType>::difference_type edge_difference_type;

    typedef typename std::vector<EdgeType>::iterator edge_iterator;
    typedef typename std::vector<EdgeType>::const_iterator edge_const_iterator;


    // iterator typedefs
    typedef start_node_iterator_ start_node_iterator;
    typedef const start_node_iterator const_start_node_iterator;

private:
    ////////////////////////////////////////////////////////////////////////////
    // internal classes

    // indices to properties of the associated edge
    struct edge
    {
        edge(size_type from, size_type to)
            : from_property(from), to_property(to)
        {
        }

        size_type from_property;
        size_type to_property;

        bool
        operator==(const edge& other) const
        {
            return (from_property == other.from_property) &&
                   (to_property == other.to_property);
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


    void
    swap(dag& other)
    {
        node_properties_.swap(other.node_properties_);
        edge_properties_.swap(other.edge_properties_);
        edges_.swap(other.edges_);
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
        edges_.emplace_back(from - cbegin(), to - cbegin());
        edge_properties_.push_back(prop);
        return std::prev(edge_properties_.end());
    }


    std::pair<iterator, iterator>
    end_nodes(edge_const_iterator edge)
    {
        // index of edge property
        auto edge_property_index = edge - edge_properties_.begin();

        // find
    }

private:
    std::vector<NodeType> node_properties_;
    std::vector<EdgeType> edge_properties_;

    std::vector<edge> edges_;
};
}
