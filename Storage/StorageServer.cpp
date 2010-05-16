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
       const std::shared_ptr<IPC::ISessionFactory>& sessionFactory,
       const std::shared_ptr<IStorageMessageProcessor>& storageMessageProcessor)
    : transportServer(transportServer),
      sessionFactory(sessionFactory),
      storageMessageProcessor(storageMessageProcessor) {
  }
  const std::shared_ptr<IPC::ITransportServer> transportServer;
  const std::shared_ptr<IPC::ISessionFactory> sessionFactory;
  const std::shared_ptr<IStorageMessageProcessor> storageMessageProcessor;
  Sessions sessions;
};

StorageServer::StorageServer(const std::shared_ptr<IPC::ITransportServer>& transportServer,
                             const std::shared_ptr<IPC::ISessionFactory>& sessionFactory,
                             const std::shared_ptr<IStorageMessageProcessor>& storageMessageProcessor)
  : pimpl(new Impl(transportServer, sessionFactory, storageMessageProcessor)) {
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
  // receiving
  {
    auto it = this->pimpl->sessions.begin();
    while (it != this->pimpl->sessions.end()) {
      const std::shared_ptr<IPC::ISession>& session = *it;
      if (!session->Receive()) {
        goto Fail;
      }
      const std::string& data = session->GetLastReceivedData();
      if (!this->pimpl->storageMessageProcessor->Process(data)) {
        goto Fail;
      }
      ++it;
      continue;
    Fail:
      session->Close();
      it = this->pimpl->sessions.erase(it);
      continue;
    }
  }
  // sending
  {
    auto it = this->pimpl->sessions.begin();
    while (it != this->pimpl->sessions.end()) {
      const std::shared_ptr<IPC::ISession>& session = *it;
      if (!session->Send()) {
        session->Close();
        it = this->pimpl->sessions.erase(it);
        continue;
      }
      ++it;
      continue;
    }
  }
  return true;
}

}
}
