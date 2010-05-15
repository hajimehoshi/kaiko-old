#ifndef KAIKO_UTIL_SERIALIZATION
#define KAIKO_UTIL_SERIALIZATION

#include "Exception.hpp"

#include <boost/noncopyable.hpp>
#include <string>
#include <tuple>

namespace Kaiko {
namespace Util {

class Serialization : boost::noncopyable {
public:
  template<class TInputIterator>
  static std::tuple<int, int> BytesToLength(const TInputIterator& begin, const TInputIterator& end);
  static std::string LengthToBytes(int length);
private:
  Serialization();
};

template<class TInputIterator>
std::tuple<int, int>
Serialization::BytesToLength(const TInputIterator& begin, const TInputIterator& end) {
  int length = 0;
  int readBytesNum = 0;
  for (TInputIterator it = begin; it != end; ++it) {
    ++readBytesNum;
    const char ch = *it;
    length <<= 7;
    length += ch & 0x7f;
    if (length == 0 && ch == '\x80') {
      throw Exception(__FILE__, __LINE__, "redundant bytes");
    }
    if (length < 0) {
      throw Exception(__FILE__, __LINE__, "too big length");
    }
    if ((ch & 0x80) == 0) {
      return std::make_tuple(length, readBytesNum);
    }
  }
  return std::make_tuple(0, 0);
}

}
}

#endif
