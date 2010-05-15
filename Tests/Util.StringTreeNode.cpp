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

    {
      auto nodes = stringTreeNode.GetChildNodes();
      BOOST_CHECK_EQUAL(2, static_cast<int>(nodes.size()));
      BOOST_CHECK_EQUAL("bar", nodes[0]->GetKey());
      BOOST_CHECK_EQUAL(0, static_cast<int>(nodes[0]->GetChildNodes().size()));
      BOOST_CHECK_EQUAL("baz", nodes[1]->GetKey());
      BOOST_CHECK_EQUAL(0, static_cast<int>(nodes[1]->GetChildNodes().size()));
    }

    stringTreeNode.GetChildNodes()[0]->AddChildNode("qux");
    {
      auto nodes = stringTreeNode.GetChildNodes();
      BOOST_CHECK_EQUAL(1, static_cast<int>(nodes[0]->GetChildNodes().size()));
      BOOST_CHECK_EQUAL("qux", nodes[0]->GetChildNodes()[0]->GetKey());
    }
  }
}

BOOST_AUTO_TEST_CASE(Util_StringTreeNode_CreateFromString) {
  {
    StringTreeNode stringTreeNode("foo");
    BOOST_CHECK_EQUAL("foo", stringTreeNode.GetKey());
    stringTreeNode.AddChildNode("bar");
    stringTreeNode.AddChildNode("baz");
    stringTreeNode.GetChildNodes()[0]->AddChildNode("qux");
    auto stringTreeNode2 = StringTreeNode::CreateFromString(stringTreeNode.ToString());
    BOOST_CHECK(stringTreeNode2);
    BOOST_CHECK_EQUAL("foo", stringTreeNode2->GetKey());
    BOOST_CHECK_EQUAL(2, static_cast<int>(stringTreeNode2->GetChildNodes().size()));
    BOOST_CHECK_EQUAL("bar", stringTreeNode2->GetChildNodes()[0]->GetKey());
    BOOST_CHECK_EQUAL(1, static_cast<int>(stringTreeNode2->GetChildNodes()[0]->GetChildNodes().size()));
    BOOST_CHECK_EQUAL("qux", stringTreeNode2->GetChildNodes()[0]->GetChildNodes()[0]->GetKey());
    BOOST_CHECK_EQUAL("baz", stringTreeNode2->GetChildNodes()[1]->GetKey());
    BOOST_CHECK_EQUAL(0, static_cast<int>(stringTreeNode2->GetChildNodes()[1]->GetChildNodes().size()));
  }
}

}
}
