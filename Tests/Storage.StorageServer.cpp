#include "../Storage/StorageServer.hpp"
#include "MockObjects.hpp"

#include <boost/test/unit_test.hpp>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::Storage;

BOOST_AUTO_TEST_CASE(Storage_StorageServer_Execute) {
  {
    std::shared_ptr<IPC::MockTransportServer> transportServer(new IPC::MockTransportServer());
    std::shared_ptr<IPC::MockSessionFactory> sessionFactory(new IPC::MockSessionFactory());
    StorageServer storageServer(transportServer, sessionFactory);
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    transportServer->lastAcceptedClient = transportClient;
    BOOST_CHECK_EQUAL(true, storageServer.Execute());
    transportServer->lastAcceptedClient.reset();
    std::shared_ptr<IPC::MockSession> session = std::static_pointer_cast<IPC::MockSession>(sessionFactory->lastCreatedSession);
    BOOST_CHECK_EQUAL(transportClient, session->transportClient);
    session->Close();
    BOOST_CHECK_EQUAL(true, storageServer.Execute());
  }
}

}
}
