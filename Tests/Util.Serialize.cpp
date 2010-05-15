#include "../Util/Exception.hpp"
#include "../Util/Serialization.hpp"

#include <boost/test/unit_test.hpp>
#include <vector>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::Util;

BOOST_AUTO_TEST_CASE(Util_Serialization_BytesToLength) {
  {
    const std::string bytes("");
    int length;
    int readBytesNum;
    std::tie(length, readBytesNum) = Serialization::BytesToLength(bytes.begin(), bytes.end());
    BOOST_CHECK_EQUAL(0, length);
    BOOST_CHECK_EQUAL(0, readBytesNum);
  }
  {
    const std::string bytes("\x03");
    int length;
    int readBytesNum;
    std::tie(length, readBytesNum) = Serialization::BytesToLength(bytes.begin(), bytes.end());
    BOOST_CHECK_EQUAL(3, length);
    BOOST_CHECK_EQUAL(1, readBytesNum);
  }
  {
    const std::string bytes("\x7f");
    int length;
    int readBytesNum;
    std::tie(length, readBytesNum) = Serialization::BytesToLength(bytes.begin(), bytes.end());
    BOOST_CHECK_EQUAL(127, length);
    BOOST_CHECK_EQUAL(1, readBytesNum);
  }
  {
    std::string bytes("\x81");
    bytes.push_back('\x00');
    int length;
    int readBytesNum;
    std::tie(length, readBytesNum) = Serialization::BytesToLength(bytes.begin(), bytes.end());
    BOOST_CHECK_EQUAL(128, length);
    BOOST_CHECK_EQUAL(2, readBytesNum);
  }
  {
    const std::string bytes("\x81\x01");
    int length;
    int readBytesNum;
    std::tie(length, readBytesNum) = Serialization::BytesToLength(bytes.begin(), bytes.end());
    BOOST_CHECK_EQUAL(129, length);
    BOOST_CHECK_EQUAL(2, readBytesNum);
  }
  {
    const std::string bytes("\x81\x48");
    int length;
    int readBytesNum;
    std::tie(length, readBytesNum) = Serialization::BytesToLength(bytes.begin(), bytes.end());
    BOOST_CHECK_EQUAL(200, length);
    BOOST_CHECK_EQUAL(2, readBytesNum);
  }
  {
    const std::string bytes("\x93\x96\x2f");
    int length;
    int readBytesNum;
    std::tie(length, readBytesNum) = Serialization::BytesToLength(bytes.begin(), bytes.end());
    BOOST_CHECK_EQUAL(314159, length);
    BOOST_CHECK_EQUAL(3, readBytesNum);
  }
  {
    const std::string bytes("\xff\xff\xff\xff\xff");
    BOOST_CHECK_THROW(Serialization::BytesToLength(bytes.begin(), bytes.end()), Exception);
  }
}

BOOST_AUTO_TEST_CASE(Util_Serialization_BytesToLength_Error) {
  {
    // redundant bytes
    const std::string bytes("\x80\x01");
    BOOST_CHECK_THROW(Serialization::BytesToLength(bytes.begin(), bytes.end()), Exception);
  }
}

BOOST_AUTO_TEST_CASE(Util_Serialization_LengthToBytes) {
  BOOST_CHECK_EQUAL("\x03", Serialization::LengthToBytes(3));
  BOOST_CHECK_EQUAL("\x7f", Serialization::LengthToBytes(127));
  {
    std::string expected("\x81");
    expected.push_back('\x00');
    BOOST_CHECK_EQUAL(expected, Serialization::LengthToBytes(128));
  }
  BOOST_CHECK_EQUAL("\x81\x01", Serialization::LengthToBytes(129));
  BOOST_CHECK_EQUAL("\x81\x48", Serialization::LengthToBytes(200));
  BOOST_CHECK_EQUAL("\x93\x96\x2f", Serialization::LengthToBytes(314159));
}

}
}

