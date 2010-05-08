#ifndef KAIKO_UTIL_SYSTEMEXEPTION_HPP
#define KAIKO_UTIL_SYSTEMEXEPTION_HPP

#include "Exception.hpp"

#include <memory>

namespace Kaiko {
namespace Util {

class SystemException : public Exception {
public:
  SystemException(const std::string& file, int line);
  SystemException(const std::string& file, int line, int errorNo);
  virtual ~SystemException() throw();
  int GetErrorNo() const;
private:
  struct Impl;
  std::shared_ptr<const Impl> pimpl;
};

}
}

#endif
