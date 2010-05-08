#ifndef KAIKO_UTIL_IENUMERABLE_HPP
#define KAIKO_UTIL_IENUMERABLE_HPP

#include "IEnumerator.hpp"

#include <memory>

namespace Kaiko {
namespace Util {

template<class T>
class IEnumerable {
public:
  virtual ~IEnumerable() { }
  virtual std::shared_ptr<IEnumerator<T>> GetEnumerator() const = 0;
};

}
}

#endif
