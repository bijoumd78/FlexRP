#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "flexrpsharedmemory.h"

TEST_CASE("Get recon module property values", "Properties") {
  static std::vector<std::string> properties{"1", "3.14", "false",
                                             "Hello unit test"};
  FlexRP::FlexRPSharedMemory::createSharedMemory(properties);

  REQUIRE(properties.size() == 4);

  static std::vector<std::string> values;
  FlexRP::FlexRPSharedMemory::getReconmoduleProperty(values, 4);

  SECTION("Get property values") {
    REQUIRE(std::stoi(values[0]) == 1);
    REQUIRE(std::stof(values[1]) == Approx(3.14f));
    REQUIRE(FlexRP::FlexRPSharedMemory::to_bool(values[2]) == false);
    REQUIRE(values[3] == std::string{"Hello unit test"});
  }
}
