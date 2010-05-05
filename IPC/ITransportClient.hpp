#ifndef KAIKO_IPC_ITRANSPORTCLIENT_HPP
#define KAIKO_IPC_ITRANSPORTCLIENT_HPP

#include <string>

namespace Kaiko {
namespace IPC {

class ITransportClient {
public:
  virtual ~ITransportClient() { }
  virtual void Close() throw() = 0;
  virtual const std::string& GetLastReceivedData() const = 0;
  virtual bool Receive() = 0;
  virtual bool Send(const std::string&) = 0;
};

}
}

#endif
