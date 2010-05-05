#include "Serialization.hpp"

#include <deque>

namespace Kaiko {
namespace Util {

std::string
Serialization::LengthToBytes(int length) {
  std::deque<char> bytes;
  bytes.push_front(static_cast<char>(length & 0x7f));
  while (true) {
    length >>= 7;
    if (!length) {
      break;
    }
    bytes.push_front(static_cast<char>(0x80 | (length & 0x7f)));
  }
  return std::string(bytes.begin(), bytes.end()); 
}

}
}
