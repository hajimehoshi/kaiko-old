#ifndef KAIKO_IPC_SIMPLESESSIONFACTORY_HPP
#define KAIKO_IPC_SIMPLESESSIONFACTORY_HPP

#include "ISession.hpp"
#include "ISessionFactory.hpp"
#include "ITransportClient.hpp"

#include <boost/noncopyable.hpp>
#include <memory>

namespace Kaiko {
namespace IPC {

class SimpleSessionFactory : public ISessionFactory,
                             private boost::noncopyable {
public:
  std::shared_ptr<ISession> Create(std::shared_ptr<ITransportClient> transportClient);
};

}
}

#endif
