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
#include <memory>

namespace Kaiko {
namespace Storage {

class StorageServer : private boost::noncopyable {
public:
  StorageServer();
  void Run(std::shared_ptr<IPC::ITransportServer> transportServer,
           std::shared_ptr<IPC::ISessionFactory> sessionFactory);
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};

}
}

#endif
