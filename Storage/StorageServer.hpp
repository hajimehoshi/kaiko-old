/*
 * From a client to a server
 *   Subcribe
 *     Name
 *   Create
 *     Name
 *   Read
 *     Name
 *     Keys
 *   Update
 *     Name
 *     Pairs
 *   Delete
 *     Name
 *
 * From a server to a client
 *   Data
 *     Name
 *   Error
 *     Type
 *
 */

#ifndef KAIKO_STORAGE_STORAGESERVER_HPP
#define KAIKO_STORAGE_STORAGESERVER_HPP

#include "../IPC/ISessionFactory.hpp"
#include "../IPC/ITransportServer.hpp"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace Kaiko {
namespace Storage {

class StorageServer : private boost::noncopyable {
public:
  StorageServer();
  void Run(boost::shared_ptr<IPC::ITransportServer> transportServer,
           boost::shared_ptr<IPC::ISessionFactory> sessionFactory);
private:
  struct Impl;
  boost::scoped_ptr<Impl> pimpl;
};

}
}

#endif
