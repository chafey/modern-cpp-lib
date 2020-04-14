#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <modern_cpp_lib/lib.hpp>

TEST_CASE( "Quick check", "[main]" ) {
    auto result = add(5,10);

    REQUIRE( result == 15 );
}
