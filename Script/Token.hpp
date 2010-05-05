#ifndef KAIKO_SCRIPT_TOKEN_HPP
#define KAIKO_SCRIPT_TOKEN_HPP

#include <string>

namespace Kaiko {
namespace Script {

enum TokenType {
  TokenTypeLineFeed           = '\n',
  TokenTypeExclamationMark    = '!',
  TokenTypePercentSign        = '%',
  TokenTypeAmpersandSign      = '&',
  TokenTypeLeftParenthesis    = '(',
  TokenTypeRightParenthesis   = ')',
  TokenTypeAsterisk           = '*',
  TokenTypePlusSign           = '+',
  TokenTypeComma              = ',',
  TokenTypeHyphenMinus        = '-',
  TokenTypeFullStop           = '.',
  TokenTypeSolidus            = '/',
  TokenTypeColon              = ':',
  TokenTypeSemicolon          = ';',
  TokenTypeLessThanSign       = '<',
  TokenTypeEqualsSign         = '=',
  TokenTypeGreaterThanSign    = '>',
  TokenTypeQuestionMark       = '?',
  TokenTypeAt                 = '@',
  TokenTypeLeftSquareBracket  = '[',
  TokenTypeReverseSolidus     = '\\',
  TokenTypeRightSquareBracket = ']',
  TokenTypeCircumflexAccent   = '^',
  TokenTypeLeftCurlyBracket   = '{',
  TokenTypeVerticalLine       = '|',
  TokenTypeRightCurlyBracket  = '}',

  TokenTypeOperators = 0x100,
  TokenTypeAndAnd,             // &&
  TokenTypeLtLt,               // <<
  TokenTypeLtEqual,            // <=
  TokenTypeEqualEqual,         // ==
  TokenTypeGtEqual,            // >=
  TokenTypeGtGt,               // >>
  TokenTypeGtGtGt,             // >>>
  TokenTypeOrOr,               // ||

  TokenTypeKeywords = 0x200,
  TokenTypeKeywordBreak,
  TokenTypeKeywordCatch,
  TokenTypeKeywordContinue,
  TokenTypeKeywordDefer,
  TokenTypeKeywordFalse,
  TokenTypeKeywordFor,
  TokenTypeKeywordIf,
  TokenTypeKeywordNil,
  TokenTypeKeywordReturn,
  TokenTypeKeywordSelf,
  TokenTypeKeywordStatic,
  TokenTypeKeywordTry,
  TokenTypeKeywordTrue,
  TokenTypeKeywordYield,

  TokenTypeOthers = 0x300,
  TokenTypeIdentifier,
  TokenTypeIntegerLiteral,
  TokenTypeStringLiteral,
};

class Token {
public:
  Token(TokenType type, int lineNo, int column);
  Token(TokenType type, int lineNo, int column, int value);
  Token(TokenType type, int lineNo, int column, const std::string& value);
  TokenType GetType() const;
  int GetLineNo() const;
  int GetColumn() const;
  int GetIntegerValue() const;
  const std::string& GetStringValue() const;
private:
  TokenType type;
  int lineNo;
  int column;
  int integerValue;
  std::string stringValue;
};

}
}

#endif
