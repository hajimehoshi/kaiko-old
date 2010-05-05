#ifndef KAIKO_IPC_ITRANSPORTSERVER_HPP
#define KAIKO_IPC_ITRANSPORTSERVER_HPP

#include "ITransportClient.hpp"

#include <boost/shared_ptr.hpp>

namespace Kaiko {
namespace IPC {

class ITransportServer {
public:
  virtual ~ITransportServer() { }
  virtual bool Accept() = 0;
  virtual void Close() throw() = 0;
  virtual boost::shared_ptr<ITransportClient> GetLastAcceptedClient() const = 0;
};

}
}

#endif
