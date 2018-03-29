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

    SECTION("push back 2 values")
    {
        ssw.push_back(100, 2);

        CHECK(ssw[0] == 0);
        CHECK(ssw[1] == 100);
        CHECK(ssw[2] == 100);
    }

    SECTION("push back 10 values")
    {
        ssw.push_back(100, 10);

        CHECK(ssw[0] == 100);
        CHECK(ssw[1] == 100);
        CHECK(ssw[2] == 100);
    }

    SECTION("push_front 2 values")
    {
        ssw.push_front(11, 2);

        CHECK(ssw[0] == 11);
        CHECK(ssw[1] == 11);
        CHECK(ssw[2] == 0);
    }

    SECTION("push_front 10 values")
    {
        ssw.push_front(12, 10);

        CHECK(ssw[0] == 12);
        CHECK(ssw[1] == 12);
        CHECK(ssw[2] == 12);
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

TEST_CASE("construct static_heap_sliding_window with iterators to range bigger "
          "than Size")
{
    std::vector<int> range{1, 2, 3, 4, 5};

    helene::static_heap_sliding_window<int, 3> shsw(range.begin(), range.end());

    CHECK(shsw[0] == 3);
    CHECK(shsw[1] == 4);
    CHECK(shsw[2] == 5);
}

TEST_CASE("construct static_heap_sliding_window with iterators to range "
          "smaller than Size")
{
    std::vector<int> range{1, 2, 3};

    helene::static_heap_sliding_window<int, 5> shsw(range.begin(), range.end());

    CHECK(shsw[0] == 0);
    CHECK(shsw[1] == 0);
    CHECK(shsw[2] == 1);
    CHECK(shsw[3] == 2);
    CHECK(shsw[4] == 3);
}
