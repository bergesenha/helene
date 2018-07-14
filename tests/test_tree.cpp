#include <catch.hpp>
#include <tree.hpp>


TEST_CASE("", "")
{
    helene::tree<char> mt('a');

    const auto root_tag = mt.root_tag();
    auto children1 = mt.children(root_tag);
    CHECK(std::distance(children1.first, children1.second) == 0);

    CHECK(mt[root_tag] == 'a');

    SECTION("insert child node of root")
    {
        const auto b = mt.insert_node('b', root_tag);

        auto iter_1_gen = mt.children(root_tag);

        CHECK(mt[b] == 'b');
        CHECK(mt[root_tag] == 'a');
        CHECK(std::distance(iter_1_gen.first, iter_1_gen.second) == 1);
        CHECK(*iter_1_gen.first == b);
    }
}
