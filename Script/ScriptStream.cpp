#include "ScriptStream.hpp"

#include <cassert>

namespace Kaiko {
namespace Script {

ScriptStream::ScriptStream(const std::string& str)
  : str(str),
    strIt(str.begin()),
    currentLineNo(1),
    currentColumn(-1),
    previousLastColumn(-1) {
}

char
ScriptStream::GetCurrentChar() const {
  assert(0 <= this->currentColumn);
  assert(this->strIt != str.end());
  return *this->strIt;
}

int
ScriptStream::GetCurrentLineNo() const {
  return this->currentLineNo;
}

int
ScriptStream::GetCurrentColumn() const {
  return this->currentColumn;
}

bool
ScriptStream::MoveNext() {
  assert(strIt != str.end());
  if (this->currentColumn != -1) {
    if (*this->strIt == '\n') {
      this->previousLastColumn = this->currentColumn;
      ++this->currentLineNo;
      this->currentColumn = -1;
    }
    if (strIt != str.end()) {
      ++this->strIt;
    }
  }
  ++this->currentColumn;
  if (strIt == str.end()) {
    return false;
  }
  const char c = *strIt;
  return c != '\0' && c != '\004' && c != '\032';
}

void
ScriptStream::MovePrevious() {
  assert(0 <= this->currentColumn);
  --this->currentColumn;
  if (this->currentColumn == -1) {
    if (this->currentLineNo == 1) {
      return;
    }
    assert(this->previousLastColumn != -1);
    --this->currentLineNo;
    this->currentColumn = this->previousLastColumn;
    this->previousLastColumn = -1;
  }
  --this->strIt;
  assert(1 <= this->currentLineNo);
}

}
}
