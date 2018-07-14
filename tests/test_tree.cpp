#include <catch.hpp>
#include <tree.hpp>


TEST_CASE("", "")
{
    helene::tree<char> mt('a');

    const auto root_tag = mt.root_tag();

    CHECK(mt[root_tag] == 'a');

    SECTION("insert child node of root")
    {
        const auto b = mt.insert_node('b', root_tag);

        CHECK(mt[b] == 'b');
        CHECK(mt[root_tag] == 'a');
    }
}
