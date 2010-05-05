#ifndef KAIKO_UTIL_SERIALIZATION
#define KAIKO_UTIL_SERIALIZATION

#include "Exception.hpp"

#include <boost/noncopyable.hpp>
#include <string>

namespace Kaiko {
namespace Util {

class Serialization : boost::noncopyable {
public:
  template<class TInputIterator>
  static int BytesToLength(TInputIterator begin, TInputIterator end, int* readBytesNum);
  static std::string LengthToBytes(int length);
private:
  Serialization();
};

template<class TInputIterator>
int Serialization::BytesToLength(TInputIterator begin, TInputIterator end, int* readBytesNum) {
  int length = 0;
  *readBytesNum = 0;
  for (TInputIterator it = begin; it != end; ++it) {
    ++(*readBytesNum);
    const char ch = *it;
    length <<= 7;
    length += ch & 0x7f;
    if (length == 0 && ch == '\x80') {
      readBytesNum = 0;
      throw Exception(__FILE__, __LINE__, "redundant bytes");
    }
    if (length < 0) {
      readBytesNum = 0;
      throw Exception(__FILE__, __LINE__, "too big length");
    }
    if ((ch & 0x80) == 0) {
      return length;
    }
  }
  *readBytesNum = 0;
  return 0;
}

}
}

#endif
