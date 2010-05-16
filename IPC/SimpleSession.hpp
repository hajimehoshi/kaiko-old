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
  explicit SimpleSession(const std::shared_ptr<ITransportClient>& transportClient);
  ~SimpleSession() throw();
  void AddDataToSend(const std::string& data);
  void Close() throw();
  const std::string& GetLastReceivedData() const;
  bool Receive();
  bool Send();
private:
  struct Impl;
  const std::unique_ptr<Impl> pimpl;
};

}
}

#endif
