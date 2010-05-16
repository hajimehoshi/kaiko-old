#ifndef KAIKO_STORAGE_ISTORAGEMESSAGEPROCESSOR
#define KAIKO_STORAGE_ISTORAGEMESSAGEPROCESSOR

#include <memory>
#include <string>

namespace Kaiko {
namespace Storage {

class IStorageMessageProcessor {
public:
  virtual ~IStorageMessageProcessor() { }
  virtual bool Process(const std::string& message) = 0;
};

}
}

#endif
