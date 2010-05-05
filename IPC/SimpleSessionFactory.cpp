#include "SimpleSessionFactory.hpp"

#include "SimpleSession.hpp"

namespace Kaiko {
namespace IPC {

boost::shared_ptr<ISession>
SimpleSessionFactory::Create(boost::shared_ptr<ITransportClient> transportClient) {
  return boost::shared_ptr<ISession>(new SimpleSession(transportClient));
}

}
}

