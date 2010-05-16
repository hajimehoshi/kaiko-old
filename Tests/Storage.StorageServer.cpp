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
    storageServer.Execute();
  }
}

}
}
