#ifndef KAIKO_SCRIPT_SCRIPTSTREAM_HPP
#define KAIKO_SCRIPT_SCRIPTSTREAM_HPP

#include "IScriptStream.hpp"

#include <boost/noncopyable.hpp>
#include <string>

namespace Kaiko {
namespace Script {

class ScriptStream : public IScriptStream,
                     private boost::noncopyable {
public:
  explicit ScriptStream(const std::string& str);
  char GetCurrentChar() const;
  int GetCurrentLineNo() const;
  int GetCurrentColumn() const;
  bool MoveNext();
  void MovePrevious();
private:
  const std::string& str;
  std::string::const_iterator strIt;
  int currentLineNo;
  int currentColumn;
  int previousLastColumn;
};

}
}

#endif
