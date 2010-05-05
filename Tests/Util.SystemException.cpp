#include "../Util/SystemException.hpp"

#include <boost/test/unit_test.hpp>
#include <windows.h>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::Util;

BOOST_AUTO_TEST_CASE(Util_Exception_Exception) {
  {
    SystemException exception(__FILE__, __LINE__, ERROR_FILE_NOT_FOUND);
    BOOST_CHECK_EQUAL(ERROR_FILE_NOT_FOUND, exception.GetErrorNo());
    BOOST_CHECK(exception.What().find("The system cannot find the file specified.") != std::string::npos);
    BOOST_CHECK_EQUAL(exception.What(), std::string(exception.what()));
  }
  {
    SystemException exception(__FILE__, __LINE__, WSAEWOULDBLOCK);
    BOOST_CHECK_EQUAL(WSAEWOULDBLOCK, exception.GetErrorNo());
    BOOST_CHECK(exception.What().find("A non-blocking socket operation could not be completed immediately.") != std::string::npos);
    BOOST_CHECK_EQUAL(exception.What(), std::string(exception.what()));
  }
}

}
}
