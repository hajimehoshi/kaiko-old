#ifndef KAIKO_IPC_SOCKETCLIENT_HPP
#define KAIKO_IPC_SOCKETCLIENT_HPP

#include "ITransportClient.hpp"

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>
#include <winsock2.h>

namespace Kaiko {
namespace IPC {

class SocketClient : public ITransportClient,
                     private boost::noncopyable {
public:
  SocketClient(const std::string& ip, int port);
  explicit SocketClient(SOCKET socket);
  ~SocketClient();
  void Close();
  const std::string& GetLastReceivedData() const;
  bool Receive();
  bool Send(const std::string&);
private:
  struct Impl;
  const std::unique_ptr<Impl> pimpl;
};

}
}

#endif
