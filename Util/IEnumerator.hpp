#ifndef KAIKO_UTIL_IENUMERATOR_HPP
#define KAIKO_UTIL_IENUMERATOR_HPP

namespace Kaiko {
namespace Util {

template<class T>
class IEnumerator {
public:
  virtual ~IEnumerator() { }
  virtual T GetCurrent() const = 0;
  virtual bool MoveNext() = 0;
};

}
}

#endif
