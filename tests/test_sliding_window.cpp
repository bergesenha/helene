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


TEST_CASE("construct static_heap_sliding_window with iterators to range")
{
    std::string initial_message("hello world");
    helene::static_heap_sliding_window<std::string::value_type, 11> message(
        initial_message.begin(), initial_message.end());

    CHECK(message[0] == 'h');
    CHECK(message[1] == 'e');
    CHECK(message[2] == 'l');
    CHECK(message[3] == 'l');
    CHECK(message[4] == 'o');
    CHECK(message[5] == ' ');
    CHECK(message[6] == 'w');
    CHECK(message[7] == 'o');
    CHECK(message[8] == 'r');
    CHECK(message[9] == 'l');
    CHECK(message[10] == 'd');
}
