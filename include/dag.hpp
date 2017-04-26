#pragma once


#include <vector>
#include <iterator>
#include <functional>


namespace helene
{


template <class PropertyType>
class persistent_iterator_
{
    template <class NodeType, class EdgeType>
    friend class dag;

public:
    typedef typename std::vector<PropertyType>::size_type size_type;

    typedef typename std::vector<PropertyType>::difference_type difference_type;
    typedef PropertyType value_type;
    typedef PropertyType* pointer;
    typedef PropertyType& reference;

public:
    persistent_iterator_(size_type current_index,
                         std::vector<PropertyType>& nodes)
        : current_index_(current_index), nodes_ref_(nodes)
    {
    }

public:
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

    // EqualityComparable
    bool
    operator==(const persistent_iterator_& other) const
    {
        return (current_index_ == other.current_index_) &&
               (&(nodes_ref_.get()) == &(other.nodes_ref_.get()));
    }

    // InputIterator
    bool
    operator!=(const persistent_iterator_& other) const
    {
        return !(*this == other);
    }

    pointer operator->()
    {
        return &(nodes_ref_.get()[current_index_]);
    }

    persistent_iterator_ operator++(int)
    {
        auto temp = *this;
        ++(*this);

        return temp;
    }

    // BidirectionalIterator
    persistent_iterator_& operator--()
    {
        --current_index_;
        return *this;
    }

    persistent_iterator_ operator--(int)
    {
        auto temp = *this;
        --(*this);
        return temp;
    }

    // RandomAccessIterator
    persistent_iterator_&
    operator+=(difference_type n)
    {
        current_index_ += n;
        return *this;
    }

    persistent_iterator_
    operator+(difference_type n) const
    {
        return persistent_iterator_(current_index_ + n, nodes_ref_);
    }

    friend persistent_iterator_
    operator+(difference_type lhs, const persistent_iterator_& rhs)
    {
        return rhs + lhs;
    }

    persistent_iterator_&
    operator-=(difference_type n)
    {
        current_index_ -= n;
        return *this;
    }

    persistent_iterator_
    operator-(difference_type n) const
    {
        return persistent_iterator_(current_index_ - n, nodes_ref_);
    }

    difference_type
    operator-(const persistent_iterator_& other) const
    {
        return current_index_ - other.current_index_;
    }

    reference operator[](difference_type n)
    {
        return nodes_ref_.get()[current_index_ + n];
    }

    bool
    operator<(const persistent_iterator_& other) const
    {
        return current_index_ < other.current_index_;
    }

    bool
    operator>(const persistent_iterator_& other) const
    {
        return current_index_ > other.current_index_;
    }

    bool
    operator<=(const persistent_iterator_& other) const
    {
        return current_index_ <= other.current_index_;
    }

    bool
    operator>=(const persistent_iterator_& other) const
    {
        return current_index_ >= other.current_index_;
    }

private:
    std::reference_wrapper<std::vector<PropertyType>> nodes_ref_;
    size_type current_index_;
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

    typedef persistent_iterator_<NodeType> iterator;


    // edge typedefs
    typedef EdgeType edge_value_type;
    typedef EdgeType& edge_reference;
    typedef const EdgeType& const_edge_reference;
    typedef typename std::vector<EdgeType>::size_type edge_size_type;
    typedef
        typename std::vector<EdgeType>::difference_type edge_difference_type;

    typedef persistent_iterator_<EdgeType> edge_iterator;


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
        return iterator(0, node_properties_);
    }

    iterator
    end()
    {
        return iterator(node_properties_.size(), node_properties_);
    }


    edge_iterator
    edge_begin()
    {
        return edge_iterator(0, edge_properties_);
    }

    edge_iterator
    edge_end()
    {
        return edge_iterator(edge_properties_.size(), edge_properties_);
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

    size_type
    size() const
    {
        return node_properties_.size();
    }

    edge_size_type
    edge_size() const
    {
        return edge_properties_.size();
    }

    bool
    empty() const
    {
        return node_properties_.empty();
    }

    bool
    edge_empty() const
    {
        return edge_properties_.empty();
    }

public:
    ////////////////////////////////////////////////////////////////////////////
    // dag specific member functions


    // Add node
    iterator
    add_node(const NodeType& prop)
    {
        const auto index = node_properties_.size();
        node_properties_.push_back(prop);
        return iterator(index, node_properties_);
    }


    // Add edge between nodes.
    // If edge introduces cycle edge will not be inserted.
    // Returns iterator to edge property or edge_end() if edge was prevented
    // from being inserted.
    edge_iterator
    add_edge(iterator from, iterator to, const EdgeType& prop)
    {
        const auto from_index = from.current_index_;
        const auto to_index = to.current_index_;
        edge_properties_.push_back(prop);
        edges_.emplace_back(from_index, to_index);
    }


private:
    std::vector<NodeType> node_properties_;
    std::vector<EdgeType> edge_properties_;

    std::vector<edge> edges_;
};
}


namespace std
{
template <class PropertyType>
struct iterator_traits<helene::persistent_iterator_<PropertyType>>
{
    typedef typename helene::persistent_iterator_<PropertyType>::difference_type
        difference_type;

    typedef typename helene::persistent_iterator_<PropertyType>::value_type
        value_type;

    typedef
        typename helene::persistent_iterator_<PropertyType>::pointer pointer;

    typedef typename helene::persistent_iterator_<PropertyType>::reference
        reference;

    typedef random_access_iterator_tag iterator_category;
};
}

