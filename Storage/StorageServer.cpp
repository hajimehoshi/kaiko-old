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
  typedef boost::unordered_set< boost::shared_ptr<IPC::ISession> > Sessions;
  Sessions sessions;
};

StorageServer::StorageServer()
  : pimpl(new Impl()) {
}

void
StorageServer::Run(boost::shared_ptr<IPC::ITransportServer> transportServer,
                   boost::shared_ptr<IPC::ISessionFactory> sessionFactory) {
  assert(transportServer);
  while (true) {
    try {
      if (!transportServer->Accept()) {
        break;
      }
    } catch (const Util::SystemException&) {
      // TODO: logging
      break;
    }
    boost::shared_ptr<IPC::ITransportClient> transportClient = transportServer->GetLastAcceptedClient();
    if (transportClient) {
      this->pimpl->sessions.insert(sessionFactory->Create(transportClient));
    }
    Impl::Sessions::iterator it = this->pimpl->sessions.begin();
    while (it != this->pimpl->sessions.end()) {
      boost::shared_ptr<IPC::ISession> session = *it;
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
