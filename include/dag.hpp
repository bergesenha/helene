#pragma once


#include <vector>
#include <algorithm>
#include <numeric>


#include "iterators.hpp"


namespace helene
{

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
        topo_cache_.push_back(index);
        return iterator(index, node_properties_);
    }


    void
    remove_node(iterator nd)
    {
        const auto index_to_remove = nd.current_index_;
        const auto index_of_last = node_properties_.size() - 1;
        std::swap(*nd, node_properties_.back());
        node_properties_.pop_back();

        // find indices of edges to remove
        std::vector<typename std::vector<edge>::size_type> indices_to_remove;

        for(auto i = 0ul; i < edges_.size(); ++i)
        {
            auto& ed = edges_[i];
            if((ed.from_property == index_to_remove) ||
               (ed.to_property == index_to_remove))
            {
                indices_to_remove.push_back(i);
            }
        }

        // erase edges associated
        std::for_each(indices_to_remove.rbegin(),
                      indices_to_remove.rend(),
                      [this](typename std::vector<edge>::size_type i) {
                          edges_.erase(edges_.begin() + i);
                          edge_properties_.erase(edge_properties_.begin() + i);
                      });

        // fix indices in edges for the node that was swapped
        std::for_each(edges_.begin(),
                      edges_.end(),
                      [index_to_remove, index_of_last](edge& ed) {
                          if(ed.from_property == index_of_last)
                          {
                              ed.from_property = index_to_remove;
                          }

                          if(ed.to_property == index_of_last)
                          {
                              ed.to_property = index_to_remove;
                          }
                      });

        // update topological order.
        topo_needs_update_ = true;
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

        if(update_topological_order())
        {
            return edge_iterator(index, edge_properties_);
        }
        else
        {
            edge_properties_.pop_back();
            edges_.pop_back();
            return edge_end();
        }
    }


    void
    remove_edge(edge_iterator ed)
    {
        const auto index = ed.current_index_;

        swap_and_pop_edge(index);
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


    // return iterator to edge between from and to if it exists, otherwise will
    // return edge_end()
    edge_iterator
    connecting_edge(iterator from, iterator to)
    {
        auto found = std::find_if(
            edges_.begin(), edges_.end(), [from, to](const edge& ed) {
                if(ed.from_property == from.current_index_ &&
                   ed.to_property == to.current_index_)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            });

        return edge_iterator(found - edges_.begin(), edge_properties_);
    }


    std::pair<order_iterator, order_iterator>
    topological_order()
    {
        if(topo_needs_update_)
        {
            update_topological_order();
        }

        return std::make_pair(
            order_iterator(0, topo_cache_, node_properties_),
            order_iterator(topo_cache_.size(), topo_cache_, node_properties_));
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

    bool
    update_topological_order()
    {

        // will eventually contain topological order
        std::vector<size_type> order;
        order.reserve(node_properties_.size());

        // initialize nodes with start nodes
        auto nodes = start_nodes_order(edges_);

        // keep track of what edges are removed
        std::vector<mark> edge_marks(edges_.size(), mark::not_removed);

        while(!nodes.empty())
        {
            order.push_back(nodes.back());
            nodes.pop_back();

            // get children
            auto child_indices = child_order(order.back(), edges_, edge_marks);

            // remove edges to children
            auto out_edges = out_edge_order(order.back(), edges_, edge_marks);
            std::for_each(out_edges.begin(),
                          out_edges.end(),
                          [&edge_marks](edge_size_type edge_index) {
                              edge_marks[edge_index] = mark::removed;
                          });

            for_each(child_indices.begin(),
                     child_indices.end(),
                     [&nodes, &edge_marks, this](size_type child_index) {

                         // check if there are other edges going into child
                         auto other_parents =
                             parent_order(child_index, edges_, edge_marks);

                         if(other_parents.empty())
                         {
                             nodes.push_back(child_index);
                         }

                     });
        }

        topo_needs_update_ = false;

        if(std::find(edge_marks.begin(), edge_marks.end(), mark::not_removed) !=
           edge_marks.end())
        {
            return false;
        }
        else
        {
            topo_cache_ = order;
            return true;
        }
    }

    void
    swap_and_pop_edge(edge_size_type idx)
    {
        std::swap(edges_[idx], edges_.back());
        std::swap(edge_properties_[idx], edge_properties_.back());
        edges_.pop_back();
        edge_properties_.pop_back();
    }

private:
    std::vector<NodeType> node_properties_;
    std::vector<EdgeType> edge_properties_;
    std::vector<edge> edges_;

    std::vector<size_type> topo_cache_;
    bool topo_needs_update_;
};
}

