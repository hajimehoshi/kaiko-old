#ifndef KAIKO_IPC_NAMED_PIPE_HPP
#define KAIKO_IPC_NAMED_PIPE_HPP

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>

namespace Kaiko {
namespace IPC {

class NamedPipe : private boost::noncopyable {
public:
  NamedPipe(const std::string& name, bool isServer);
  ~NamedPipe() throw();
  void Close() throw();
  bool Connect();
  bool Disconnect();
  const std::string& GetLastReceivedData() const;
  bool Receive();
  bool Send(const std::string& data);
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};

}
}

#endif

