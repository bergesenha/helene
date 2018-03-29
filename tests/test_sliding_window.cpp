#include <numeric>

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

    SECTION("set all elements in window by iterators")
    {
        auto beg_it = ssw.begin();
        auto end_it = ssw.end();

        CHECK(std::distance(beg_it, end_it) == 3);

        std::iota(beg_it, end_it, 10);


        CHECK(ssw[0] == 10);
        CHECK(ssw[1] == 11);
        CHECK(ssw[2] == 12);

        SECTION("push_back 2 elements")
        {
            ssw.push_back(int(), 2);

            CHECK(ssw[0] == 12);
            CHECK(ssw[1] == int());
            CHECK(ssw[2] == int());
        }
    }
}
