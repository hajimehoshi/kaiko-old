#include "SocketClient.hpp"

#include "SocketException.hpp"

namespace Kaiko {
namespace IPC {

struct SocketClient::Impl {
  explicit Impl(SOCKET socket)
    : socket(socket) {
  }
  SOCKET socket;
  std::string lastReceivedData;
};

SocketClient::SocketClient(const std::string& ip, int port)
  : pimpl(new Impl(INVALID_SOCKET)) {
  this->pimpl->socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (this->pimpl->socket == INVALID_SOCKET) {
    throw SocketException(__FILE__, __LINE__);
  }
  sockaddr_in addr = { 0 };
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = ::inet_addr(ip.c_str());
  addr.sin_port        = ::htons(static_cast<unsigned short>(port));
  if (::connect(this->pimpl->socket, (sockaddr *)&addr, sizeof(addr))) {
    throw SocketException(__FILE__, __LINE__);
  }
}

SocketClient::SocketClient(SOCKET socket)
  : pimpl(new Impl(socket)) {
}

SocketClient::~SocketClient() throw() {
  this->Close();
}

void
SocketClient::Close() throw() {
  if (this->pimpl->socket != INVALID_SOCKET) {
    ::shutdown(this->pimpl->socket, SD_BOTH);
    ::closesocket(this->pimpl->socket);
    this->pimpl->socket = INVALID_SOCKET;
  }
}

const std::string&
SocketClient::GetLastReceivedData() const {
  return this->pimpl->lastReceivedData;
}

bool
SocketClient::Receive() {
  if (this->pimpl->socket == INVALID_SOCKET) {
    return false;
  }
  // non blocking
  unsigned long val = 1;
  if (::ioctlsocket(this->pimpl->socket, FIONBIO, &val)) {
    throw SocketException(__FILE__, __LINE__);
  }
  char buffer[4096];
  const int length = ::recv(this->pimpl->socket, buffer, sizeof(buffer) / sizeof(buffer[0]), 0);
  switch (length) {
  case 0:
    this->pimpl->lastReceivedData.clear();
    return false;
  case SOCKET_ERROR:
    if (::WSAGetLastError() == WSAEWOULDBLOCK) {
      this->pimpl->lastReceivedData.clear();
      return true;
    } else {
      throw SocketException(__FILE__, __LINE__);
    }
  default:
    this->pimpl->lastReceivedData.swap(std::string(buffer, length));
    return true;
  }
}

bool
SocketClient::Send(const std::string& data) {
  if (this->pimpl->socket == INVALID_SOCKET) {
    return false;
  }
  if (data.empty()) {
    return true;
  }
  // non blocking
  unsigned long val = 1;
  if (::ioctlsocket(this->pimpl->socket, FIONBIO, &val)) {
    throw SocketException(__FILE__, __LINE__);
  }
  const int length = ::send(this->pimpl->socket, data.c_str(), data.length(), 0);
  switch (length) {
  case 0:
    return false;
  case SOCKET_ERROR:
    if (::WSAGetLastError() == WSAEWOULDBLOCK) {
      return true;
    } else {
      throw SocketException(__FILE__, __LINE__);
    }
  default:
    if (static_cast<int>(data.size()) != length) {
      throw SocketException(__FILE__, __LINE__);
    }
    return true;
  }
}

}
}
