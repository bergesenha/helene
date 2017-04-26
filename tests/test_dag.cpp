#include "catch.hpp"


#include <dag.hpp>


TEST_CASE("dag with node type char and edge type int", "[dag<char, int>]")
{
    helene::dag<char, int> a;


    SECTION("adding a node")
    {
        auto it = a.add_node('a');

        REQUIRE(*it == 'a');
        REQUIRE(*a.begin() == 'a');
        REQUIRE(*a.cbegin() == 'a');
        REQUIRE(a.begin() == it);
        REQUIRE(a.cbegin() == it);

        SECTION("incrementing iterator")
        {
            ++it;

            REQUIRE(it == a.end());
            REQUIRE(it == a.cend());
        }
    }
}
