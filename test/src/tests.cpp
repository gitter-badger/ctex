#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#define __glogger_implementation__
#include "glogger.hpp"

#include <memory>

#include "ctex.hpp"

std::shared_ptr<CTex> ctex;

std::string run(const std::string& cformula)
{
    GLogger::instance().logInfo("in: ", cformula);
    std::string latex_formula = ctex->translate(cformula);
    GLogger::instance().logInfo("out: ", latex_formula);
    GLogger::instance().logInfo("");
    return latex_formula;
}

TEST_CASE("handle underscore") {
    REQUIRE(
        run("y = x_1;").compare(R"!($$ y = x{\_}1 $$)!") == 0
    );
}

TEST_CASE("transform []") {
    REQUIRE(
        run("y = xin[0] + xin[1];").compare(R"!($$ y = xin _{\normalsize 0 }  + xin _{\normalsize 1 }  $$)!") == 0
    );
}

TEST_CASE("handle frac" ) {
    REQUIRE(
        run("y = tan(x / y);").compare(R"!($$ y = tan \left( \frac{x}{y} \right) $$)!") == 0
    );
}
    
TEST_CASE("handle sqrt" ) {
    REQUIRE(
        run("y = sqrt(x * x + y * y);").compare(R"!($$ y = \sqrt{x \cdot x + y \cdot y} $$)!") == 0
    );
}

int main( int argc, char* argv[] )
{
    GLogger::instance().set_output_mode(GLogger::Console);
    GLogger::instance().set_min_level(GLogger::Console, GLogger::Info);
    ctex = std::make_shared<CTex>(CTex::default_regex());
    int result = Catch::Session().run( argc, argv );
    ctex.reset();
    return ( result < 0xff ? result : 0xff );
}
