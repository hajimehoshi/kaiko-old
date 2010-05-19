#include "Exception.hpp"

#include <sstream>

namespace Kaiko {
namespace Util {

namespace {

std::string
FormatErrorMessage(const std::string& file, int line, const std::string& message) {
  std::ostringstream oss;
  oss << file << "(" << line << "): " << message;
  return oss.str();
}

}

struct Exception::Impl {
  Impl(const std::string& message)
    : message(message) {
  }
  std::string message;
};

Exception::Exception(const std::string& file, int line)
  : exception(), pimpl(new Impl(FormatErrorMessage(file, line, "unknown error"))) {
}

Exception::Exception(const std::string& file, int line, const std::string& message)
  : exception(), pimpl(new Impl(FormatErrorMessage(file, line, message))) {
}

Exception::~Exception() {
}

const std::string&
Exception::What() const {
  return this->pimpl->message;
}

const char*
Exception::what() const {
  return this->pimpl->message.c_str();
}

}
}
