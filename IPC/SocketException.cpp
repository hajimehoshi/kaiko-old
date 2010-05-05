#include "SocketException.hpp"

#include "winsock2.h"

namespace Kaiko {
namespace IPC {

SocketException::SocketException(const std::string& file, int line)
  : SystemException(file, line, ::WSAGetLastError()) {
}

SocketException::~SocketException() {
}

}
}
