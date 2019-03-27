#include "coevent/endpoint.h"

#include "3rd_party/catch2/catch.hpp"

TEST_CASE("Endpoint manages an IP address and port.") {
  coevent::endpoint endpoint1, endpoint2{"127.0.0.1", 9000}, endpoint3{"FE80:CD00:0000:03DE:1257:0000:211E:729C", 9000};

  SECTION("Endpoints are comparable.") {
    REQUIRE(endpoint1 == endpoint1);
    REQUIRE(endpoint1 != endpoint2);
    REQUIRE(endpoint2 == endpoint2);
    REQUIRE(endpoint3 != endpoint2);
  }
}
