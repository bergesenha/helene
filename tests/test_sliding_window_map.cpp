#include <catch.hpp>
#include <sliding_window.hpp>


TEST_CASE("default construct a sliding_window_map", "[sliding_window_map]")
{
    helene::sliding_window_map<float, int, 20> swm;

    SECTION("extent of default constructed should be 0 - size")
    {
        const auto ext = swm.extent();
        CHECK(ext.first == Approx(0.0f));
        CHECK(ext.second == Approx(20.0f));
    }

    SECTION("refer to a key within the extents shouldn't throw")
    {
        CHECK_NOTHROW(swm.at(10.0f) == 0);
        CHECK_THROWS_AS(swm.at(20.0f), std::out_of_range);
    }
}
