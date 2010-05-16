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

#include "IStorageMessageProcessor.hpp"
#include "../IPC/ISessionFactory.hpp"
#include "../IPC/ITransportServer.hpp"

#include <boost/noncopyable.hpp>
#include <memory>

namespace Kaiko {
namespace Storage {

class StorageServer : private boost::noncopyable {
public:
  StorageServer(const std::shared_ptr<IPC::ITransportServer>& transportServer,
                const std::shared_ptr<IPC::ISessionFactory>& sessionFactory,
                const std::shared_ptr<IStorageMessageProcessor>& storageMessageProcessor);
  ~StorageServer() throw();
  bool Execute();
private:
  struct Impl;
  const std::unique_ptr<Impl> pimpl;
};

}
}

#endif
