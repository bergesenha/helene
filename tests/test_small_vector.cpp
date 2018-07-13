#include <catch.hpp>
#include <small_vector.hpp>


TEST_CASE("", "")
{
    helene::small_vector<int> intvec;

    CHECK(intvec.size() == 0);

    SECTION("push back a value")
    {
        intvec.push_back(1);

        CHECK(intvec[0] == 1);
        CHECK(intvec.size() == 1);
    }

    SECTION("push back 2 values")
    {
        intvec.push_back(1);
        intvec.push_back(2);

        CHECK(intvec[0] == 1);
        CHECK(intvec[1] == 2);
        CHECK(intvec.size() == 2);
    }
}
