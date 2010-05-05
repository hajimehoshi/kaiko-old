#include "Token.hpp"

namespace Kaiko {
namespace Script {

Token::Token(TokenType type, int lineNo, int column)
  : type(type), lineNo(lineNo), column(column) {
}

Token::Token(TokenType type, int lineNo, int column, int value)
  : type(type), lineNo(lineNo), column(column), integerValue(value) {
}

Token::Token(TokenType type, int lineNo, int column, const std::string& value)
  : type(type), lineNo(lineNo), column(column), stringValue(value) {
}

TokenType
Token::GetType() const {
  return this->type;
}

int
Token::GetLineNo() const {
  return this->lineNo;
}

int
Token::GetColumn() const {
  return this->column;
}

const std::string&
Token::GetStringValue() const {
  return this->stringValue;
}

int
Token::GetIntegerValue() const {
  return this->integerValue;
}

}
}
