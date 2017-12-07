#include "catch.hpp"

#include <soa.hpp>


struct point
{
    float x;
    float y;
    float z;
};


TEST_CASE("", "[soa]")
{
    typedef helene::soa<
        point,
        helene::member_container<decltype(&point::x), &point::x>,
        helene::member_container<decltype(&point::y), &point::y>,
        helene::member_container<decltype(&point::z), &point::z>>
        mysoa;

    mysoa a;


    CHECK(a.size() == 0);

    SECTION("push back a value")
    {
        point p{1.1f, 2.2f, 3.3f};


        a.push_back(p);
    }
}
