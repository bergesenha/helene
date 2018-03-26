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

        SECTION("push back another value")
        {
            swb.push_back(5);

            CHECK(swb[0] == 2);
            CHECK(swb[1] == 3);
            CHECK(swb[2] == 4);
            CHECK(swb[3] == 5);
        }

        SECTION("push front a value")
        {
            swb.push_front(10);

            CHECK(swb[0] == 10);
            CHECK(swb[1] == 1);
            CHECK(swb[2] == 2);
            CHECK(swb[3] == 3);


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
    }
}
