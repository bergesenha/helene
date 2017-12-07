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


TEST_CASE("create element_proxy with some values", "[element_proxy]")
{
    typedef helene::element_proxy<
        point,
        helene::member_reference<decltype(&point::x), &point::x>,
        helene::member_reference<decltype(&point::y), &point::y>,
        helene::member_reference<decltype(&point::z), &point::z>>
        point_proxy;

    float x = 1.1f;
    float y = 2.1f;
    float z = 3.1f;

    point_proxy p(x, y, z);

    SECTION("implicitly convert to point")
    {
        point pp = p;

        CHECK(pp.x == Approx(1.1f));
        CHECK(pp.y == Approx(2.1f));
        CHECK(pp.z == Approx(3.1f));
    }

    SECTION("assign point to element_proxy")
    {
        point pp{ 2.2f, 3.3f, 4.4f};

        p = pp;

        CHECK(x == Approx(2.2f));
        CHECK(y == Approx(3.3f));
        CHECK(z == Approx(4.4f));
    }
}
