#include "Lexer.hpp"

#include "IScriptStream.hpp"

#include <cassert>
#include <map>
#include <string>

namespace Kaiko {
namespace Script {

namespace {

inline bool
IsAlphabet(const char c) {
  return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

inline bool
IsHexDigit(const char c) {
  return ('0' <= c && c <= '9') || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
}

inline char
HexDigitToInt(const char c) {
  assert(IsHexDigit(c));
  if ('0' <= c && c <= '9') {
    return c - '0';
  } else if ('A' <= c && c <= 'F') {
    return c - 'A' + 10;
  } else if ('a' <= c && c <= 'f') {
    return c - 'a' + 10;
  } else {
    assert(0);
    return 0;
  }
}

}

class LexerEnumerator : public Util::IEnumerator<Token>,
                        private boost::noncopyable {
public:
  LexerEnumerator(const Lexer& lexer);
  virtual Token GetCurrent() const;
  virtual bool MoveNext();
private:
  const Lexer& lexer;
  std::vector<const Token>::size_type index;
};

Lexer::Lexer(IScriptStream& scriptStream) {
  std::map<std::string, TokenType> keywordToTokenType;
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("break", TokenTypeKeywordBreak));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("catch", TokenTypeKeywordCatch));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("continue", TokenTypeKeywordContinue));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("defer", TokenTypeKeywordDefer));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("false", TokenTypeKeywordFalse));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("for", TokenTypeKeywordFor));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("if", TokenTypeKeywordIf));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("nil", TokenTypeKeywordNil));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("return", TokenTypeKeywordReturn));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("self", TokenTypeKeywordSelf));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("static", TokenTypeKeywordStatic));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("true", TokenTypeKeywordTrue));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("try", TokenTypeKeywordTry));
  keywordToTokenType.insert(std::map<std::string, TokenType>::value_type("yield", TokenTypeKeywordYield));

  while(scriptStream.MoveNext()) {
    switch (scriptStream.GetCurrentChar()) {
    case ' ':
    case '\t':
    case '\v':
    case '\r':
      // spaces (do nothing)
      break;
    case '\n':
      // new line
      if (!this->tokens.empty()) {
        this->tokens.push_back(Token(TokenTypeLineFeed,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn()));
      }
      break;
    case '!':
    case '%':
    case '(':
    case ')':
    case '*':
    case '+':
    case ',':
    case '-':
    case '.':
    case ':':
    case ';':
    case '?':  
    case '@':
    case '[':
    case '\\':
    case ']':
    case '^':
    case '{':
    case '}':
      this->tokens.push_back(Token(static_cast<TokenType>(scriptStream.GetCurrentChar()),
                                   scriptStream.GetCurrentLineNo(),
                                   scriptStream.GetCurrentColumn()));
      break;
    case '&':
      if (scriptStream.MoveNext() && scriptStream.GetCurrentChar() == '&') {
        this->tokens.push_back(Token(TokenTypeAndAnd,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn() - 1));
      } else {
        this->tokens.push_back(Token(TokenTypeAmpersandSign,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn() - 1));
        scriptStream.MovePrevious();
      }
      break;
    case '<':
      if (scriptStream.MoveNext()) {
        switch (scriptStream.GetCurrentChar()) {
        case '<':
          this->tokens.push_back(Token(TokenTypeLtLt,
                                       scriptStream.GetCurrentLineNo(),
                                       scriptStream.GetCurrentColumn() - 1));
          break;
        case '=':
          this->tokens.push_back(Token(TokenTypeLtEqual,
                                       scriptStream.GetCurrentLineNo(),
                                       scriptStream.GetCurrentColumn() - 1));
          break;
        default:
          this->tokens.push_back(Token(TokenTypeLessThanSign,
                                       scriptStream.GetCurrentLineNo(),
                                       scriptStream.GetCurrentColumn() - 1));
          scriptStream.MovePrevious();
          break;
        }
      } else {
        this->tokens.push_back(Token(TokenTypeLessThanSign,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn() - 1));
        scriptStream.MovePrevious();
      }
      break;
    case '=':
      if (scriptStream.MoveNext() && scriptStream.GetCurrentChar() == '=') {
        this->tokens.push_back(Token(TokenTypeEqualEqual,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn() - 1));
      } else {
        this->tokens.push_back(Token(TokenTypeEqualsSign,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn() - 1));
        scriptStream.MovePrevious();
      }
      break;
    case '>':
      if (scriptStream.MoveNext()) {
        switch (scriptStream.GetCurrentChar()) {
        case '=':
          this->tokens.push_back(Token(TokenTypeGtEqual,
                                       scriptStream.GetCurrentLineNo(),
                                       scriptStream.GetCurrentColumn() - 1));
          break;
        case '>':
          if (scriptStream.MoveNext() && scriptStream.GetCurrentChar() == '>') {
            this->tokens.push_back(Token(TokenTypeGtGtGt,
                                         scriptStream.GetCurrentLineNo(),
                                         scriptStream.GetCurrentColumn() - 2));
          } else {
            this->tokens.push_back(Token(TokenTypeGtGt,
                                         scriptStream.GetCurrentLineNo(),
                                         scriptStream.GetCurrentColumn() - 2));
            scriptStream.MovePrevious();
          }
          break;
        default:
          this->tokens.push_back(Token(TokenTypeGreaterThanSign,
                                       scriptStream.GetCurrentLineNo(),
                                       scriptStream.GetCurrentColumn() - 1));
          scriptStream.MovePrevious();
          break;
        }
        
      } else {
        this->tokens.push_back(Token(TokenTypeGreaterThanSign,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn() - 1));
        scriptStream.MovePrevious();
      }
      break;
    case '|':
      if (scriptStream.MoveNext() && scriptStream.GetCurrentChar() == '|') {
        this->tokens.push_back(Token(TokenTypeOrOr,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn() - 1));
      } else {
        this->tokens.push_back(Token(TokenTypeVerticalLine,
                                     scriptStream.GetCurrentLineNo(),
                                     scriptStream.GetCurrentColumn() - 1));
        scriptStream.MovePrevious();
      }
      break;
    case '/':
      // comment or operator
      if (scriptStream.MoveNext()) {
        switch (scriptStream.GetCurrentChar()) {
        case '/':
          // one-line comment
          do {
            if (!scriptStream.MoveNext()) {
              return;
            }
          } while (scriptStream.GetCurrentChar() != '\n');
          continue;
        case '*':
          // multi-line comment
          while (true) {
            if (!scriptStream.MoveNext()) {
              throw "multi-line comment meets end of script";
            }
            if (scriptStream.GetCurrentChar() == '*') {
              if (!scriptStream.MoveNext()) {
                throw "multi-line comment meets end of script";
              }
              if (scriptStream.GetCurrentChar() == '/') {
                break;
              }
            }
          }
          continue;
        default:
          // operator
          break;
        }
      }
      this->tokens.push_back(Token(TokenTypeSolidus,
                                   scriptStream.GetCurrentLineNo(),
                                   scriptStream.GetCurrentColumn() - 1));
      scriptStream.MovePrevious();
      break;
    case '0':
      {
        const int startLineNo = scriptStream.GetCurrentLineNo();
        const int startColumn = scriptStream.GetCurrentColumn();
        int tokenValue = 0;
        if (scriptStream.MoveNext()) {
          switch (scriptStream.GetCurrentChar()) {
          case 'b':
          case 'B':
            {
              if (!scriptStream.MoveNext()) {
                throw "numerical literal without digits";
              }
              char c = scriptStream.GetCurrentChar();
              do {
                if (c != '_') {
                  tokenValue <<= 1;
                  tokenValue += c - '0';
                }
              } while (scriptStream.MoveNext() &&
                       (c = scriptStream.GetCurrentChar(), (c == '0' || c == '1' || c == '_')));
            }
            break;
          case 'x':
          case 'X':
            {
              if (!scriptStream.MoveNext()) {
                throw "numerical literal without digits";
              }
              char c = scriptStream.GetCurrentChar();
              do {
                if (c != '_') {    
                  tokenValue <<= 4;
                  tokenValue += HexDigitToInt(c);
                }
              } while (scriptStream.MoveNext() &&
                       (c = scriptStream.GetCurrentChar(), (IsHexDigit(c) || c == '_')));
            }
            break;
          case '_':
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
            {
              char c = scriptStream.GetCurrentChar();
              do {
                if (c != '_') {    
                  tokenValue <<= 3;
                  tokenValue += c - '0';
                }
              } while (scriptStream.MoveNext() &&
                       (c = scriptStream.GetCurrentChar(), ('0' <= c && c <= '7' || c == '_')));
            }
            break;
          default:
            // zero
            break;
          }
        }
        this->tokens.push_back(Token(TokenTypeIntegerLiteral, startLineNo, startColumn, tokenValue));
        scriptStream.MovePrevious();
      }
      break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      {
        const int startLineNo = scriptStream.GetCurrentLineNo();
        const int startColumn = scriptStream.GetCurrentColumn();
        char c = scriptStream.GetCurrentChar();
        int tokenValue = c - '0';
        while (scriptStream.MoveNext() &&
               (c = scriptStream.GetCurrentChar(), (('0' <= c && c <= '9') || c == '_'))) {
          if (c != '_') {
            tokenValue *= 10;
            tokenValue += c - '0';
          }
        }
        this->tokens.push_back(Token(TokenTypeIntegerLiteral, startLineNo, startColumn, tokenValue));
        scriptStream.MovePrevious();
      }
      break;
    case '"':
      {
        // string literal
        std::string tokenStr;
        const int startLineNo = scriptStream.GetCurrentLineNo();
        const int startColumn = scriptStream.GetCurrentColumn();
        while (true) {
          if (!scriptStream.MoveNext()) {
            throw "unterminated string meets end of script";
          }
          switch (scriptStream.GetCurrentChar()) {
          case '\\':
            // char escape
            if (!scriptStream.MoveNext()) {
              throw "unterminated string meets end of script";
            }
            switch (scriptStream.GetCurrentChar()) {
            case '\\': tokenStr += '\\'; break;
            case '"':  tokenStr += '"';  break;
            case '\'': tokenStr += '\''; break;
            case 'a':  tokenStr += '\a'; break;
            case 'b':  tokenStr += '\b'; break;
            case 'f':  tokenStr += '\f'; break;
            case 'n':  tokenStr += '\n'; break;
            case 'r':  tokenStr += '\r'; break;
            case 't':  tokenStr += '\t'; break;
            case 'v':  tokenStr += '\v'; break;
            case 'x':
              {
                // hex escape
                if (!scriptStream.MoveNext()) {
                  throw "unterminated string meets end of script";
                }
                const char c = scriptStream.GetCurrentChar();
                if (IsHexDigit(c)) {
                  unsigned char hexValue = HexDigitToInt(c);
                  if (!scriptStream.MoveNext()) {
                    throw "unterminated string meets end of script";
                  }
                  const char c = scriptStream.GetCurrentChar();
                  if (IsHexDigit(c)) {
                    hexValue <<= 4;
                    hexValue += HexDigitToInt(c);
                  } else {
                    scriptStream.MovePrevious();
                  }
                  tokenStr += hexValue;
                } else {
                  throw "invalid hex escape";
                }
              }
              break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
              // oct escape  
              {
                unsigned char octValue = scriptStream.GetCurrentChar() - '0';
                if (!scriptStream.MoveNext()) {
                  throw "unterminated string meets end of script";
                }
                const char c = scriptStream.GetCurrentChar();
                if ('0' <= c && c <= '7') {
                  octValue <<= 3;
                  octValue += c - '0';
                  if (!scriptStream.MoveNext()) {
                    throw "unterminated string meets end of script";
                  }
                  const char c = scriptStream.GetCurrentChar();
                  if ('0' <= c && c <= '7') {
                    octValue <<= 3;
                    octValue += c - '0';
                  } else {
                    scriptStream.MovePrevious();
                  }
                } else {
                  scriptStream.MovePrevious();
                }
                tokenStr += octValue;
              }
              break;
            case 'u':
              // Unicode escape
              {
                unsigned short unicodeChar = 0;
                for (int i = 0; i < 4; ++i) {
                  if (!scriptStream.MoveNext()) {
                    throw "unterminated string meets end of script";
                  }
                  const char c = scriptStream.GetCurrentChar();
                  if (IsHexDigit(c)) {
                    unicodeChar <<= 4;
                    unicodeChar += HexDigitToInt(c);
                  } else {
                    throw "invalid Unicode escape";
                  }
                }
                if (unicodeChar <= 0x007f) {
                  tokenStr += unicodeChar & 0x7f;
                } else if (unicodeChar <= 0x07ff) {
                  tokenStr += ((unicodeChar & 0x07c0) >> 6) | 0xc0;
                  tokenStr += ((unicodeChar & 0x003f)     ) | 0x80;
                } else {
                  tokenStr += ((unicodeChar & 0xf000) >> 12) | 0xe0;
                  tokenStr += ((unicodeChar & 0x0fc0) >>  6) | 0x80;
                  tokenStr += ((unicodeChar & 0x003f)      ) | 0x80;
                }
              }
              break;
            default:
              throw "invalid backslash escape";
              break;
            }
            break;
          case '"':
            goto END_OF_STRING_LITERAL;
          default:
            tokenStr += scriptStream.GetCurrentChar();
            break;
          }
        }
      END_OF_STRING_LITERAL:
        this->tokens.push_back(Token(TokenTypeStringLiteral, startLineNo, startColumn, tokenStr));
        tokenStr.clear();
        break;
      }
    default:
      {
        // identifier or keyword
        std::string tokenStr;
        char c = scriptStream.GetCurrentChar();
        if (IsAlphabet(c)) {
          const int startLineNo = scriptStream.GetCurrentLineNo();
          const int startColumn = scriptStream.GetCurrentColumn();
          do {
            tokenStr += c;
            if (!scriptStream.MoveNext()) {
              break;
            }
            c = scriptStream.GetCurrentChar();
          } while (IsAlphabet(c));
          
          //keywordToTokenType
          std::map<std::string, TokenType>::iterator it = keywordToTokenType.find(tokenStr);
          if (it != keywordToTokenType.end()) {
            this->tokens.push_back(Token(it->second, startLineNo, startColumn));
          } else {
            this->tokens.push_back(Token(TokenTypeIdentifier, startLineNo, startColumn, tokenStr));
          }
          scriptStream.MovePrevious();
        }
      }
      break;
    }
  }
}

std::shared_ptr<Util::IEnumerator<Token>>
Lexer::GetEnumerator() const {
  return std::shared_ptr<Util::IEnumerator<Token>>(new LexerEnumerator(*this));
}

LexerEnumerator::LexerEnumerator(const Lexer& lexer)
  : lexer(lexer), index(-1) {
}

Token
LexerEnumerator::GetCurrent() const {
  assert(0 <= this->index);
  assert(this->index < this->lexer.tokens.size());
  return this->lexer.tokens.at(this->index);
}

bool
LexerEnumerator::MoveNext() {
  ++this->index;
  if (this->index < this->lexer.tokens.size()) {
    return true;
  } else {
    return false;
  }
}

}
}
