#include "../Storage/StorageServer.hpp"

#include <boost/test/unit_test.hpp>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::Storage;

BOOST_AUTO_TEST_CASE(Storage_StorageServer_Run) {
  {
    StorageServer storageServer(nullptr, nullptr);
  }
}

}
}
