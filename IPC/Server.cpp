#include "Server.hpp"

#include "ISession.hpp"
#include "../Util/SystemException.hpp"

#include <boost/unordered_set.hpp>
#include <cassert>

namespace Kaiko {
namespace IPC {

struct Server::Impl : private boost::noncopyable {
  typedef boost::unordered_set<std::shared_ptr<ISession>> Sessions;
  Impl(const std::shared_ptr<ITransportServer>& transportServer,
       const std::shared_ptr<ISessionFactory>& sessionFactory)
    : transportServer(transportServer),
      sessionFactory(sessionFactory) {
  }
  const std::shared_ptr<ITransportServer> transportServer;
  const std::shared_ptr<ISessionFactory> sessionFactory;
  Sessions sessions;
};

Server::Server(const std::shared_ptr<ITransportServer>& transportServer,
               const std::shared_ptr<ISessionFactory>& sessionFactory)
  : pimpl(new Impl(transportServer, sessionFactory)) {
}

Server::~Server() throw() {
}

bool
Server::Process() {
  try {
    if (!this->pimpl->transportServer->Accept()) {
      return false;
    }
  } catch (const Util::SystemException&) {
    // TODO: logging
    return false;
  }
  auto transportClient = this->pimpl->transportServer->GetLastAcceptedClient();
  if (transportClient) {
    this->pimpl->sessions.insert(this->pimpl->sessionFactory->Create(transportClient));
  }
  // receiving
  {
    auto it = this->pimpl->sessions.begin();
    while (it != this->pimpl->sessions.end()) {
      const std::shared_ptr<ISession>& session = *it;
      if (!session->Receive()) {
        session->Close();
        it = this->pimpl->sessions.erase(it);
        continue;
      }
      ++it;
    }
  }
  // processing
  // sending
  {
    auto it = this->pimpl->sessions.begin();
    while (it != this->pimpl->sessions.end()) {
      const std::shared_ptr<ISession>& session = *it;
      if (!session->Send()) {
        session->Close();
        it = this->pimpl->sessions.erase(it);
        continue;
      }
      ++it;
    }
  }
  return true;
}

}
}
