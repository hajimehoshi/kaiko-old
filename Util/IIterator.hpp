#ifndef KAIKO_UTIL_IITERATOR_HPP
#define KAIKO_UTIL_IITERATOR_HPP

namespace Kaiko {
namespace Util {

template<class T>
class IIterator {
public:
  virtual ~IIterator();
  virtual T operator*() const = 0;
  virtual IIterator<T> operator++() = 0;
  virtual IIterator<T> operator++(int) = 0;
  virtual bool operator==(const IIterator<T>& rhs) const = 0;
  virtual bool operator!=(const IIterator<T>& rhs) const = 0;
};

}
}

#endif
