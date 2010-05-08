#include "../IPC/SocketEnvironment.hpp"
#include "../IPC/SocketException.hpp"
#include "../IPC/SocketServer.hpp"

#include <boost/test/unit_test.hpp>
#include <windows.h>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::IPC;

BOOST_AUTO_TEST_CASE(IPC_SocketServer) {
  SocketEnvironment socketEnvironment;
  SocketServer server;

  BOOST_CHECK_EQUAL(true, server.Accept());
  BOOST_CHECK(!server.GetLastAcceptedClient());

  SocketClient clientSocket1("127.0.0.1", server.GetPort());
  SocketClient clientSocket2("127.0.0.1", server.GetPort());
  SocketClient clientSocket3("127.0.0.1", server.GetPort());

  std::shared_ptr<ITransportClient> clientSocketInServer1;
  std::shared_ptr<ITransportClient> clientSocketInServer2;
  std::shared_ptr<ITransportClient> clientSocketInServer3;
  std::shared_ptr<ITransportClient> clientSocketInServer4;

  do {
    BOOST_CHECK_EQUAL(true, server.Accept());
    clientSocketInServer1 = server.GetLastAcceptedClient();
  } while (!clientSocketInServer1);
  do {
    BOOST_CHECK_EQUAL(true, server.Accept());
    clientSocketInServer2 = server.GetLastAcceptedClient();
  } while (!clientSocketInServer2);
  do {
    BOOST_CHECK_EQUAL(true, server.Accept());
    clientSocketInServer3 = server.GetLastAcceptedClient();
  } while (!clientSocketInServer3);

  BOOST_CHECK_EQUAL(true, server.Accept());
  clientSocketInServer4 = server.GetLastAcceptedClient();
  BOOST_CHECK(!clientSocketInServer4);

  clientSocket1.Send("foo");
  clientSocket1.Send("bar");
  clientSocket1.Send("baz");

  {
    std::string receivedData;
    do {
      receivedData.append(clientSocketInServer1->GetLastReceivedData());
      BOOST_CHECK_EQUAL(true, clientSocketInServer1->Receive());
    } while (receivedData.size() < 9);
    BOOST_CHECK_EQUAL("foobarbaz", receivedData);

    BOOST_CHECK_EQUAL(true, clientSocketInServer1->Receive());
    BOOST_CHECK_EQUAL("", clientSocketInServer1->GetLastReceivedData());

    BOOST_CHECK_EQUAL(true, clientSocketInServer1->Receive());
    BOOST_CHECK_EQUAL("", clientSocketInServer1->GetLastReceivedData());
  }

  BOOST_CHECK_EQUAL(true, clientSocketInServer2->Send("FOO"));
  BOOST_CHECK_EQUAL(true, clientSocketInServer2->Send("BAR"));
  BOOST_CHECK_EQUAL(true, clientSocketInServer2->Send("BAZ"));

  {
    std::string receivedData;
    do {
      receivedData.append(clientSocket2.GetLastReceivedData());
      BOOST_CHECK_EQUAL(true, clientSocket2.Receive());
    } while (receivedData.size() < 9);
    BOOST_CHECK_EQUAL("FOOBARBAZ", receivedData);

    BOOST_CHECK_EQUAL(true, clientSocket2.Receive());
    BOOST_CHECK_EQUAL("",    clientSocket2.GetLastReceivedData());

    BOOST_CHECK_EQUAL(true, clientSocket2.Receive());
    BOOST_CHECK_EQUAL("",    clientSocket2.GetLastReceivedData());
  }
}

BOOST_AUTO_TEST_CASE(IPC_SocketServer_SendEmptyData) {
  SocketEnvironment socketEnvironment;
  SocketServer server;

  SocketClient clientSocket("127.0.0.1", server.GetPort());
  std::shared_ptr<ITransportClient> clientSocketInServer;
  do {
    BOOST_CHECK_EQUAL(true, server.Accept());
    clientSocketInServer = server.GetLastAcceptedClient();
  } while (!clientSocketInServer);


  BOOST_CHECK_EQUAL(true, clientSocket.Send(""));
  clientSocket.Close();
  BOOST_CHECK_EQUAL(false, clientSocket.Send(""));
}

BOOST_AUTO_TEST_CASE(IPC_SocketServer_SendBigData) {
  SocketEnvironment socketEnvironment;
  SocketServer server;

  SocketClient clientSocket("127.0.0.1", server.GetPort());
  std::shared_ptr<ITransportClient> clientSocketInServer;
  do {
    BOOST_CHECK_EQUAL(true, server.Accept());
    clientSocketInServer = server.GetLastAcceptedClient();
  } while (!clientSocketInServer);

  std::string bigData = std::string(16777216, 'a');
  BOOST_CHECK_EQUAL(true, clientSocket.Send(bigData));

  std::string receivedData;
  do {
    receivedData.append(clientSocketInServer->GetLastReceivedData());
    BOOST_CHECK_EQUAL(true, clientSocketInServer->Receive());
  } while (receivedData.size() < bigData.size());
  BOOST_CHECK_EQUAL(bigData.size(), receivedData.size());
}

}
}
