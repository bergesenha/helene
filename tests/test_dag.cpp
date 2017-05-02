#include "catch.hpp"

#include <algorithm>

#include <dag.hpp>


TEST_CASE("test dag", "[dag<char, int>]")
{
    typedef helene::dag<char, int> dag_type;

    dag_type one;

    REQUIRE(one.size() == 0);
    REQUIRE(one.edge_size() == 0);
    REQUIRE(one.begin() == one.end());
    REQUIRE(one.end() - one.begin() == 0);
    REQUIRE(one.edge_begin() == one.edge_end());
    REQUIRE(one.edge_end() - one.edge_begin() == 0);

    SECTION("add a node")
    {
        auto it1 = one.insert('a');

        REQUIRE(one.size() == 1);
        REQUIRE(one.edge_size() == 0);
        REQUIRE(one.begin() != one.end());
        REQUIRE(one.end() - one.begin() == 1);
        REQUIRE(one.edge_begin() == one.edge_end());
        REQUIRE(one.edge_end() - one.edge_begin() == 0);

        REQUIRE(*it1 == 'a');


        SECTION("attempt to get children of first node")
        {
            auto child_pair = one.children(it1);

            REQUIRE(child_pair.first == child_pair.second);
        }

        SECTION("attempt to get parents of first node")
        {
            auto parent_pair = one.parents(it1);

            REQUIRE(parent_pair.first == parent_pair.second);
        }


        SECTION("add another node and an edge between them")
        {
            auto it2 = one.insert('b');
            auto e_it1 = one.add_edge(it1, it2, 10);

            REQUIRE(*it1 == 'a');
            REQUIRE(*it2 == 'b');
            REQUIRE(*e_it1 == 10);

            REQUIRE(one.size() == 2);
            REQUIRE(one.edge_size() == 1);
            REQUIRE(one.edge_end() != one.edge_begin());
            REQUIRE(one.edge_end() - one.edge_begin() == 1);

            REQUIRE(*one.edge_endpoints(e_it1).first == 'a');
            REQUIRE(*one.edge_endpoints(e_it1).second == 'b');

            REQUIRE(one.connecting_edge(it1, it2) != one.edge_end());
            REQUIRE(*one.connecting_edge(it1, it2) == 10);

            REQUIRE(one.connecting_edge(it2, it1) == one.edge_end());

            SECTION("get children of first node")
            {
                auto child_pair = one.children(it1);

                REQUIRE(*child_pair.first == 'b');
                REQUIRE(child_pair.second - child_pair.first == 1);
            }

            SECTION("get parents of second node")
            {
                auto parent_pair = one.parents(it2);

                REQUIRE(*parent_pair.first == 'a');
                REQUIRE(parent_pair.second - parent_pair.first == 1);
            }

            SECTION("attempt to add a cyclic edge")
            {
                auto it_no = one.add_edge(it2, it1, 1000);

                REQUIRE(it_no == one.edge_end());
                REQUIRE(one.edge_size() == 1);
            }

            SECTION("add another node and an edge from first")
            {
                auto it3 = one.insert('c');
                auto e_it2 = one.add_edge(it1, it3, 20);

                REQUIRE(one.size() == 3);
                REQUIRE(one.edge_size() == 2);

                SECTION("get children of first node")
                {
                    auto child_pair = one.children(it1);

                    REQUIRE(child_pair.second - child_pair.first == 2);
                    REQUIRE(std::find(child_pair.first,
                                      child_pair.second,
                                      'b') != child_pair.second);
                    REQUIRE(std::find(child_pair.first,
                                      child_pair.second,
                                      'c') != child_pair.second);
                    REQUIRE(std::find(child_pair.first,
                                      child_pair.second,
                                      'a') == child_pair.second);
                }

                SECTION("get parents of third node")
                {
                    auto parent_pair = one.parents(it3);

                    REQUIRE(*parent_pair.first == 'a');
                    REQUIRE(parent_pair.second - parent_pair.first == 1);
                }

                SECTION("add another node with edge from second and third")
                {
                    auto it4 = one.insert('d');
                    auto e_it3 = one.add_edge(it2, it4, 30);
                    auto e_it4 = one.add_edge(it3, it4, 40);

                    REQUIRE(one.size() == 4);
                    REQUIRE(one.edge_size() == 4);

                    SECTION("get parents of fourth node")
                    {
                        auto parent_pair = one.parents(it4);

                        REQUIRE(parent_pair.second - parent_pair.first == 2);
                        REQUIRE(std::find(parent_pair.first,
                                          parent_pair.second,
                                          'b') != parent_pair.second);
                        REQUIRE(std::find(parent_pair.first,
                                          parent_pair.second,
                                          'c') != parent_pair.second);
                        REQUIRE(std::find(parent_pair.first,
                                          parent_pair.second,
                                          'a') == parent_pair.second);
                        REQUIRE(std::find(parent_pair.first,
                                          parent_pair.second,
                                          'd') == parent_pair.second);
                    }

                    SECTION("get start nodes")
                    {
                        auto start_pair = one.start_nodes();

                        REQUIRE(*start_pair.first == 'a');
                        REQUIRE(start_pair.second - start_pair.first == 1);
                    }


                    SECTION("add a node with no connections to it")
                    {
                        auto it5 = one.insert('e');

                        REQUIRE(one.size() == 5);
                        REQUIRE(one.edge_size() == 4);

                        SECTION("get start nodes")
                        {
                            auto start_pair = one.start_nodes();

                            REQUIRE(start_pair.second - start_pair.first == 2);
                            REQUIRE(std::find(start_pair.first,
                                              start_pair.second,
                                              'a') != start_pair.second);
                            REQUIRE(std::find(start_pair.first,
                                              start_pair.second,
                                              'e') != start_pair.second);
                            REQUIRE(std::find(start_pair.first,
                                              start_pair.second,
                                              'b') == start_pair.second);
                        }

                        SECTION("add edge from fifth node to first")
                        {
                            auto e_it5 = one.add_edge(it5, it1, 50);

                            SECTION("get topological_order")
                            {
                                auto topo_pair = one.topological_order();

                                REQUIRE(*topo_pair.first == 'e');
                                REQUIRE(*std::next(topo_pair.first) == 'a');
                                REQUIRE(*std::prev(topo_pair.second) == 'd');
                            }
                        }
                    }

                    SECTION("get topological order")
                    {
                        auto topo_pair = one.topological_order();

                        REQUIRE(*topo_pair.first == 'a');
                        REQUIRE(*std::prev(topo_pair.second) == 'd');
                        REQUIRE(std::find(std::next(topo_pair.first),
                                          std::prev(topo_pair.second),
                                          'b') != std::prev(topo_pair.second));
                        REQUIRE(std::find(std::next(topo_pair.first),
                                          std::prev(topo_pair.second),
                                          'c') != std::prev(topo_pair.second));
                    }

                    SECTION("remove edge")
                    {
                        one.remove_edge(e_it3);

                        REQUIRE(*e_it1 == 10);
                        REQUIRE(*e_it2 == 20);
                        REQUIRE(one.edge_size() == 3);

                        SECTION("get topological_order")
                        {
                            auto topo_pair = one.topological_order();

                            REQUIRE(topo_pair.second - topo_pair.first == 4);
                            REQUIRE(*topo_pair.first == 'a');
                            REQUIRE(*(topo_pair.first + 1) != 'd');
                        }
                    }

                    SECTION("remove first node")
                    {
                        one.remove_node(it1);

                        REQUIRE(one.size() == 3);
                        REQUIRE(one.edge_size() == 2);

                        REQUIRE(std::find(one.begin(), one.end(), 'b') !=
                                one.end());
                        REQUIRE(std::find(one.begin(), one.end(), 'c') !=
                                one.end());
                        REQUIRE(std::find(one.begin(), one.end(), 'd') !=
                                one.end());

                        REQUIRE(std::find(one.begin(), one.end(), 'a') ==
                                one.end());

                        SECTION("get topological_order")
                        {
                            auto topo_pair = one.topological_order();

                            REQUIRE(topo_pair.second - topo_pair.first == 3);
                            REQUIRE(*(std::prev(topo_pair.second)) == 'd');
                            REQUIRE(std::find(topo_pair.first,
                                              std::prev(topo_pair.second),
                                              'b') !=
                                    std::prev(topo_pair.second));
                            REQUIRE(std::find(topo_pair.first,
                                              std::prev(topo_pair.second),
                                              'c') !=
                                    std::prev(topo_pair.second));
                            REQUIRE(std::find(topo_pair.first,
                                              std::prev(topo_pair.second),
                                              'd') ==
                                    std::prev(topo_pair.second));
                        }
                    }

                    SECTION("remove second node")
                    {
                        one.remove_node(it2);

                        REQUIRE(one.size() == 3);
                        REQUIRE(one.edge_size() == 2);

                        REQUIRE(std::find(one.begin(), one.end(), 'a') !=
                                one.end());
                        REQUIRE(std::find(one.begin(), one.end(), 'c') !=
                                one.end());
                        REQUIRE(std::find(one.begin(), one.end(), 'd') !=
                                one.end());

                        REQUIRE(std::find(one.begin(), one.end(), 'b') ==
                                one.end());

                        SECTION("get topological_order")
                        {
                            auto topo_pair = one.topological_order();

                            REQUIRE(topo_pair.second - topo_pair.first == 3);
                            REQUIRE(*topo_pair.first == 'a');
                            REQUIRE(*(std::next(topo_pair.first)) == 'c');
                            REQUIRE(*(std::prev(topo_pair.second)) == 'd');
                        }
                    }

                    SECTION("remove third node")
                    {
                        one.remove_node(it3);

                        REQUIRE(one.size() == 3);
                        REQUIRE(one.edge_size() == 2);

                        REQUIRE(std::find(one.begin(), one.end(), 'a') !=
                                one.end());
                        REQUIRE(std::find(one.begin(), one.end(), 'b') !=
                                one.end());
                        REQUIRE(std::find(one.begin(), one.end(), 'd') !=
                                one.end());

                        REQUIRE(std::find(one.begin(), one.end(), 'c') ==
                                one.end());

                        SECTION("get topological_order")
                        {
                            auto topo_pair = one.topological_order();

                            REQUIRE(topo_pair.second - topo_pair.first == 3);
                            REQUIRE(*topo_pair.first == 'a');
                            REQUIRE(*(std::next(topo_pair.first)) == 'b');
                            REQUIRE(*(std::prev(topo_pair.second)) == 'd');
                        }
                    }

                    SECTION("remove fourth node")
                    {
                        one.remove_node(it4);

                        REQUIRE(one.size() == 3);
                        REQUIRE(one.edge_size() == 2);

                        REQUIRE(std::find(one.begin(), one.end(), 'a') !=
                                one.end());
                        REQUIRE(std::find(one.begin(), one.end(), 'b') !=
                                one.end());
                        REQUIRE(std::find(one.begin(), one.end(), 'c') !=
                                one.end());

                        REQUIRE(std::find(one.begin(), one.end(), 'd') ==
                                one.end());

                        SECTION("get topological_order")
                        {
                            auto topo_pair = one.topological_order();

                            REQUIRE(topo_pair.second - topo_pair.first == 3);
                            REQUIRE(*topo_pair.first == 'a');
                            REQUIRE(std::find(std::next(topo_pair.first),
                                              topo_pair.second,
                                              'b') != topo_pair.second);
                            REQUIRE(std::find(std::next(topo_pair.first),
                                              topo_pair.second,
                                              'c') != topo_pair.second);
                        }
                    }
                }

                SECTION("get topological order")
                {
                    auto topo_pair = one.topological_order();

                    REQUIRE(*topo_pair.first == 'a');
                    REQUIRE(std::find(topo_pair.first + 1,
                                      topo_pair.second,
                                      'b') != topo_pair.second);
                    REQUIRE(std::find(topo_pair.first + 1,
                                      topo_pair.second,
                                      'c') != topo_pair.second);
                    REQUIRE(topo_pair.second - topo_pair.first == 3);
                }

                SECTION("remove first edge")
                {
                    one.remove_edge(e_it1);

                    REQUIRE(one.edge_size() == 1);
                    REQUIRE(*one.edge_begin() == 20);

                    SECTION("get topological order")
                    {
                        auto topo_pair = one.topological_order();

                        REQUIRE(topo_pair.second - topo_pair.first == 3);
                        REQUIRE(*topo_pair.first != 'c');
                    }
                }

                SECTION("remove first node")
                {
                    one.remove_node(it1);

                    REQUIRE(one.size() == 2);

                    REQUIRE(std::find(one.begin(), one.end(), 'b') !=
                            one.end());
                    REQUIRE(std::find(one.begin(), one.end(), 'c') !=
                            one.end());
                    REQUIRE(std::find(one.begin(), one.end(), 'a') ==
                            one.end());

                    REQUIRE(one.edge_size() == 0);
                }

                SECTION("remove second node")
                {
                    one.remove_node(it2);

                    REQUIRE(one.size() == 2);

                    REQUIRE(std::find(one.begin(), one.end(), 'a') !=
                            one.end());
                    REQUIRE(std::find(one.begin(), one.end(), 'c') !=
                            one.end());
                    REQUIRE(std::find(one.begin(), one.end(), 'b') ==
                            one.end());

                    REQUIRE(one.edge_size() == 1);

                    SECTION("get topological order")
                    {
                        auto topo_pair = one.topological_order();

                        REQUIRE(topo_pair.second - topo_pair.first == 2);

                        REQUIRE(*topo_pair.first == 'a');
                        REQUIRE(*(std::next(topo_pair.first)) == 'c');
                    }
                }

                SECTION("remove third node")
                {
                    one.remove_node(it3);

                    REQUIRE(one.size() == 2);

                    REQUIRE(std::find(one.begin(), one.end(), 'a') !=
                            one.end());
                    REQUIRE(std::find(one.begin(), one.end(), 'b') !=
                            one.end());
                    REQUIRE(std::find(one.begin(), one.end(), 'c') ==
                            one.end());

                    REQUIRE(one.edge_size() == 1);

                    SECTION("get topological order")
                    {
                        auto topo_pair = one.topological_order();

                        REQUIRE(topo_pair.second - topo_pair.first == 2);

                        REQUIRE(*topo_pair.first == 'a');
                        REQUIRE(*(std::next(topo_pair.first)) == 'b');
                    }
                }
            }

            SECTION("get topological order")
            {
                auto topo_pair = one.topological_order();

                REQUIRE(*topo_pair.first == 'a');
                REQUIRE(*(topo_pair.first + 1) == 'b');
                REQUIRE(topo_pair.second - topo_pair.first == 2);
            }

            SECTION("remove edge")
            {
                one.remove_edge(e_it1);

                REQUIRE(one.size() == 2);
                REQUIRE(one.edge_size() == 0);
                REQUIRE(one.edge_begin() == one.edge_end());
            }

            SECTION("remove first node")
            {
                one.remove_node(it1);

                REQUIRE(one.size() == 1);
                REQUIRE(one.edge_size() == 0);
                REQUIRE(*one.begin() == 'b');
            }

            SECTION("remove second node")
            {
                one.remove_node(it2);

                REQUIRE(one.size() == 1);
                REQUIRE(one.edge_size() == 0);
                REQUIRE(*one.begin() == 'a');
            }
        }

        SECTION("attempt to add edge from a node to itself")
        {
            auto it_no = one.add_edge(it1, it1, 2000);

            REQUIRE(it_no == one.edge_end());
            REQUIRE(one.edge_size() == 0);
        }

        SECTION("get start nodes")
        {
            auto start_pair = one.start_nodes();

            REQUIRE(*start_pair.first == 'a');
            REQUIRE(start_pair.second - start_pair.first == 1);
        }

        SECTION("get topological order")
        {
            auto topo_pair = one.topological_order();

            REQUIRE(*topo_pair.first == 'a');
            REQUIRE(topo_pair.second - topo_pair.first == 1);
        }

        SECTION("remove the node")
        {
            one.remove_node(it1);

            REQUIRE(one.size() == 0);
            REQUIRE(one.begin() == one.end());
        }
    }

    SECTION("attempt to get start nodes of empty dag")
    {
        auto start_pair = one.start_nodes();

        REQUIRE(start_pair.first == start_pair.second);
    }
}
