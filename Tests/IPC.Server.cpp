#include "../IPC/Server.hpp"

#include "MockObjects.hpp"

#include <boost/test/unit_test.hpp>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::IPC;

BOOST_AUTO_TEST_CASE(IPC_Server_Process) {
  {
    const std::shared_ptr<IPC::MockTransportServer> transportServer(new IPC::MockTransportServer());
    const std::shared_ptr<IPC::MockSessionFactory> sessionFactory(new IPC::MockSessionFactory());
    const std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    Server server(transportServer, sessionFactory);
    transportServer->lastAcceptedClient = transportClient;
    BOOST_CHECK_EQUAL(true, server.Process());
    transportServer->lastAcceptedClient.reset();
    const auto session = std::static_pointer_cast<IPC::MockSession>(sessionFactory->lastCreatedSession);
    BOOST_CHECK_EQUAL(transportClient, session->transportClient);
    session->Close();
    BOOST_CHECK_EQUAL(true, server.Process());
  }
}

}
}
