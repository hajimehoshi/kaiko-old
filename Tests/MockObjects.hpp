#ifndef KAIKO_TEST_MOCKOBJECTS_HPP
#define KAIKO_TEST_MOCKOBJECTS_HPP

#include "../IPC/ISession.hpp"
#include "../IPC/ISessionFactory.hpp"
#include "../IPC/ITransportClient.hpp"
#include "../IPC/ITransportServer.hpp"

#include <vector>

namespace Kaiko {
namespace Tests {

namespace IPC {

using namespace Kaiko::IPC;

class MockTransportClient : public ITransportClient,
                            private boost::noncopyable {
public:
  MockTransportClient()
    : receivedDataCollectionIndex(-1), isClosed(false) {
  }
  void
  Close() throw() {
    this->isClosed = true;
  }
  const std::string&
  GetLastReceivedData() const {
    if (this->receivedDataCollectionIndex < static_cast<int>(this->receivedDataCollection.size())) {
      const std::string& result = this->receivedDataCollection.at(this->receivedDataCollectionIndex);
      return result;
    } else {
      static const std::string emptyStr;
      return emptyStr;
    }
  }
  bool
  Receive() {
    if (this->receivedDataCollectionIndex < static_cast<int>(this->receivedDataCollection.size())) {
      ++this->receivedDataCollectionIndex;
    }
    return true;
  }
  bool
  Send(const std::string& data) {
    this->sentData.append(data);
    return true;
  }
  std::string sentData;
  std::vector<const std::string> receivedDataCollection;
  int receivedDataCollectionIndex;
  bool isClosed;
};

class MockTransportServer : public ITransportServer,
                            private boost::noncopyable {
public:
  MockTransportServer()
    : isClosed(false) {
  }
  bool
  Accept() {
    return this->lastAcceptedClient;
  }
  void
  Close() throw() {
    this->isClosed = true;
  }
  const std::shared_ptr<IPC::ITransportClient>&
  GetLastAcceptedClient() const {
    return this->lastAcceptedClient;
  }
  bool isClosed;
  std::shared_ptr<IPC::ITransportClient> lastAcceptedClient;
};

class MockSession : public ISession,
                    private boost::noncopyable {
public:
  MockSession(std::shared_ptr<ITransportClient> transportClient)
    : transportClient(transportClient), receivedDataCollectionIndex(-1), isClosed(false) {
  }
  void
  Close() throw() {
    this->isClosed = true;
  }
  const std::string&
  GetLastReceivedData() const {
    if (this->receivedDataCollectionIndex < static_cast<int>(this->receivedDataCollection.size())) {
      const std::string& result = this->receivedDataCollection.at(this->receivedDataCollectionIndex);
      return result;
    } else {
      static const std::string emptyStr;
      return emptyStr;
    }
  }
  bool
  Receive() {
    if (this->receivedDataCollectionIndex < static_cast<int>(this->receivedDataCollection.size())) {
      ++this->receivedDataCollectionIndex;
    }
    return true;
  }
  bool
  Send(const std::string& data) {
    this->sentDataCollection.push_back(data);
    return true;
  }
  const std::shared_ptr<ITransportClient> transportClient;
  std::vector<const std::string> sentDataCollection;
  std::vector<const std::string> receivedDataCollection;
  int receivedDataCollectionIndex;
  bool isClosed;
};

class MockSessionFactory : public ISessionFactory,
                           private boost::noncopyable {
public:
  std::shared_ptr<IPC::ISession>
  Create(const std::shared_ptr<ITransportClient>& transportClient) {
    return std::shared_ptr<IPC::ISession>(new MockSession(transportClient));
  }
};

}

}
}

#endif
