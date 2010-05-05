#ifndef KAIKO_IPC_SIMPLESESSION_HPP
#define KAIKO_IPC_SIMPLESESSION_HPP

#include "ISession.hpp"
#include "ITransportClient.hpp"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace Kaiko {
namespace IPC {

class SimpleSession : public ISession,
                      private boost::noncopyable {
public:
  explicit SimpleSession(boost::shared_ptr<ITransportClient> transportClient);
  ~SimpleSession() throw();
  void Close() throw();
  const std::string& GetLastReceivedData() const;
  bool Receive();
  bool Send(const std::string& data);
private:
  struct Impl;
  boost::scoped_ptr<Impl> pimpl;
};

}
}

#endif
