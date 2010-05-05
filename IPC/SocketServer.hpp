#ifndef KAIKO_IPC_SOCKETSERVER_HPP
#define KAIKO_IPC_SOCKETSERVER_HPP

#include "ITransportServer.hpp"
#include "SocketClient.hpp"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

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
  boost::shared_ptr<ITransportClient> GetLastAcceptedClient() const;
  int GetPort() const;
private:
  struct Impl;
  boost::scoped_ptr<Impl> pimpl;
};

}
}

#endif
