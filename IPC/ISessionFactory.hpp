#ifndef KAIKO_IPC_ISESSIONFACTORY_HPP
#define KAIKO_IPC_ISESSIONFACTORY_HPP

#include "ISession.hpp"
#include "ITransportClient.hpp"

#include <boost/shared_ptr.hpp>

namespace Kaiko {
namespace IPC {

class ISessionFactory {
public:
  virtual ~ISessionFactory() { }
  virtual boost::shared_ptr<ISession> Create(boost::shared_ptr<ITransportClient> transportClient) = 0;
};

}
}

#endif
