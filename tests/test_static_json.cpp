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


TEST_CASE("", "")
{
    json<field<data_name, int>> js1;

    js1.get<data_name>() = 23;

    CHECK(js1.get<data_name>() == 23);
    CHECK(js1.str() == "{ 23 }");
}


TEST_CASE("json with two fields", "[json]")
{
    json<field<data_name, int>, field<char_name, char>> js1;

    js1.get<data_name>() = 23;
    js1.get<char_name>() = 't';

    CHECK(js1.get<data_name>() == 23);
    CHECK(js1.get<char_name>() == 't');
    CHECK(js1.str() == "{ 23, 116 }");
}
