#include "../IPC/SimpleSession.hpp"

#include <boost/test/unit_test.hpp>
#include <vector>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::IPC;

class MockTransportClient : public ITransportClient,
                            private boost::noncopyable {
public:
  MockTransportClient()
    : lastReceivedDataCollectionIndex(-1), isClosed(false) {
  }
  void Close() throw() {
    this->isClosed = true;
  }
  const std::string& GetLastReceivedData() const {
    if (this->lastReceivedDataCollectionIndex < static_cast<int>(this->lastReceivedDataCollection.size())) {
      const std::string& result = this->lastReceivedDataCollection.at(this->lastReceivedDataCollectionIndex);
      return result;
    } else {
      static const std::string emptyStr;
      return emptyStr;
    }    
  }
  bool Receive() {
    if (this->lastReceivedDataCollectionIndex < static_cast<int>(this->lastReceivedDataCollection.size())) {
      ++this->lastReceivedDataCollectionIndex;
    }
    return true;
  }
  bool Send(const std::string& data) {
    this->sentData.append(data);
    return true;
  }
  std::string sentData;
  std::vector<const std::string> lastReceivedDataCollection;
  int lastReceivedDataCollectionIndex;
  bool isClosed;
};

BOOST_AUTO_TEST_CASE(IPC_SimpleSession_Send) {
  {
    // empty
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    BOOST_CHECK_EQUAL(true, session.Send(""));
    BOOST_CHECK_EQUAL(true, transportClient->sentData.empty());
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(127, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x7f', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(2));
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(128, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x81', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL('\x00', transportClient->sentData.at(2));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(3));
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(129, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x81', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL('\x01', transportClient->sentData.at(2));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(3));
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data = std::string(200, 'a');
    BOOST_CHECK_EQUAL(true, session.Send(data));
    BOOST_CHECK_EQUAL('\x80', transportClient->sentData.at(0));
    BOOST_CHECK_EQUAL('\x81', transportClient->sentData.at(1));
    BOOST_CHECK_EQUAL('\x48', transportClient->sentData.at(2));
    BOOST_CHECK_EQUAL(data, transportClient->sentData.substr(3));
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
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
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
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
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    std::string data(127, 'a');
    transportClient->lastReceivedDataCollection.push_back(std::string("\x80\x7f") + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(128, 'a');
    std::string header("\x80\x81");
    header.push_back('\0');
    transportClient->lastReceivedDataCollection.push_back(header + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(129, 'a');
    const std::string header("\x80\x81\x01");
    transportClient->lastReceivedDataCollection.push_back(header + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(200, 'a');
    const std::string header("\x80\x81\x48");
    transportClient->lastReceivedDataCollection.push_back(header + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(314159, 'a');
    const std::string header("\x80\x93\x96\x2f");
    transportClient->lastReceivedDataCollection.push_back(header + data);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL(data, session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
}

BOOST_AUTO_TEST_CASE(IPC_SimpleSession_ReceiveContinuously) {
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->lastReceivedDataCollection.push_back("\x80\x03" "foo" "\x80\x06" "barbaz");
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("foo", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("barbaz", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->lastReceivedDataCollection.push_back("\x80\x03" "f");
    transportClient->lastReceivedDataCollection.push_back("");
    transportClient->lastReceivedDataCollection.push_back("oo" "\x80");
    transportClient->lastReceivedDataCollection.push_back("");
    transportClient->lastReceivedDataCollection.push_back("");
    transportClient->lastReceivedDataCollection.push_back("\x06" "barbaz");
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
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    const std::string data(4096 * 100, 'a');
    std::string header("\x80\x99\x80");
    header.push_back('\x00');
    transportClient->lastReceivedDataCollection.push_back(header);
    for (int i = 0; i < 100; ++i) {
      transportClient->lastReceivedDataCollection.push_back(std::string(4096, 'a'));
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
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->lastReceivedDataCollection.push_back("\xff");
    BOOST_CHECK_EQUAL(false, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, transportClient->isClosed);
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->lastReceivedDataCollection.push_back("\x80\x03" "foo" "\xff");
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("foo", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, transportClient->isClosed);
  }
  {
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    transportClient->lastReceivedDataCollection.push_back("\x80\xff\xff\xff\xff\xff");
    BOOST_CHECK_EQUAL(false, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, transportClient->isClosed);
  }
  {
    // empty data
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    std::string bytes("\x80");
    bytes.push_back('\x00');
    bytes.append("\x80\x03" "foo");
    transportClient->lastReceivedDataCollection.push_back(bytes);
    BOOST_CHECK_EQUAL(true, session.Receive());
    BOOST_CHECK_EQUAL("foo", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(false, transportClient->isClosed);
  }
  {
    // empty data (redundant bytes)
    boost::shared_ptr<MockTransportClient> transportClient(new MockTransportClient());
    SimpleSession session(transportClient);
    std::string bytes("\x80\x80\x80");
    bytes.push_back('\x00');
    bytes.append("\x80\x03" "foo");
    transportClient->lastReceivedDataCollection.push_back(bytes);
    BOOST_CHECK_EQUAL(false, session.Receive());
    BOOST_CHECK_EQUAL("", session.GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, transportClient->isClosed);
  }
}

}
}
