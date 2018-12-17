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
