#ifndef KAIKO_IPC_SOCKETSERVER_HPP
#define KAIKO_IPC_SOCKETSERVER_HPP

#include "ITransportServer.hpp"
#include "SocketClient.hpp"

#include <boost/noncopyable.hpp>
#include <memory>

namespace Kaiko {
namespace IPC {

class SocketServer : public ITransportServer,
                     private boost::noncopyable {
public:
  SocketServer();
  explicit SocketServer(int port);
  ~SocketServer() throw();
  bool Accept();
  void Close() throw();
  std::shared_ptr<ITransportClient> GetLastAcceptedClient() const;
  int GetPort() const;
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};

}
}

#endif
