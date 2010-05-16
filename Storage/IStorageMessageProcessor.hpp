#ifndef KAIKO_STORAGE_ISTORAGEMESSAGEPROCESSOR_HPP
#define KAIKO_STORAGE_ISTORAGEMESSAGEPROCESSOR_HPP

#include "../IPC/ISession.hpp"

#include <memory>

namespace Kaiko {
namespace Storage {

class IStorageMessageProcessor {
public:
  virtual ~IStorageMessageProcessor() { }
  virtual void Process(const std::shared_ptr<IPC::ISession>& session) = 0;
};

}
}

#endif
