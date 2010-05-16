#ifndef KAIKO_IPC_ISESSIONFACTORY_HPP
#define KAIKO_IPC_ISESSIONFACTORY_HPP

#include "ISession.hpp"
#include "ITransportClient.hpp"

#include <memory>

namespace Kaiko {
namespace IPC {

class ISessionFactory {
public:
  virtual ~ISessionFactory() { }
  virtual std::shared_ptr<ISession> Create(const std::shared_ptr<ITransportClient>& transportClient) = 0;
};

}
}

#endif
