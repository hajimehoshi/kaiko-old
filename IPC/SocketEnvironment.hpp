#ifndef KAIKO_IPC_SOCKETENVIRONMENT_HPP
#define KAIKO_IPC_SOCKETENVIRONMENT_HPP

#include <boost/noncopyable.hpp>

namespace Kaiko {
namespace IPC {

class SocketEnvironment : private boost::noncopyable {
public:
  SocketEnvironment();
  ~SocketEnvironment();
};

}
}

#endif
