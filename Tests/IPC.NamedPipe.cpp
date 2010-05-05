#include "../IPC/NamedPipe.hpp"
#include "../Util/SystemException.hpp"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

namespace Kaiko {
namespace IPC {

using namespace Kaiko::IPC;

class ConnectFunc {
public:
  typedef void result_type;
  result_type operator()(NamedPipe& namedPipeServer) {
    try {
      namedPipeServer.Connect();
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }
};

BOOST_AUTO_TEST_CASE(IPC_NamedPipe) {
  {
    /*NamedPipe namedPipeServer("kaiko.test.server", true);
    boost::thread connector(boost::bind(ConnectFunc(), boost::ref(namedPipeServer)));
    NamedPipe namedPipeClient("kaiko.test.server", false);
    connector.join();
    BOOST_CHECK_EQUAL(true, namedPipeClient.Send("test"));*/
  }
}

}
}

