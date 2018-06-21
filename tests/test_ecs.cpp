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

    const auto first_row = tb.insert_row("Bergesen", 35, 1.83f);
    const auto second_row = tb.insert_row("Lund-Hansen", 70, 1.80f);
}
