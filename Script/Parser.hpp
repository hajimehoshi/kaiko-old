#ifndef KAIKO_SCRIPT_PARSER_HPP
#define KAIKO_SCRIPT_PARSER_HPP

#include <boost/noncopyable.hpp>

namespace Kaiko {
namespace Script {

class Lexer;

class Parser : private boost::noncopyable {
  explicit Parser(Lexer& lexer);
private:
  Lexer& lexer;
};

}
}

#endif
