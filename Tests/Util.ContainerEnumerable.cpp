#include "../Util/ContainerEnumerable.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include <list>
#include <vector>

namespace Kaiko {
namespace Test {

using namespace Kaiko::Util;

BOOST_AUTO_TEST_CASE(Util_ContainerEnumerable_Vector) {
  {
    std::vector<int> items;
    ContainerEnumerable< std::vector<int> > containerEnumerable(items);
    items.push_back(3);
    items.push_back(1);
    items.push_back(4);
    items.push_back(1);
    items.push_back(5);
    {
      boost::shared_ptr< IEnumerator<int> > e = containerEnumerable.GetEnumerator();
      BOOST_CHECK(e.get());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(3, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(1, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(4, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(1, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(5, e->GetCurrent());
      BOOST_CHECK_EQUAL(false, e->MoveNext());
    }
    items.push_back(9);
    {
      boost::shared_ptr< IEnumerator<int> > e = containerEnumerable.GetEnumerator();
      BOOST_CHECK(e.get());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(3, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(1, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(4, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(1, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(5, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(9, e->GetCurrent());
      BOOST_CHECK_EQUAL(false, e->MoveNext());
    }
  }
}

BOOST_AUTO_TEST_CASE(Util_ContainerEnumerable_List) {
  {
    std::list<int> items;
    ContainerEnumerable< std::list<int> > containerEnumerable(items);
    items.push_back(3);
    items.push_back(1);
    items.push_back(4);
    items.push_back(1);
    items.push_back(5);
    {
      boost::shared_ptr< IEnumerator<int> > e = containerEnumerable.GetEnumerator();
      BOOST_CHECK(e.get());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(3, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(1, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(4, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(1, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(5, e->GetCurrent());
      BOOST_CHECK_EQUAL(false, e->MoveNext());
    }
    items.push_back(9);
    {
      boost::shared_ptr< IEnumerator<int> > e = containerEnumerable.GetEnumerator();
      BOOST_CHECK(e.get());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(3, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(1, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(4, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(1, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(5, e->GetCurrent());
      BOOST_CHECK_EQUAL(true, e->MoveNext());
      BOOST_CHECK_EQUAL(9, e->GetCurrent());
      BOOST_CHECK_EQUAL(false, e->MoveNext());
    }
  }
}

}
}

