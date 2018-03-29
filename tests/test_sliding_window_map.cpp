#include <catch.hpp>
#include <sliding_window.hpp>


TEST_CASE("default construct a sliding_window_map", "[sliding_window_map]")
{
    helene::sliding_window_map<float, int, 20> swm;

    SECTION("window of default constructed should be 0 - size")
    {
        const auto ext = swm.window();
        CHECK(ext.first == Approx(0.0f));
        CHECK(ext.second == Approx(20.0f));
    }

    SECTION("refer to a key within the extents, range checked, shouldn't throw")
    {
        CHECK_NOTHROW(swm.at(10.0f) == 0);
        CHECK_THROWS_AS(swm.at(20.0f), std::out_of_range);
    }

    SECTION("assign to an element, range checked")
    {
        swm.at(10.5f) = 100;

        CHECK(swm.at(10.5f) == 100);
    }

    SECTION("assign to an element within window, not range checked")
    {
        swm.insert_or_assign(9.5f, 112);

        CHECK(swm.at(9.5f) == 112);
        CHECK(swm[9.5f] == 112);
        CHECK(swm.window().first == Approx(0.0f));
        CHECK(swm.window().second == Approx(20.0f));
    }

    SECTION("assign to an element above the current window")
    {
        swm.insert_or_assign(21.0f, 113);

        CHECK(swm.at(21.0f) == 113);
        CHECK(swm[21.0f] == 113);

        CHECK(swm.window().second == Approx(22.0f));
        CHECK(swm.window().first == Approx(2.0f));
    }

    SECTION("insert to an element below current window")
    {
        swm.insert_or_assign(-3.0f, 50);

        CHECK(swm.at(-3.0f) == 50);
        CHECK(swm[-3.0f] == 50);

        CHECK(swm.window().first == Approx(-3.0f));
        CHECK(swm.window().second == Approx(17.0f));
    }
}


TEST_CASE("construct a sliding_window_map with origin", "[sliding_window_map]")
{
    helene::sliding_window_map<double, char, 5> swm(-2.0);

    CHECK(swm.window().first == Approx(-2.0));
    CHECK(swm.window().second == Approx(3.0));
}
