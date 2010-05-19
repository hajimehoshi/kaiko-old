#include "SocketServer.hpp"

#include "SocketException.hpp"

#include <winsock2.h>

namespace Kaiko {
namespace IPC {

struct SocketServer::Impl {
  explicit Impl(int port)
    : port(port) {
    this->socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->socket == INVALID_SOCKET) {
      throw SocketException(__FILE__, __LINE__);
    }
    sockaddr_in addr = { 0 };
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    addr.sin_port        = ::htons(static_cast<unsigned short>(port));
    if (::bind(this->socket, (sockaddr *)&addr, sizeof(addr))) {
      throw SocketException(__FILE__, __LINE__);
    }
    if (::listen(this->socket, 10)) {
      throw SocketException(__FILE__, __LINE__);
    }
  }
  SOCKET socket;
  int port;
  std::shared_ptr<ITransportClient> lastAcceptedSocketClient;
};

SocketServer::SocketServer()
  : pimpl(new Impl(0)) {
}

SocketServer::SocketServer(int port)
  : pimpl(new Impl(port)) {
}

SocketServer::~SocketServer() {
  this->Close();
}

bool
SocketServer::Accept() {
  if (this->pimpl->socket == INVALID_SOCKET) {
    this->pimpl->lastAcceptedSocketClient.reset();
    return false;
  }
  // non blocking
  unsigned long val = 1;
  if (::ioctlsocket(this->pimpl->socket, FIONBIO, &val)) {
    throw SocketException(__FILE__, __LINE__);
  }
  const SOCKET clientSocket = ::accept(this->pimpl->socket, nullptr, nullptr);
  if (clientSocket != INVALID_SOCKET) {
    this->pimpl->lastAcceptedSocketClient.reset(new SocketClient(clientSocket));
  } else {
    this->pimpl->lastAcceptedSocketClient.reset();
  }
  return true;
}

void
SocketServer::Close() {
  if (this->pimpl->socket != INVALID_SOCKET) {
    ::shutdown(this->pimpl->socket, SD_BOTH);
    ::closesocket(this->pimpl->socket);
    this->pimpl->socket = INVALID_SOCKET;
  }
}

const std::shared_ptr<ITransportClient>&
SocketServer::GetLastAcceptedClient() const {
  return this->pimpl->lastAcceptedSocketClient;
}

int
SocketServer::GetPort() const {
  if (this->pimpl->port == 0) {
    sockaddr_in addr = { 0 };
    int len = sizeof(addr);
    if (::getsockname(this->pimpl->socket, (sockaddr *)&addr, &len)) {
      throw SocketException(__FILE__, __LINE__);
    }
    this->pimpl->port = ::ntohs(addr.sin_port);
  }
  return this->pimpl->port;
}

}
}
