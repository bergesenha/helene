#include <catch.hpp>

#include <static_json.hpp>


struct data_name
{
    static constexpr const char* value = "data";
};

struct long_name
{
    static constexpr const char* value = "long_name";
};

struct object_name
{
    static constexpr const char* value = "subobj";
};

using helene::static_json::field;
using helene::static_json::json;


TEST_CASE("json with one field", "[json]")
{
    json<field<data_name, int>> js1;

    js1.get<data_name>() = 23;

    CHECK(js1.get<data_name>() == 23);
    CHECK(js1.str() == "{ data: 23 }");

    SECTION("stream out")
    {
        std::ostringstream js1out;
        js1out << js1;
        CHECK(js1out.str() == js1.str());
    }
}


TEST_CASE("json with two fields", "[json]")
{
    json<field<data_name, int>, field<long_name, std::size_t>> js1;

    js1.get<data_name>() = 23;
    js1.get<long_name>() = 1001;

    CHECK(js1.get<data_name>() == 23);
    CHECK(js1.get<long_name>() == 1001);
    CHECK(js1.str() == "{ data: 23, long_name: 1001 }");

    SECTION("stream out")
    {
        std::ostringstream js1out;
        js1out << js1;
        CHECK(js1out.str() == js1.str());
    }

    SECTION("mutate and stream in/deserialize")
    {
        std::istringstream in(js1.str());
        js1.get<data_name>() = 50;
        js1.get<long_name>() = 2001;

        in >> js1;

        CHECK(js1.get<data_name>() == 23);
        CHECK(js1.get<long_name>() == 1001);
    }
}


TEST_CASE("json with a number and a subobject", "[json]")
{
    json<field<data_name, int>,
         field<object_name, json<field<long_name, char>>>>
        js2;

    js2.get<data_name>() = 101;
    js2.get<object_name>().get<long_name>() = 'a';

    CHECK(js2.get<data_name>() == 101);
    CHECK(js2.get<object_name>().get<long_name>() == 'a');
    CHECK(js2.str() == "{ data: 101, subobj: { long_name: 97 } }");
}
