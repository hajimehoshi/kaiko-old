#include "NamedPipe.hpp"

#include "../Util/Strings.hpp"
#include "../Util/SystemException.hpp"

#include <windows.h>

namespace Kaiko {
namespace IPC {

struct NamedPipe::Impl {
  HANDLE handle;
  std::string lastReceivedData;
};

NamedPipe::NamedPipe(const std::string& name, bool isServer)
  : pimpl(new Impl()) {
  this->pimpl->handle = INVALID_HANDLE_VALUE;
  std::string pipeName("\\\\.\\pipe\\");
  pipeName.append(name);
  std::wstring pipeNameW = Util::Strings::UTF8ToUTF16LE(pipeName);
  if (isServer) {
    this->pimpl->handle = ::CreateNamedPipe(pipeNameW.c_str(),
                                            PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
                                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
                                            1,
                                            4096,
                                            4096,
                                            5000,
                                            0);
  } else {
    this->pimpl->handle = ::CreateFile(pipeNameW.c_str(),
                                       GENERIC_READ | GENERIC_WRITE,
                                       0,
                                       0,
                                       OPEN_EXISTING,
                                       SECURITY_SQOS_PRESENT | SECURITY_IDENTIFICATION,
                                       0);
  }
  if (this->pimpl->handle == INVALID_HANDLE_VALUE) {
    throw Util::SystemException(__FILE__, __LINE__);
  }
}

NamedPipe::~NamedPipe() throw() {
  this->Close();
}

void
NamedPipe::Close() throw() {
  if (this->pimpl->handle == INVALID_HANDLE_VALUE) {
    ::CloseHandle(this->pimpl->handle);
    this->pimpl->handle = INVALID_HANDLE_VALUE;
  }
}

bool
NamedPipe::Connect() {
  if (this->pimpl->handle == INVALID_HANDLE_VALUE) {
    return false;
  }
  if (!::ConnectNamedPipe(this->pimpl->handle, 0)) {
    throw Util::SystemException(__FILE__, __LINE__);
  }
  return true;
}

bool
NamedPipe::Disconnect() {
  if (this->pimpl->handle == INVALID_HANDLE_VALUE) {
    return false;
  }
  if (!::DisconnectNamedPipe(this->pimpl->handle)) {
    throw Util::SystemException(__FILE__, __LINE__);
  }
  return true;
}

const std::string&
NamedPipe::GetLastReceivedData() const {
  return this->pimpl->lastReceivedData;
}

bool
NamedPipe::Receive() {
  if (this->pimpl->handle == INVALID_HANDLE_VALUE) {
    this->pimpl->lastReceivedData.erase();
    return false;
  }
  char buffer[4096];
  DWORD numOfBytesRead = 0;
  if (!::ReadFile(this->pimpl->handle,
                  buffer,
                  sizeof(buffer) / sizeof(buffer[0]),
                  &numOfBytesRead,
                  0)) {
    throw Util::SystemException(__FILE__, __LINE__);
  }
  if (numOfBytesRead) {
    this->pimpl->lastReceivedData.swap(std::string(buffer, numOfBytesRead));
    return true;
  } else {
    this->pimpl->lastReceivedData.erase();
    return false;
  }
}

bool
NamedPipe::Send(const std::string& data) {
  if (this->pimpl->handle == INVALID_HANDLE_VALUE) {
    return false;
  }
  if (!::WriteFile(this->pimpl->handle,
                   data.c_str(),
                   data.size(),
                   0,
                   0)) {
    throw Util::SystemException(__FILE__, __LINE__);
  }
  return true;
}

}
}

