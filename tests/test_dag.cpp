#include "catch.hpp"


#include <dag.hpp>


TEST_CASE("dag with node type char and edge type int", "[dag<char, int>]")
{
    helene::dag<char, int> a;


    SECTION("adding a node")
    {
        auto it = a.add_node('a');

        REQUIRE(*it == 'a');
    }
}
