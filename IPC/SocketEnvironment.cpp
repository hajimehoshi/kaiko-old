#include "SocketEnvironment.hpp"

#include "SocketException.hpp"

#include <winsock2.h>

namespace Kaiko {
namespace IPC {

SocketEnvironment::SocketEnvironment() {
  WSADATA data;
  if (::WSAStartup(MAKEWORD(2, 0), &data)) {
    throw SocketException(__FILE__, __LINE__);
  }
}

SocketEnvironment::~SocketEnvironment() throw() {
  ::WSACleanup();
}

}
}
