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


TEST_CASE("default construct sliding_window_map", "[sliding_window_map]")
{
    helene::sliding_window_map<double, int, 20> swm;

    const auto ex = swm.extent();

    CHECK(ex.first == Approx(0.0));
    CHECK(ex.second == Approx(20.0));

    CHECK(swm.at(10.0) == 0);
}
