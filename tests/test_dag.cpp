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
        auto it1 = one.add_node('a');

        REQUIRE(one.size() == 1);
        REQUIRE(one.edge_size() == 0);
        REQUIRE(one.begin() != one.end());
        REQUIRE(one.end() - one.begin() == 1);
        REQUIRE(one.edge_begin() == one.edge_end());
        REQUIRE(one.edge_end() - one.edge_begin() == 0);

        REQUIRE(*it1 == 'a');


        SECTION("add another node and an edge between them")
        {
            auto it2 = one.add_node('b');
            auto e_it1 = one.add_edge(it1, it2, 10);

            REQUIRE(*it1 == 'a');
            REQUIRE(*it2 == 'b');
            REQUIRE(*e_it1 == 10);

            REQUIRE(one.size() == 2);
            REQUIRE(one.edge_size() == 1);
            REQUIRE(one.edge_end() != one.edge_begin());
            REQUIRE(one.edge_end() - one.edge_begin() == 1);
        }
    }
}
