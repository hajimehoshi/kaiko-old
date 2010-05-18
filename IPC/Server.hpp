#ifndef KAIKO_IPC_SERVER_HPP
#define KAIKO_IPC_SERVER_HPP

#include "ISessionFactory.hpp"
#include "ITransportServer.hpp"

#include <boost/noncopyable.hpp>
#include <memory>

namespace Kaiko {
namespace IPC {

class Server : private boost::noncopyable {
public:
  Server(const std::shared_ptr<ITransportServer>& transportServer,
         const std::shared_ptr<ISessionFactory>& sessionFactory);
  ~Server() throw();
  bool Process();
private:
  struct Impl;
  const std::unique_ptr<Impl> pimpl;
};

}
}

#endif
