#ifndef KAIKO_UTIL_EXEPTION_HPP
#define KAIKO_UTIL_EXEPTION_HPP

#include <boost/shared_ptr.hpp>
#include <exception>
#include <string>

namespace Kaiko {
namespace Util {

class Exception : public std::exception {
public:
  Exception(const std::string& file, int line);
  Exception(const std::string& file, int line, const std::string& message);
  virtual ~Exception() throw();
  virtual const std::string& What() const;
  virtual const char* what() const;
public:
  struct Impl;
  boost::shared_ptr<const Impl> pimpl;
};

}
}

#endif
