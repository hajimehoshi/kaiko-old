#include "StorageServer.hpp"

#include "../IPC/ISession.hpp"
#include "../Util/SystemException.hpp"

//#include <boost/property_tree/json_parser.hpp>
//#include <boost/property_tree/ptree.hpp>
#include <boost/unordered_set.hpp>
#include <cassert>
#include <sstream>

namespace Kaiko {
namespace Storage {

struct StorageServer::Impl {
  typedef boost::unordered_set<std::shared_ptr<IPC::ISession>> Sessions;
  Sessions sessions;
};

StorageServer::StorageServer()
  : pimpl(new Impl()) {
}

void
StorageServer::Run(std::shared_ptr<IPC::ITransportServer> transportServer,
                   std::shared_ptr<IPC::ISessionFactory> sessionFactory) {
  assert(transportServer);
  for (;;) {
    try {
      if (!transportServer->Accept()) {
        break;
      }
    } catch (const Util::SystemException&) {
      // TODO: logging
      break;
    }
    auto transportClient = transportServer->GetLastAcceptedClient();
    if (transportClient) {
      this->pimpl->sessions.insert(sessionFactory->Create(transportClient));
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
      std::istringstream iss(data);
      //boost::property_tree::ptree tree;
      //read_json(iss, tree);
      ++it;
    }
  }
}

}
}
