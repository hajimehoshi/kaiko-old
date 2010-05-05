#ifndef KAIKO_UTIL_STRINGS
#define KAIKO_UTIL_STRINGS

#include <boost/noncopyable.hpp>
#include <string>

namespace Kaiko {
namespace Util {

class Strings : boost::noncopyable {
public:
  static std::wstring UTF8ToUTF16LE(const std::string& str);
private:
  Strings();
};

}
}

#endif
