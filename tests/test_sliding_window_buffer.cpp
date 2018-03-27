#include <vector>
#include <algorithm>
#include <iterator>

#include <catch.hpp>
#include <sliding_window_buffer.hpp>


typedef helene::sliding_window_buffer<int, 4> swb_type;

TEST_CASE("default construct a sliding_window_buffer",
          "[sliding_window_buffer]")
{
    swb_type swb;

    CHECK(swb.size() == 4);

    SECTION("push back four values")
    {
        swb.push_back(1);
        swb.push_back(2);
        swb.push_back(3);
        swb.push_back(4);

        CHECK(swb[0] == 1);
        CHECK(swb[1] == 2);
        CHECK(swb[2] == 3);
        CHECK(swb[3] == 4);

        CHECK(swb.front() == 1);
        CHECK(swb.back() == 4);

        SECTION("push back another value")
        {
            swb.push_back(5);

            CHECK(swb[0] == 2);
            CHECK(swb[1] == 3);
            CHECK(swb[2] == 4);
            CHECK(swb[3] == 5);

            CHECK(swb.front() == 2);
            CHECK(swb.back() == 5);
        }

        SECTION("push front a value")
        {
            swb.push_front(10);

            CHECK(swb[0] == 10);
            CHECK(swb[1] == 1);
            CHECK(swb[2] == 2);
            CHECK(swb[3] == 3);

            CHECK(swb.front() == 10);
            CHECK(swb.back() == 3);


            SECTION("copy full range into vector")
            {
                std::vector<int> res;

                std::copy(swb.begin(), swb.end(), std::back_inserter(res));

                CHECK(res.size() == 4);
                CHECK(res[0] == 10);
                CHECK(res[1] == 1);
                CHECK(res[2] == 2);
                CHECK(res[3] == 3);
            }
        }

        SECTION("copy full range into vector")
        {
            std::vector<int> res;

            std::copy(swb.begin(), swb.end(), std::back_inserter(res));

            CHECK(res.size() == 4);
            CHECK(res[0] == 1);
            CHECK(res[1] == 2);
            CHECK(res[2] == 3);
            CHECK(res[3] == 4);
        }

        SECTION("access values through const_iterators")
        {
            auto cb = swb.cbegin();
            auto ce = swb.cend();

            CHECK(*cb == 1);
            CHECK(cb[1] == 2);
            CHECK(cb[2] == 3);
            CHECK(cb[3] == 4);
            CHECK(cb + 4 == ce);
        }
    }
}


TEST_CASE("construct sliding_window_buffer with range",
          "[sliding_window_buffer]")
{
    std::vector<int> vals{1, 2, 3, 4};

    swb_type swb(vals.begin(), vals.end());

    CHECK(swb[0] == 1);
    CHECK(swb[1] == 2);
    CHECK(swb[2] == 3);
    CHECK(swb[3] == 4);
    CHECK(!swb.empty());

    SECTION("push back another value")
    {
        swb.push_back(5);

        CHECK(swb[0] == 2);
        CHECK(swb[1] == 3);
        CHECK(swb[2] == 4);
        CHECK(swb[3] == 5);

        CHECK(swb.front() == 2);
        CHECK(swb.back() == 5);
    }

    SECTION("push front a value")
    {
        swb.push_front(10);

        CHECK(swb[0] == 10);
        CHECK(swb[1] == 1);
        CHECK(swb[2] == 2);
        CHECK(swb[3] == 3);

        CHECK(swb.front() == 10);
        CHECK(swb.back() == 3);


        SECTION("copy full range into vector")
        {
            std::vector<int> res;

            std::copy(swb.begin(), swb.end(), std::back_inserter(res));

            CHECK(res.size() == 4);
            CHECK(res[0] == 10);
            CHECK(res[1] == 1);
            CHECK(res[2] == 2);
            CHECK(res[3] == 3);
        }

        SECTION("copy construct a sliding_window_buffer from first")
        {
            swb_type swb2(swb);

            CHECK(swb2 == swb);

            CHECK(swb2[0] == 10);
            CHECK(swb2[1] == 1);
            CHECK(swb2[2] == 2);
            CHECK(swb2[3] == 3);

            SECTION("swap sliding_window_buffers")
            {
                swb2.swap(swb);

                CHECK(swb == swb2);
                CHECK(swb[0] == swb2[0]);
                CHECK(swb[1] == swb2[1]);
                CHECK(swb[2] == swb2[2]);
                CHECK(swb[3] == swb2[3]);
            }
        }
    }

    SECTION("copy full range into vector")
    {
        std::vector<int> res;

        std::copy(swb.begin(), swb.end(), std::back_inserter(res));

        CHECK(res.size() == 4);
        CHECK(res[0] == 1);
        CHECK(res[1] == 2);
        CHECK(res[2] == 3);
        CHECK(res[3] == 4);
    }

    SECTION("copy construct sliding_window_buffer from first")
    {
        swb_type swb2(swb);
        CHECK(swb2[0] == 1);
        CHECK(swb2[1] == 2);
        CHECK(swb2[2] == 3);
        CHECK(swb2[3] == 4);

        SECTION("modify copy")
        {
            swb2[0] = 10;
            swb2[2] = 30;

            CHECK(swb2[0] == 10);
            CHECK(swb2[2] == 30);
            CHECK(swb[0] == 1);
            CHECK(swb[2] == 3);

            CHECK(swb2 != swb);

            SECTION("assign first to it again")
            {
                swb2 = swb;
                CHECK(swb2[0] == 1);
                CHECK(swb2[1] == 2);
                CHECK(swb2[2] == 3);
                CHECK(swb2[3] == 4);
            }

            SECTION("swap the two sliding_window_buffers")
            {
                std::swap(swb2, swb);

                CHECK(swb2 != swb);
                CHECK(swb2[0] == 1);
                CHECK(swb2[2] == 3);
                CHECK(swb[0] == 10);
                CHECK(swb[2] == 30);
            }
        }
    }
}
