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

    SECTION("insert three children of root")
    {
        const auto b = mt.insert_node('b', root_tag);
        const auto c = mt.insert_node('c', root_tag);
        const auto d = mt.insert_node('d', root_tag);

        auto children2 = mt.children(root_tag);

        CHECK(mt[b] == 'b');
        CHECK(mt[c] == 'c');
        CHECK(mt[d] == 'd');
        CHECK(std::distance(children2.first, children2.second) == 3);
    }
}
