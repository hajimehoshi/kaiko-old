#include "../IPC/SimpleSession.hpp"

#include "MockObjects.hpp"

#include <boost/test/unit_test.hpp>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::IPC;

BOOST_AUTO_TEST_CASE(IPC_SimpleSession_Send) {
  {
    // empty
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    BOOST_CHECK_EQUAL(true, session.Send(""));
    BOOST_CHECK_EQUAL(true, transportClient->sentData.empty());
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(127, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x7f', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(2));
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(128, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x81', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL('\x00', transportClient->sentData.at(2));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(3));
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(129, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x81', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL('\x01', transportClient->sentData.at(2));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(3));
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(200, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x81', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL('\x48', transportClient->sentData.at(2));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(3));
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(314159, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x93', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL('\x96', transportClient->sentData.at(2));
    BOOST_CHECK_EQUAL('\x2f', transportClient->sentData.at(3));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(4));
  }
}

BOOST_AUTO_TEST_CASE(IPC_SimpleSession_SendContinuously) {
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    BOOST_CHECK_EQUAL(true, session.Send("foo"));
    BOOST_CHECK_EQUAL(true, session.Send("barbaz"));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x03', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL("foo",  transportClient->sentData.substr(2, 3));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(5));
    BOOST_CHECK_EQUAL('\x06', transportClient->sentData.at(6));
    BOOST_CHECK_EQUAL("barbaz",  transportClient->sentData.substr(7));
  }
}

BOOST_AUTO_TEST_CASE(IPC_SimpleSession_Receive) {
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    std::string data(127, 'a');
    transportClient->receivedDataCollection.push_back(std::string("\x80\x7f") + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(128, 'a');
    std::string header("\x80\x81");
    header.push_back('\0');
    transportClient->receivedDataCollection.push_back(header + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(129, 'a');
    const std::string header("\x80\x81\x01");
    transportClient->receivedDataCollection.push_back(header + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(200, 'a');
    const std::string header("\x80\x81\x48");
    transportClient->receivedDataCollection.push_back(header + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(314159, 'a');
    const std::string header("\x80\x93\x96\x2f");
    transportClient->receivedDataCollection.push_back(header + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
}

BOOST_AUTO_TEST_CASE(IPC_SimpleSession_ReceiveContinuously) {
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->receivedDataCollection.push_back("\x80\x03" "foo" "\x80\x06" "barbaz");
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("foo", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("barbaz", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->receivedDataCollection.push_back("\x80\x03" "f");
    transportClient->receivedDataCollection.push_back("");
    transportClient->receivedDataCollection.push_back("oo" "\x80");
    transportClient->receivedDataCollection.push_back("");
    transportClient->receivedDataCollection.push_back("");
    transportClient->receivedDataCollection.push_back("\x06" "barbaz");
    do {
      BOOST_CHECK_EQUAL(true, session.Receive());
    } while (session.GetLastReceivedData().empty());
    BOOST_CHECK_EQUAL("foo", session.GetLastReceivedData());
    do {
      BOOST_CHECK_EQUAL(true, session.Receive());
    } while (session.GetLastReceivedData().empty());
    BOOST_CHECK_EQUAL("barbaz", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(4096 * 100, 'a');
    std::string header("\x80\x99\x80");
    header.push_back('\x00');
    transportClient->receivedDataCollection.push_back(header);
    for (int i = 0; i < 100; ++i) {
      transportClient->receivedDataCollection.push_back(std::string(4096, 'a'));
    }
    do {
      BOOST_CHECK_EQUAL(true, session.Receive());
    } while (session.GetLastReceivedData().empty());
    BOOST_CHECK_EQUAL(data.size(), session.GetLastReceivedData().size());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
}

BOOST_AUTO_TEST_CASE(IPC_SimpleSession_ReceiveInvalidBytes) {
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->receivedDataCollection.push_back("\xff");
    BOOST_CHECK_EQUAL(false, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, transportClient->isClosed);
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->receivedDataCollection.push_back("\x80\x03" "foo" "\xff");
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("foo", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, transportClient->isClosed);
  }
  {
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->receivedDataCollection.push_back("\x80\xff\xff\xff\xff\xff");
    BOOST_CHECK_EQUAL(false, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, transportClient->isClosed);
  }
  {
    // empty data
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    std::string bytes("\x80");
    bytes.push_back('\x00');
    bytes.append("\x80\x03" "foo");
    transportClient->receivedDataCollection.push_back(bytes);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("foo", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    // empty data (redundant bytes)
    std::shared_ptr<IPC::MockTransportClient> transportClient(new IPC::MockTransportClient());
    SimpleSession session(transportClient);
    std::string bytes("\x80\x80\x80");
    bytes.push_back('\x00');
    bytes.append("\x80\x03" "foo");
    transportClient->receivedDataCollection.push_back(bytes);
    BOOST_CHECK_EQUAL(false, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, transportClient->isClosed);
  }
}

}
}
