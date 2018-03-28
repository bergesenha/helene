#include <catch.hpp>
#include <sliding_window.hpp>


TEST_CASE("default construct sliding_windows", "[sliding_window]")
{
    helene::stack_sliding_window<int, 5> ssw;
    helene::static_heap_sliding_window<int, 5> shsw;

    SECTION("push back value 1")
    {
        ssw.push_back(1);
        shsw.push_back(1);

        CHECK(ssw.back() == 1);
        CHECK(shsw.back() == 1);
    }
}
