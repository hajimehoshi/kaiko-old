#ifndef KAIKO_IPC_ISESSION_HPP
#define KAIKO_IPC_ISESSION_HPP

#include <string>

namespace Kaiko {
namespace IPC {

class ISession {
public:
  virtual ~ISession() { }
  virtual void Close() throw() = 0;
  virtual const std::string& GetLastReceivedData() const = 0;
  virtual bool Receive() = 0;
  virtual bool Send(const std::string& data) = 0;
};

}
}

#endif
