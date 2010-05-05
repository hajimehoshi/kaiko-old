#ifndef KAIKO_UTIL_IENUMERABLE_HPP
#define KAIKO_UTIL_IENUMERABLE_HPP

#include "IEnumerator.hpp"
#include <boost/shared_ptr.hpp>

namespace Kaiko {
namespace Util {

template<class T>
class IEnumerable {
public:
  virtual ~IEnumerable() { }
  virtual boost::shared_ptr<IEnumerator<T>> GetEnumerator() const = 0;
};

}
}

#endif
