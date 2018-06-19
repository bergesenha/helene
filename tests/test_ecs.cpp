#include <catch.hpp>

#include <ecs.hpp>


enum class TableName
{
    Numbers,
    Names
};

template <TableName Name, class T>
using named_table = helene::table_description<TableName, Name, T>;


TEST_CASE("", "")
{
    using helene::database;

    database<named_table<TableName::Numbers, int>,
             named_table<TableName::Names, std::string>>
        db;
}
