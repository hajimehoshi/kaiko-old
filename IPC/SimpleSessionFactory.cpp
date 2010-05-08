#include "SimpleSessionFactory.hpp"

#include "SimpleSession.hpp"

namespace Kaiko {
namespace IPC {

std::shared_ptr<ISession>
SimpleSessionFactory::Create(std::shared_ptr<ITransportClient> transportClient) {
  return std::shared_ptr<ISession>(new SimpleSession(transportClient));
}

}
}

