#include "../Script/ScriptStream.hpp"

#include <boost/test/unit_test.hpp>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::Script;

BOOST_AUTO_TEST_CASE(Script_ScriptStream_MoveNext) {
  std::string str = "foo\nbar";
  ScriptStream scriptStream(str);
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('f', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(0, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('o', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('o', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('\n', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(3, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('b', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(0, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('a', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('r', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(false, scriptStream.MoveNext());
}

BOOST_AUTO_TEST_CASE(Script_ScriptStream_MovePrevious) {
  std::string str = "foo\nbar";
  ScriptStream scriptStream(str);
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('f', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(0, scriptStream.GetCurrentColumn());

  scriptStream.MovePrevious();
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('f', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(0, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('o', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentColumn());

  scriptStream.MovePrevious();
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('o', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('o', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('\n', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(3, scriptStream.GetCurrentColumn());

  scriptStream.MovePrevious();
  BOOST_REQUIRE_EQUAL('o', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('\n', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(3, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('b', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(0, scriptStream.GetCurrentColumn());

  scriptStream.MovePrevious();
  BOOST_REQUIRE_EQUAL('\n', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(3, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('b', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(0, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('a', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentColumn());
  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('r', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentColumn());

  scriptStream.MovePrevious();
  BOOST_REQUIRE_EQUAL('a', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(1, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(true, scriptStream.MoveNext());
  BOOST_REQUIRE_EQUAL('r', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(false, scriptStream.MoveNext());

  scriptStream.MovePrevious();
  BOOST_REQUIRE_EQUAL('r', scriptStream.GetCurrentChar());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentLineNo());
  BOOST_REQUIRE_EQUAL(2, scriptStream.GetCurrentColumn());

  BOOST_REQUIRE_EQUAL(false, scriptStream.MoveNext());
}

}
}
