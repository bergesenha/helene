#include "catch.hpp"

#include <algorithm>

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
        REQUIRE(a.begin() != a.end());
        REQUIRE(a.end() - a.begin() == 1);

        REQUIRE(a.edge_begin() == a.edge_end());
        REQUIRE(a.edge_cbegin() == a.edge_cend());
        REQUIRE(a.edge_end() - a.edge_begin() == 0);

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
            REQUIRE(edge_it_10 == a.edge_begin());
            REQUIRE(a.edge_end() - edge_it_10 == 1);
            REQUIRE(a.edge_begin() != a.edge_end());

            SECTION("attempt to introduce edge causing cycle")
            {
                auto edge_it_no = a.add_edge(node_it_a, node_it_b, 100);

                REQUIRE(edge_it_no == a.edge_end());
            }

            SECTION("copy construct another dag")
            {
                helene::dag<char, int> mydag2 = a;

                REQUIRE(*a.begin() == *mydag2.begin());
                REQUIRE(std::equal(a.begin(), a.end(), mydag2.begin()));
                REQUIRE(std::equal(
                    a.edge_begin(), a.edge_end(), mydag2.edge_begin()));

                REQUIRE(a == mydag2);
            }

            SECTION("copy assign another dag")
            {
                helene::dag<char, int> mydag2;
                mydag2 = a;

                REQUIRE(*a.begin() == *mydag2.begin());
                REQUIRE(std::equal(a.begin(), a.end(), mydag2.begin()));
                REQUIRE(std::equal(
                    a.edge_begin(), a.edge_end(), mydag2.edge_begin()));

                REQUIRE(a == mydag2);
            }

            SECTION("swap with another dag")
            {
                helene::dag<char, int> mydag2;
                mydag2.add_node('c');

                a.swap(mydag2);

                REQUIRE(mydag2 != a);
                REQUIRE(*mydag2.begin() == 'a');
                REQUIRE(*a.begin() == 'c');
                REQUIRE(*mydag2.edge_begin() == 10);
            }
        }
    }
}
