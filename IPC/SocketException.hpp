#ifndef KAIKO_IPC_SOCKETEXEPTION_HPP
#define KAIKO_IPC_SOCKETEXEPTION_HPP

#include "../Util/SystemException.hpp"

namespace Kaiko {
namespace IPC {

class SocketException : public Util::SystemException {
public:
  SocketException(const std::string& file, int line);
  virtual ~SocketException();
};

}
}

#endif
