#include <catch.hpp>

#include <stable_container.hpp>


TEST_CASE("", "")
{
    helene::stable_vector<int> sv;
    const auto first_index = sv.add(10);
    const auto second_index = sv.add(20);

    CHECK(sv[0] == 10);
    CHECK(sv[1] == 20);
    CHECK(first_index == 0);
    CHECK(second_index == 1);
    CHECK(sv.size() == 2);

    SECTION("erase first element")
    {
        sv.erase(first_index);

        CHECK(sv[second_index] == 20);
        CHECK(sv.size() == 1);


        SECTION("add another element")
        {
            const auto third_index = sv.add(30);

            CHECK(third_index == first_index);
            CHECK(sv[third_index] == 30);
            CHECK(sv.size() == 2);
        }
    }

    SECTION("erase second element")
    {
        sv.erase(second_index);

        CHECK(sv[first_index] == 10);
        CHECK(sv.size() == 1);
    }
}
