#include "catch.hpp"


#include <dag.hpp>


TEST_CASE("dag with node type char and edge type int", "[dag<char, int>]")
{
    helene::dag<char, int> a;


    SECTION("adding a node")
    {
        auto node_it_a = a.add_node('a');

        REQUIRE(*node_it_a == 'a');
        REQUIRE(*a.begin() == 'a');
        REQUIRE(*a.cbegin() == 'a');
        REQUIRE(a.begin() == node_it_a);
        REQUIRE(a.cbegin() == node_it_a);

        SECTION("incrementing iterator")
        {
            ++node_it_a;

            REQUIRE(node_it_a == a.end());
            REQUIRE(node_it_a == a.cend());
        }

        SECTION("add another node and an edge from first to second")
        {
            auto node_it_b = a.add_node('b');
            auto edge_it_10 = a.add_edge(node_it_a, node_it_b, 10);

            REQUIRE(*edge_it_10 == 10);
        }
    }
}
