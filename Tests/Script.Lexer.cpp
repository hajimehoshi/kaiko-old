#include "../Script/Lexer.hpp"
#include "../Script/ScriptStream.hpp"

#include <boost/test/unit_test.hpp>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::Script;

BOOST_AUTO_TEST_CASE(Script_Lexer_Scan_Comment) {
  {
    std::string str =
      "//foo\n"
      "bar\n"
      "//baz";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIdentifier,
                      e->GetCurrent().GetType());  
    BOOST_CHECK_EQUAL(2, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("bar", e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeLineFeed,
                      e->GetCurrent().GetType());  
    BOOST_CHECK_EQUAL(2, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(3, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str =
      "/*foo\n"
      "bar/*baz*/\n"
      "qux\n"
      "/* quux corge \n"
      " */  grault";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIdentifier,
                      e->GetCurrent().GetType());  
    BOOST_CHECK_EQUAL(3, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("qux", e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeLineFeed,
                      e->GetCurrent().GetType());  
    BOOST_CHECK_EQUAL(3, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(3, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIdentifier,
                      e->GetCurrent().GetType());  
    BOOST_CHECK_EQUAL(5, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(5, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("grault", e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
}

BOOST_AUTO_TEST_CASE(Script_Lexer_Characters) {
  {
    std::string str = "& && & &&";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeAmpersandSign,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeAndAnd,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(2, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeAmpersandSign,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(5, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeAndAnd,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(7, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "| || | ||";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeVerticalLine,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeOrOr,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(2, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeVerticalLine,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(5, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeOrOr,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(7, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "< << <= < << <=";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeLessThanSign,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeLtLt,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(2, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeLtEqual,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(5, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeLessThanSign,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(8, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeLtLt,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(10, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeLtEqual,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(13, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "= == = ==";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeEqualsSign,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeEqualEqual,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(2, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeEqualsSign,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(5, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeEqualEqual,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(7, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "> >= >> >>> > >= >> >>>";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeGreaterThanSign,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeGtEqual,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(2, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeGtGt,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(5, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeGtGtGt,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(8, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeGreaterThanSign,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(12, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeGtEqual,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(14, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeGtGt,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(17, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeGtGtGt,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(20, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
}

BOOST_AUTO_TEST_CASE(Script_Lexer_Identifier) {
  std::string str = "foo";
  ScriptStream scriptStream(str);
  Lexer lexer(scriptStream);
  boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
  BOOST_CHECK_EQUAL(true, e->MoveNext());
  BOOST_CHECK_EQUAL(TokenTypeIdentifier,
                    e->GetCurrent().GetType());
  BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
  BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
  BOOST_CHECK_EQUAL("foo", e->GetCurrent().GetStringValue());
  BOOST_CHECK_EQUAL(false, e->MoveNext());
}

BOOST_AUTO_TEST_CASE(Script_Lexer_Scan_Integer) {
  {
    std::string str = "0";

    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0_";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "123456";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(123456, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "123_456_";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(123456, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0b010101";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(21, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0b_010_101_";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(21, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0B010101";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(21, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0b12";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(3, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(2, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0123456";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(42798, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0_123_456";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(42798, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "08";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(8, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0x_c0F_fEe";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(12648430, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0X_c0F_fEe";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(12648430, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str = "0xfg";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIntegerLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(15, e->GetCurrent().GetIntegerValue());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIdentifier,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(3, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("g", e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
}

BOOST_AUTO_TEST_CASE(Script_Lexer_String) {
  {
    // normal
    std::string str = "\"string literal\"";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeStringLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("string literal",
                      e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    // non-ASCII character
    std::string str = "\"\xE6\x96\x87\xE5\xAD\x97\xE5\x88\x97\"";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeStringLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1,
                      e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0,
                      e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("\xE6\x96\x87\xE5\xAD\x97\xE5\x88\x97",
                      e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    // backslash escape
    std::string str = "\"\\\\\\\"\\'\\a\\b\\f\\n\\r\\t\\v\"";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeStringLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("\\\"'\a\b\f\n\r\t\v",
                      e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    // hex escape
    std::string str = "\"\\xc0\\x0f\\xEE \\xabc \\x1z\"";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeStringLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("\xC0\x0F\xEE \xAB" "c \x1" "z",
                      e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    // oct escape
    std::string str = "\"\\31\\41\\592\\653\"";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeStringLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("\31\41\5" "92" "\xAB",
                      e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    // Unicode escape
    std::string str = "\"\\u002f \\u00A9 \\u0192 \\u3042\"";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeStringLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("\x2f \xC2\xA9 \xC6\x92 \xE3\x81\x82",
                      e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
  {
    std::string str =
      "\"Hello "
      "\\xE3\\x81\\x93"
      "\\343\\202\\223"
      "\\u306b"
      "\\xe3\\x81\\xa1"
      "\\u306F\\n\"";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeStringLiteral,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("Hello "
                      "\xE3\x81\x93"
                      "\xE3\x82\x93"
                      "\xE3\x81\xAB"
                      "\xE3\x81\xA1"
                      "\xE3\x81\xAF\n",
                      e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
}

BOOST_AUTO_TEST_CASE(Script_Lexer_Keyword) {
  {
    std::string str = "for For";
    ScriptStream scriptStream(str);
    Lexer lexer(scriptStream);
    boost::shared_ptr< Util::IEnumerator<Token> > e = lexer.GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeKeywordFor,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(0, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL(TokenTypeIdentifier,
                      e->GetCurrent().GetType());
    BOOST_CHECK_EQUAL(1, e->GetCurrent().GetLineNo());
    BOOST_CHECK_EQUAL(4, e->GetCurrent().GetColumn());
    BOOST_CHECK_EQUAL("For",
                      e->GetCurrent().GetStringValue());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
}

}
}
