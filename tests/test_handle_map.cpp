#include <catch.hpp>

#include <handle_map.hpp>

TEST_CASE("", "")
{
    helene::handle_map<int> hm;


    CHECK(hm.size() == 0);

    SECTION("push back number")
    {
        const auto index_0 = hm.push_back(0);

        CHECK(hm.size() == 1);
        CHECK(hm[index_0] == 0);
    }

    SECTION("push back three numbers")
    {
        const auto index_0 = hm.push_back(0);
        const auto index_1 = hm.push_back(10);
        const auto index_2 = hm.push_back(20);

        CHECK(hm.size() == 3);
        CHECK(hm[index_0] == 0);
        CHECK(hm[index_1] == 10);
        CHECK(hm[index_2] == 20);
    }
}
