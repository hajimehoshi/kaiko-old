#ifndef KAIKO_SCRIPT_ISCRIPTSTREAM_HPP
#define KAIKO_SCRIPT_ISCRIPTSTREAM_HPP

namespace Kaiko {
namespace Script {

class IScriptStream {
public:
  virtual char GetCurrentChar() const = 0;
  virtual int GetCurrentLineNo() const = 0;
  virtual int GetCurrentColumn() const = 0;
  virtual bool MoveNext() = 0;
  virtual void MovePrevious() = 0;
};

}
}

#endif
