#include "coevent/endpoint.h"

#include <cstring>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include <arpa/inet.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
endpoint::endpoint(const char* s, uint16_t port) {
  auto& ipv4_addr = reinterpret_cast<sockaddr_in&>(data_);
  auto rcode = inet_pton(AF_INET, s, static_cast<void*>(&ipv4_addr.sin_addr));
  if (rcode == 1) {
    ipv4_addr.sin_family = AF_INET;
    ipv4_addr.sin_port = htons(port);
    return;
  }

  auto& ipv6_addr = reinterpret_cast<sockaddr_in6&>(data_);
  rcode = inet_pton(AF_INET6, s, static_cast<void*>(&ipv6_addr.sin6_addr));
  if (rcode == 1) {
    ipv6_addr.sin6_family = AF_INET6;
    ipv6_addr.sin6_port = htons(port);
    return;
  }

  std::ostringstream oss;
  oss << "failed to construct IpAddress from " << s;
  throw std::runtime_error{oss.str()};
}

//--------------------------------------------------------------------------------------------------
// length
//--------------------------------------------------------------------------------------------------
size_t endpoint::length() const noexcept {
  switch (this->family()) {
    case AF_INET:
      return sizeof(sockaddr_in);
    case AF_INET6:
      return sizeof(sockaddr_in6);
  }
  std::cerr << "unknown family\n";
  std::terminate();
}

//--------------------------------------------------------------------------------------------------
// operator==
//--------------------------------------------------------------------------------------------------
bool operator==(const endpoint& lhs, const endpoint& rhs) noexcept {
  auto rcode = std::memcmp(static_cast<const void*>(&lhs.data_), 
      static_cast<const void*>(&rhs.data_), sizeof(lhs.data_));
  return rcode == 0;
}
} // namespace coevent
