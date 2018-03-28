#include <catch.hpp>
#include <sliding_window.hpp>


TEST_CASE("default construct sliding_windows", "[sliding_window]")
{
    helene::stack_sliding_window<int, 3> ssw;

    SECTION("push back value 1")
    {
        ssw.push_back(1);

        CHECK(ssw.back() == 1);

        CHECK(ssw[0] == 0);
        CHECK(ssw[1] == 0);
        CHECK(ssw[2] == 1);
    }

    SECTION("push front value 1")
    {
        ssw.push_front(1);

        CHECK(ssw.front() == 1);

        CHECK(ssw[0] == 1);
        CHECK(ssw[1] == 0);
        CHECK(ssw[2] == 0);
    }
}
