#include <catch.hpp>

#include <static_json.hpp>


struct data_name
{
    static constexpr const char* value = "data";
};

struct char_name
{
    static constexpr const char* value = "char_name";
};

struct object_name
{
    static constexpr const char* value = "subobj";
};

using helene::static_json::field;
using helene::static_json::json;
using helene::static_json::value_type;


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
    json<field<data_name, int>, field<char_name, char>> js1;

    js1.get<data_name>() = 23;
    js1.get<char_name>() = 't';

    CHECK(js1.get<data_name>() == 23);
    CHECK(js1.get<char_name>() == 't');
    CHECK(js1.str() == "{ data: 23, char_name: 116 }");

    SECTION("stream out")
    {
        std::ostringstream js1out;
        js1out << js1;
        CHECK(js1out.str() == js1.str());
    }
}


TEST_CASE("json with a number and a subobject", "[json]")
{
    json<field<data_name, int>,
         field<object_name, json<field<char_name, char>>>>
        js2;

    js2.get<data_name>() = 101;
    js2.get<object_name>().get<char_name>() = 'a';

    CHECK(js2.get<data_name>() == 101);
    CHECK(js2.get<object_name>().get<char_name>() == 'a');
    CHECK(js2.str() == "{ data: 101, subobj: { char_name: 97 } }");
}
