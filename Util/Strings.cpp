#include "Strings.hpp"

#include "Exception.hpp"

namespace Kaiko {
namespace Util {

/*
 * TOOD: implement more precise validator
 */
std::wstring
Strings::UTF8ToUTF16LE(const std::string& str) {
  std::wstring result;
  result.reserve(str.size());
  for (std::string::const_iterator it = str.begin();
       it != str.end();
       ++it) {
    unsigned char ch0 = *it;
    if ((ch0 & 0x80) == 0x00) {
      result += ch0;
    } else if ((ch0 & 0xe0) == 0xc0) {
      ++it;
      if (it == str.end()) {
        throw Exception(__FILE__, __LINE__, "invalid UTF-8 sequence");
      }
      unsigned char ch1 = *it;
      result += ((ch0 & 0x3f) << 6) | (ch1 & 0x3f);
    } else if ((ch0 & 0xf0) == 0xe0) {
      ++it;
      if (it == str.end()) {
        throw Exception(__FILE__, __LINE__, "invalid UTF-8 sequence");
      }
      unsigned char ch1 = *it;
      ++it;
      if (it == str.end()) {
        throw Exception(__FILE__, __LINE__, "invalid UTF-8 sequence");
      }
      unsigned char ch2 = *it;
      result += ((ch0 & 0x0f) << 12) | ((ch1 & 0x3f) << 6) | (ch2 & 0x3f);
    } else {
      throw Exception(__FILE__, __LINE__, "invalid UTF-8 sequence");
    }
  }
  return result;
}

}
}
