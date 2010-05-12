#include "../Util/StringTreeNode.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::Util;

BOOST_AUTO_TEST_CASE(Util_StringTreeNode) {
  {
    StringTreeNode stringTreeNode("foo");
    BOOST_CHECK_EQUAL("foo", stringTreeNode.GetKey());
    stringTreeNode.AddChildNode("bar");  
    stringTreeNode.AddChildNode("baz");
    BOOST_CHECK_EQUAL(true, stringTreeNode.Contains("bar"));
    BOOST_CHECK_EQUAL(true, stringTreeNode.Contains("baz"));
    BOOST_CHECK_EQUAL(false, stringTreeNode.Contains("qux"));

    auto e = stringTreeNode.GetChildNodes().GetEnumerator();
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL("bar", e->GetCurrent()->GetKey());
    BOOST_CHECK_EQUAL(true, e->MoveNext());
    BOOST_CHECK_EQUAL("baz", e->GetCurrent()->GetKey());
    BOOST_CHECK_EQUAL(false, e->MoveNext());
  }
}

}
}
