#include <catch.hpp>

#include <handle_map.hpp>

TEST_CASE("", "")
{
    helene::handle_map<int> hm;


    CHECK(hm.size() == 0);

    SECTION("push back number")
    {
        const auto index_0 = hm.insert(0);

        CHECK(hm.size() == 1);
        CHECK(hm[index_0] == 0);
    }

    SECTION("push back three numbers")
    {
        const auto index_0 = hm.insert(0);
        const auto index_1 = hm.insert(10);
        const auto index_2 = hm.insert(20);

        CHECK(hm.size() == 3);
        CHECK(hm[index_0] == 0);
        CHECK(hm[index_1] == 10);
        CHECK(hm[index_2] == 20);

        SECTION("remove first element")
        {
            hm.erase(index_0);

            CHECK(hm.size() == 2);

            SECTION("insert another element")
            {
                const auto index_3 = hm.insert(30);

                CHECK(hm.size() == 3);
                CHECK(hm[index_1] == 10);
                CHECK(hm[index_2] == 20);
                CHECK(hm[index_3] == 30);
            }
        }

        SECTION("remove second element")
        {
            hm.erase(index_1);

            CHECK(hm.size() == 2);

            SECTION("insert another element")
            {
                const auto index_3 = hm.insert(30);

                CHECK(hm.size() == 3);
                CHECK(hm[index_0] == 0);
                CHECK(hm[index_2] == 20);
                CHECK(hm[index_3] == 30);
            }
        }

        SECTION("remove third element")
        {
            hm.erase(index_2);

            CHECK(hm.size() == 2);

            SECTION("insert another element")
            {
                const auto index_3 = hm.insert(30);

                CHECK(hm.size() == 3);
                CHECK(hm[index_0] == 0);
                CHECK(hm[index_1] == 10);
                CHECK(hm[index_3] == 30);
            }
        }
    }
}
