#include "catch.hpp"

#include <soa.hpp>


struct point
{
    float x;
    float y;
    float z;
};

struct price
{
    double datetime;
    int value;
};

TEST_CASE("create an soa with points", "[soa]")
{
    typedef helene::soa<
        point,
        helene::member_container<decltype(&point::x), &point::x>,
        helene::member_container<decltype(&point::y), &point::y>,
        helene::member_container<decltype(&point::z), &point::z>>
        mysoa;

    mysoa a;


    CHECK(a.size() == 0);

    SECTION("push back values")
    {
        point p{1.1f, 2.2f, 3.3f};


        a.push_back(p);
        a.push_back({4.4f, 5.5f, 6.6f});
        REQUIRE(a.size() == 2);

        SECTION("access first element by array access")
        {
            point pp = a[0];

            CHECK(pp.x == Approx(1.1f));
            CHECK(pp.y == Approx(2.2f));
            CHECK(pp.z == Approx(3.3f));
        }

        SECTION("access underlying buffer of x members")
        {
            auto x_data = a.data<decltype(&point::x), &point::x>();

            CHECK(*x_data == Approx(1.1f));
            CHECK(x_data[1] == Approx(4.4f));
        }
    }

    SECTION("push back by members")
    {
        a.push_back_members(1.1f, 2.2f, 3.3f);
        a.push_back_members(4.4f, 5.5f, 6.6f);

        point pp1 = a[0];
        point pp2 = a[1];

        CHECK(pp1.x == Approx(1.1f));
        CHECK(pp1.y == Approx(2.2f));
        CHECK(pp1.z == Approx(3.3f));
        CHECK(pp2.x == Approx(4.4f));
        CHECK(pp2.y == Approx(5.5f));
        CHECK(pp2.z == Approx(6.6f));
    }
}

TEST_CASE("create an soa with members of price out of order", "[soa]")
{
    typedef helene::soa<
        price,
        helene::member_container<decltype(&price::value), &price::value>,
        helene::member_container<decltype(&price::datetime), &price::datetime>>
        price_soa;

    price_soa pa;

    SECTION("push back values")
    {
        price p1{1.34, 10};
        price p2{2.34, 20};

        pa.push_back(p1);
        pa.push_back(p2);

        SECTION("access elements")
        {
            price pp1 = pa[0];
        }

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
        point pp{2.2f, 3.3f, 4.4f};

        p = pp;

        CHECK(x == Approx(2.2f));
        CHECK(y == Approx(3.3f));
        CHECK(z == Approx(4.4f));
    }
}
