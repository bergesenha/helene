#include <catch.hpp>

#include <ecs.hpp>


enum class ColumnNames
{
    Surname,
    Age,
    Height
};

template <ColumnNames Name, class T>
using mycolumn = helene::column_description<ColumnNames, Name, T>;


TEST_CASE("", "")
{
    helene::table<ColumnNames,
                  mycolumn<ColumnNames::Surname, std::string>,
                  mycolumn<ColumnNames::Age, int>,
                  mycolumn<ColumnNames::Height, float>>
        tb;
}
