#pragma once


#include <vector>
#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>


namespace helene
{

template <class PropertyType>
class persistent_iterator_;

template <class PropertyType>
class ordered_iterator_
{
public:
    typedef typename std::vector<PropertyType>::size_type size_type;
    typedef typename std::vector<size_type>::difference_type difference_type;
    typedef PropertyType value_type;
    typedef PropertyType* pointer;
    typedef PropertyType& reference;

    typedef typename std::vector<size_type>::size_type index_type;

    friend class persistent_iterator_<PropertyType>;

public:
    ordered_iterator_(index_type current_index_to_index,
                      const std::vector<size_type>& indices,
                      std::vector<PropertyType>& nodes)
        : current_index_to_index_(current_index_to_index),
          indices_(indices),
          nodes_ref_(nodes)
    {
    }

public:
    // Iterator
    reference operator*()
    {
        return nodes_ref_.get()[indices_[current_index_to_index_]];
    }

    ordered_iterator_& operator++()
    {
        ++current_index_to_index_;
        return *this;
    }

    // Equality comparable
    bool
    operator==(const ordered_iterator_& other) const
    {
        return (current_index_to_index_ == other.current_index_to_index_);
    }

    // InputIterator
    bool
    operator!=(const ordered_iterator_& other) const
    {
        return !(*this == other);
    }

    pointer operator->()
    {
        return &(nodes_ref_.get()[indices_[current_index_to_index_]]);
    }

    ordered_iterator_ operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    // BidirectionalIterator
    ordered_iterator_& operator--()
    {
        --current_index_to_index_;
        return *this;
    }

    ordered_iterator_ operator--(int)
    {
        auto temp = *this;
        --(*this);
        return temp;
    }

    // RandomAccessIterator
    ordered_iterator_&
    operator+=(difference_type n)
    {
        current_index_to_index_ += n;
        return *this;
    }

    ordered_iterator_
    operator+(difference_type n) const
    {
        return ordered_iterator_(
            current_index_to_index_ + n, indices_, nodes_ref_);
    }

    friend ordered_iterator_
    operator+(difference_type lhs, const ordered_iterator_& rhs)
    {
        return rhs + lhs;
    }

    ordered_iterator_&
    operator-=(difference_type n)
    {
        current_index_to_index_ -= n;
        return *this;
    }

    ordered_iterator_
    operator-(difference_type n) const
    {
        return ordered_iterator_(
            current_index_to_index_ - n, indices_, nodes_ref_);
    }

    difference_type
    operator-(const ordered_iterator_& other) const
    {
        return current_index_to_index_ - other.current_index_to_index_;
    }

    reference operator[](difference_type n)
    {
        return nodes_ref_.get()[indices_[current_index_to_index_ + n]];
    }

    bool
    operator<(const ordered_iterator_& other) const
    {
        return current_index_to_index_ < other.current_index_to_index_;
    }

    bool
    operator>(const ordered_iterator_& other) const
    {
        return current_index_to_index_ > other.current_index_to_index_;
    }

    bool
    operator<=(const ordered_iterator_& other) const
    {
        return current_index_to_index_ <= other.current_index_to_index_;
    }

    bool
    operator>=(const ordered_iterator_& other) const
    {
        return current_index_to_index_ >= other.current_index_to_index_;
    }


private:
    index_type current_index_to_index_;
    std::vector<size_type> indices_;
    std::reference_wrapper<std::vector<PropertyType>> nodes_ref_;
};


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

    persistent_iterator_(const ordered_iterator_<PropertyType>& other)
        : current_index_(other.indices_[other.current_index_to_index_]),
          nodes_ref_(other.nodes_ref_)
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
    size_type current_index_;
    std::reference_wrapper<std::vector<PropertyType>> nodes_ref_;
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


    // special iterator typedefs
    typedef ordered_iterator_<NodeType> order_iterator;
    typedef ordered_iterator_<EdgeType> edge_order_iterator;

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

    // marks
    enum class mark
    {
        not_removed,
        removed
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
        const auto index = edge_properties_.size();
        const auto from_index = from.current_index_;
        const auto to_index = to.current_index_;
        edge_properties_.push_back(prop);
        edges_.emplace_back(from_index, to_index);

        return edge_iterator(index, edge_properties_);
    }


    // returns pair of iterator to nodes connected by edge ed <from , to>
    std::pair<iterator, iterator>
    edge_endpoints(edge_iterator ed)
    {
        const auto edge_index = ed.current_index_;

        return std::make_pair(
            iterator(edges_[edge_index].from_property, node_properties_),
            iterator(edges_[edge_index].to_property, node_properties_));
    }


    std::pair<order_iterator, order_iterator>
    children(iterator node)
    {
        auto order = child_order(node.current_index_, edges_);

        return std::make_pair(
            order_iterator(0, order, node_properties_),
            order_iterator(order.size(), order, node_properties_));
    }

    std::pair<order_iterator, order_iterator>
    parents(iterator node)
    {
        auto order = parent_order(node.current_index_, edges_);

        return std::make_pair(
            order_iterator(0, order, node_properties_),
            order_iterator(order.size(), order, node_properties_));
    }

    std::pair<order_iterator, order_iterator>
    start_nodes()
    {
        auto order = start_nodes_order(edges_);

        return std::make_pair(
            order_iterator(0, order, node_properties_),
            order_iterator(order.size(), order, node_properties_));
    }

private:
    std::vector<size_type>
    child_order(size_type parent_index, const std::vector<edge>& edges) const
    {
        std::vector<size_type> out;

        std::for_each(
            edges.begin(), edges.end(), [&out, parent_index](const edge& ed) {
                if(ed.from_property == parent_index)
                {
                    out.push_back(ed.to_property);
                }
            });

        return out;
    }


    std::vector<size_type>
    child_order(size_type parent_index,
                const std::vector<edge>& edges,
                const std::vector<mark>& edge_marks) const
    {
        std::vector<size_type> out;

        for(typename std::vector<edge>::size_type i = 0; i < edges.size(); ++i)
        {
            if(edges[i].from_property == parent_index &&
               edge_marks[i] == mark::not_removed)
            {
                out.push_back(edges[i].to_property);
            }
        }

        return out;
    }


    std::vector<edge_size_type>
    out_edge_order(size_type parent_index, const std::vector<edge>& edges)
    {
        std::vector<edge_size_type> out;

        for(typename std::vector<edge>::size_type i = 0; i < edges.size(); ++i)
        {
            if(edges[i].from_property == parent_index)
            {
                out.push_back(i);
            }
        }

        return out;
    }


    std::vector<edge_size_type>
    out_edge_order(size_type parent_index,
                   const std::vector<edge>& edges,
                   const std::vector<mark>& edge_marks)
    {
        std::vector<edge_size_type> out;

        for(typename std::vector<edge>::size_type i = 0; i < edges.size(); ++i)
        {
            if(edges[i].from_property == parent_index &&
               edge_marks[i] == mark::not_removed)
            {
                out.push_back(i);
            }
        }

        return out;
    }


    std::vector<size_type>
    parent_order(size_type child_index, const std::vector<edge>& edges) const
    {
        std::vector<size_type> out;

        std::for_each(
            edges.begin(), edges.end(), [&out, child_index](const edge& ed) {
                if(ed.to_property == child_index)
                {
                    out.push_back(ed.from_property);
                }
            });

        return out;
    }


    std::vector<size_type>
    parent_order(size_type child_index,
                 const std::vector<edge>& edges,
                 const std::vector<mark>& edge_marks) const
    {
        std::vector<size_type> out;

        for(typename std::vector<edge>::size_type i = 0; i < edges.size(); ++i)
        {
            if(edges[i].to_property == child_index &&
               edge_marks[i] == mark::not_removed)
            {
                out.push_back(edges[i].from_property);
            }
        }


        return out;
    }


    std::vector<size_type>
    start_nodes_order(const std::vector<edge>& edges) const
    {
        std::vector<size_type> node_indices(node_properties_.size());
        std::iota(node_indices.begin(), node_indices.end(), 0);

        node_indices.erase(
            // remove index if it is found in the to_property field of any
            // edge in edges
            std::remove_if(node_indices.begin(),
                           node_indices.end(),
                           [&edges](size_type index) {
                               auto found =
                                   std::find_if(edges.begin(),
                                                edges.end(),
                                                [index](const edge& ed) {
                                                    if(ed.to_property == index)
                                                    {
                                                        return true;
                                                    }
                                                    else
                                                    {
                                                        return false;
                                                    }
                                                });

                               if(found != edges.end())
                               {
                                   return true;
                               }
                               else
                               {
                                   return false;
                               }
                           }),
            node_indices.end());

        return node_indices;
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


template <class PropertyType>
struct iterator_traits<helene::ordered_iterator_<PropertyType>>
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

