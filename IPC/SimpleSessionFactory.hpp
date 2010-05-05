#ifndef KAIKO_IPC_SIMPLESESSIONFACTORY_HPP
#define KAIKO_IPC_SIMPLESESSIONFACTORY_HPP

#include "ISession.hpp"
#include "ISessionFactory.hpp"
#include "ITransportClient.hpp"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace Kaiko {
namespace IPC {

class SimpleSessionFactory : public ISessionFactory,
                             private boost::noncopyable {
public:
  boost::shared_ptr<ISession> Create(boost::shared_ptr<ITransportClient> transportClient);
};

}
}

#endif
