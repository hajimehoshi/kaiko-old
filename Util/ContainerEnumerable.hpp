#ifndef KAIKO_UTIL_CONTAINERENUMERABLE_HPP
#define KAIKO_UTIL_CONTAINERENUMERABLE_HPP

#include "IEnumerable.hpp"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace Kaiko {
namespace Util {

template<class TContainer>
class ContainerEnumerable : public IEnumerable<typename TContainer::value_type>,
                            private boost::noncopyable {
private:
  class Enumerator : public IEnumerator<typename TContainer::value_type>,
                     private boost::noncopyable {
  public:
    Enumerator(typename TContainer::const_iterator begin,
               typename TContainer::const_iterator end);
    typename TContainer::value_type GetCurrent() const;
    bool MoveNext();
  private:
    const typename TContainer::const_iterator begin;
    const typename TContainer::const_iterator end;
    typename TContainer::const_iterator it;
    bool isInit;
  };
public:
  ContainerEnumerable(const TContainer& container);
  boost::shared_ptr<IEnumerator<typename TContainer::value_type>> GetEnumerator() const;
private:
  const TContainer& container;
};

template<class TContainer>
ContainerEnumerable<TContainer>::ContainerEnumerable(const TContainer& container)
  : container(container) {
}

template<class TContainer>
boost::shared_ptr<IEnumerator<typename TContainer::value_type>>
ContainerEnumerable<TContainer>::GetEnumerator() const {
  return boost::shared_ptr<Enumerator>(new Enumerator(container.begin(), container.end()));
}

template<class TContainer>
ContainerEnumerable<TContainer>::Enumerator::Enumerator(typename TContainer::const_iterator begin,
                                                        typename TContainer::const_iterator end)
  : begin(begin), end(end), it(begin), isInit(true) {
}

template<class TContainer>
typename TContainer::value_type
ContainerEnumerable<TContainer>::Enumerator::GetCurrent() const {
  return *this->it;
}

template<class TContainer>
bool
ContainerEnumerable<TContainer>::Enumerator::MoveNext() {
  if (this->it == this->end) {
    return false;
  }
  if (this->isInit) {
    this->isInit = false;
  } else {
    ++this->it;
  }
  return this->it != this->end;
}

}
}

#endif
