#include "StorageServer.hpp"

#include "../IPC/ISession.hpp"
#include "../Util/SystemException.hpp"

#include <boost/unordered_set.hpp>
#include <cassert>
#include <sstream>

namespace Kaiko {
namespace Storage {

struct StorageServer::Impl : private boost::noncopyable {
  typedef boost::unordered_set<std::shared_ptr<IPC::ISession>> Sessions;
  Impl(const std::shared_ptr<IPC::ITransportServer>& transportServer,
       const std::shared_ptr<IPC::ISessionFactory>& sessionFactory)
    : transportServer(transportServer), sessionFactory(sessionFactory) {
  }
  const std::shared_ptr<IPC::ITransportServer> transportServer;
  const std::shared_ptr<IPC::ISessionFactory> sessionFactory;
  Sessions sessions;
};

StorageServer::StorageServer(const std::shared_ptr<IPC::ITransportServer>& transportServer,
                             const std::shared_ptr<IPC::ISessionFactory>& sessionFactory)
  : pimpl(new Impl(transportServer, sessionFactory)) {
}

StorageServer::~StorageServer() throw() {
}

bool
StorageServer::Execute() {
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
  auto it = this->pimpl->sessions.begin();
  while (it != this->pimpl->sessions.end()) {
    std::shared_ptr<IPC::ISession> session = *it;
    if (!session->Receive()) {
      session->Close();
      it = this->pimpl->sessions.erase(it);
      continue;
    }
    const std::string& data = session->GetLastReceivedData();
    ++it;
  }
  return true;
}

}
}
