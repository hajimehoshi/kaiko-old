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

class MockTransportClient : public Kaiko::IPC::ITransportClient,
                            private boost::noncopyable {
public:
  MockTransportClient()
    : receivedDataCollectionIndex(-1), isClosed(false) {
  }
  void
  Close() {
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
    if (this->isClosed) {
      return false;
    }
    if (this->receivedDataCollectionIndex < static_cast<int>(this->receivedDataCollection.size())) {
      ++this->receivedDataCollectionIndex;
    }
    return true;
  }
  bool
  Send(const std::string& data) {
    if (this->isClosed) {
      return false;
    }
    this->sentData.append(data);
    return true;
  }
  std::string sentData;
  std::vector<const std::string> receivedDataCollection;
  int receivedDataCollectionIndex;
  bool isClosed;
};

class MockTransportServer : public Kaiko::IPC::ITransportServer,
                            private boost::noncopyable {
public:
  MockTransportServer()
    : isClosed(false) {
  }
  bool
  Accept() {
    if (this->isClosed) {
      return false;
    }
    return true;
  }
  void
  Close() {
    this->isClosed = true;
  }
  const std::shared_ptr<Kaiko::IPC::ITransportClient>&
  GetLastAcceptedClient() const {
    return this->lastAcceptedClient;
  }
  bool isClosed;
  std::shared_ptr<Kaiko::IPC::ITransportClient> lastAcceptedClient;
};

class MockSession : public Kaiko::IPC::ISession,
                    private boost::noncopyable {
public:
  explicit MockSession(const std::shared_ptr<Kaiko::IPC::ITransportClient>& transportClient)
    : transportClient(transportClient), receivedDataCollectionIndex(-1), isClosed(false) {
  }
  void
  AddDataToSend(const std::string&) {
  }
  void
  Close() {
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
    if (this->isClosed) {
      return false;
    }
    if (this->receivedDataCollectionIndex < static_cast<int>(this->receivedDataCollection.size())) {
      ++this->receivedDataCollectionIndex;
    }
    return true;
  }
  bool
  Send() {
    if (this->isClosed) {
      return false;
    }
    //this->sentDataCollection.push_back(data);
    return true;
  }
  const std::shared_ptr<Kaiko::IPC::ITransportClient> transportClient;
  std::vector<const std::string> sentDataCollection;
  std::vector<const std::string> receivedDataCollection;
  int receivedDataCollectionIndex;
  bool isClosed;
};

class MockSessionFactory : public Kaiko::IPC::ISessionFactory,
                           private boost::noncopyable {
public:
  std::shared_ptr<Kaiko::IPC::ISession>
  Create(const std::shared_ptr<Kaiko::IPC::ITransportClient>& transportClient) {
    return this->lastCreatedSession = std::shared_ptr<Kaiko::IPC::ISession>(new MockSession(transportClient));
  }
  std::shared_ptr<Kaiko::IPC::ISession> lastCreatedSession;
};

}

}
}

#endif
