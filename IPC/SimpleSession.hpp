#ifndef KAIKO_IPC_SIMPLESESSION_HPP
#define KAIKO_IPC_SIMPLESESSION_HPP

#include "ISession.hpp"
#include "ITransportClient.hpp"

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>

namespace Kaiko {
namespace IPC {

class SimpleSession : public ISession,
                      private boost::noncopyable {
public:
  explicit SimpleSession(std::shared_ptr<ITransportClient> transportClient);
  ~SimpleSession() throw();
  void Close() throw();
  const std::string& GetLastReceivedData() const;
  bool Receive();
  bool Send(const std::string& data);
private:
  struct Impl;
  const std::unique_ptr<Impl> pimpl;
};

}
}

#endif
