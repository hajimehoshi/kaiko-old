#include "SystemException.hpp"

#include <windows.h>

namespace Kaiko {
namespace Util {

namespace {

std::string
ErrorNoToMessage(int errorNo) {
  TCHAR* buffer = 0;
  int length;
  length = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                           FORMAT_MESSAGE_FROM_SYSTEM |
                           FORMAT_MESSAGE_IGNORE_INSERTS,
                           0,
                           errorNo,
                           MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                           reinterpret_cast<TCHAR*>(&buffer),
                           0,
                           0);
  if (0 < length && buffer != 0) {
    static const int mbBufferSize = 256;
    char mbBuffer[mbBufferSize];
    size_t mbLength;
    wcstombs_s(&mbLength,
               mbBuffer,
               mbBufferSize,
               buffer,
               _TRUNCATE);
    if (mbBufferSize - 1 < mbLength) {
      mbLength = mbBufferSize - 1;
    }
    mbBuffer[mbLength] = '\0';
    return std::string(mbBuffer);
  } else {
    // TODO: fix it
    throw "invalid error no?";
  }
}

}

struct SystemException::Impl {
  Impl(int errorNo)
    : errorNo(errorNo) {
  }
  int errorNo;
};

SystemException::SystemException(const std::string& file, int line)
  : Exception(file, line, ErrorNoToMessage(::GetLastError())), pimpl(new Impl(::GetLastError())) {
}

SystemException::SystemException(const std::string& file, int line, int errorNo)
  : Exception(file, line, ErrorNoToMessage(errorNo)), pimpl(new Impl(errorNo)) {
}

SystemException::~SystemException() {
}

int
SystemException::GetErrorNo() const {
  return this->pimpl->errorNo;
}

}
}
