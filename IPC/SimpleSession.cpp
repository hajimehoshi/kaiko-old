#include "SimpleSession.hpp"

#include "../Util/Serialization.hpp"
#include "../Util/SystemException.hpp"

#include <cassert>
#include <deque>

namespace Kaiko {
namespace IPC {

enum ReceivingState {
  ReceivingStateInit,
  ReceivingStateLength,
  ReceivingStateData,
  ReceivingStateTerminated,
};

struct SimpleSession::Impl {
  explicit Impl(std::shared_ptr<ITransportClient> transportClient)
    : transportClient(transportClient),
      receivingState(ReceivingStateInit),
      isActive(true),
      restLengthToRead(0) {
  }
  void Inactivate() {
    this->transportClient->Close();
    this->lastReceivedData.clear();
    this->isActive = false;
    this->receivingState = ReceivingStateTerminated;
  }
  std::shared_ptr<ITransportClient> transportClient;
  std::deque<char> bufferedChars;
  bool isActive;
  ReceivingState receivingState;
  std::string lastReceivedData;
  int restLengthToRead;
};

SimpleSession::SimpleSession(std::shared_ptr<ITransportClient> transportClient)
  : pimpl(new Impl(transportClient)) {
}

SimpleSession::~SimpleSession() throw() {
  this->Close();
}

void
SimpleSession::Close() throw() {
  this->pimpl->Inactivate();
}

const std::string&
SimpleSession::GetLastReceivedData() const {
  return this->pimpl->lastReceivedData;
}

bool
SimpleSession::Receive() {
  if (!this->pimpl->isActive) {
    return false;
  }
  try {
    if (!this->pimpl->transportClient->Receive()) {
      goto Failed;
    }
  } catch (const Util::SystemException&) {
    // TODO: logging
    goto Failed;
  }
  const std::string& receivedData = this->pimpl->transportClient->GetLastReceivedData();
  this->pimpl->bufferedChars.insert(this->pimpl->bufferedChars.end(),
                                    receivedData.begin(),
                                    receivedData.end());
  for (;;) {
    if (this->pimpl->bufferedChars.empty()) {
      this->pimpl->lastReceivedData.clear();
      return true;
    }
    assert(!this->pimpl->bufferedChars.empty());
    switch (this->pimpl->receivingState) {
    case ReceivingStateInit:
      {
        assert(!this->pimpl->restLengthToRead);
        const char ch = this->pimpl->bufferedChars.front();
        this->pimpl->bufferedChars.pop_front();
        if (ch == '\x80') {
          this->pimpl->receivingState = ReceivingStateLength;
        } else {
          // invalid header
          // TODO: logging
          goto Failed;
        }
      }
      break;
    case ReceivingStateLength:
      {
        assert(!this->pimpl->restLengthToRead);
        int length;
        int readBytesNum;
        try {
          std::tie(length, readBytesNum) = Util::Serialization::BytesToLength(this->pimpl->bufferedChars.begin(),
                                                                              this->pimpl->bufferedChars.end());
        } catch (const Util::Exception&) {
          // TODO: logging
          goto Failed;
        }
        if (readBytesNum) {
          assert(0 <= length);
          assert(readBytesNum <= static_cast<int>(this->pimpl->bufferedChars.size()));
          this->pimpl->restLengthToRead = length;
          this->pimpl->bufferedChars.erase(this->pimpl->bufferedChars.begin(),
                                           this->pimpl->bufferedChars.begin() + readBytesNum);
          if (length) {            
            this->pimpl->receivingState = ReceivingStateData;
          } else {
            // empty size
            // TODO: logging
            this->pimpl->receivingState = ReceivingStateInit;
          }
        } else {
          return true;
        }
      }
      break;
    case ReceivingStateData:
      {
        assert(0 < static_cast<int>(this->pimpl->bufferedChars.size()));
        assert(0 < this->pimpl->restLengthToRead);
        if (static_cast<int>(this->pimpl->bufferedChars.size()) < this->pimpl->restLengthToRead) {
          this->pimpl->lastReceivedData.clear();
          return true;
        }
        auto dataBegin = this->pimpl->bufferedChars.begin();
        auto dataEnd   = this->pimpl->bufferedChars.begin() + this->pimpl->restLengthToRead;
        this->pimpl->lastReceivedData.swap(std::string(dataBegin, dataEnd));
        this->pimpl->bufferedChars.erase(dataBegin, dataEnd);
        this->pimpl->receivingState = ReceivingStateInit;
        this->pimpl->restLengthToRead = 0;
        return true;
      }
      break;
    case ReceivingStateTerminated:
      assert(0);
      break;
    default:
      assert(0);
      break;
    }
  }
 Failed:
  this->pimpl->Inactivate();
  return false;
}

bool
SimpleSession::Send(const std::string& data) {
  if (!this->pimpl->isActive) {
    return false;
  }
  if (data.empty()) {
    return true;
  }
  int size = data.size();
  if (size <= 0) {
    throw Util::Exception(__FILE__, __LINE__, "data size is too big");
  }
  const std::string lengthPart = Util::Serialization::LengthToBytes(size);
  try {
    if (this->pimpl->transportClient->Send("\x80") &&
        this->pimpl->transportClient->Send(lengthPart) &&
        this->pimpl->transportClient->Send(data)) {
      return true;
    }
  } catch (const Util::SystemException&) {
    // TODO: logging
  }
  this->pimpl->Inactivate();
  return false;
}

}
}
