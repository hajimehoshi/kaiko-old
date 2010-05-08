#ifndef KAIKO_SCRIPT_LEXER_HPP
#define KAIKO_SCRIPT_LEXER_HPP

#include "../Util/IEnumerable.hpp"
#include "Token.hpp"

#include <boost/noncopyable.hpp>
#include <memory>
#include <vector>

namespace Kaiko {
namespace Script {

class IScriptStream;

class Lexer : public Util::IEnumerable<Token>,
              private boost::noncopyable {
  friend class LexerEnumerator;
public:
  explicit Lexer(IScriptStream& scriptStream);
  std::shared_ptr<Util::IEnumerator<Token>> GetEnumerator() const;
private:
  std::vector<const Token> tokens;
};

}
}

#endif
