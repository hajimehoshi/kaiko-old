#include "../Util/Exception.hpp"
#include "../Util/Strings.hpp"

#include <boost/test/unit_test.hpp>

#include <iostream>

namespace Kaiko {
namespace Tests {

using namespace Kaiko::Util;

BOOST_AUTO_TEST_CASE(Util_Strings_UTF8ToUTF16LE) {
  {
    std::wstring expected = L"foo";
    std::wstring actual = Strings::UTF8ToUTF16LE("foo");
    BOOST_CHECK(expected == actual);
  }
  {
    std::wstring expected = L"caf\u00e9";
    std::wstring actual = Strings::UTF8ToUTF16LE("caf\xc3\xa9");
    BOOST_CHECK(expected == actual);
  }
  {
    std::wstring expected = L"\u30c6\u30b9\u30c8";
    std::wstring actual = Strings::UTF8ToUTF16LE("\xe3\x83\x86\xe3\x82\xb9\xe3\x83\x88");
    BOOST_CHECK(expected == actual);
  }
  {
    BOOST_CHECK_THROW(Strings::UTF8ToUTF16LE("caf\xc3"), Exception);
    BOOST_CHECK_THROW(Strings::UTF8ToUTF16LE("\xe3\x83\x86\xe3\x82\xb9\xe3"), Exception);
    BOOST_CHECK_THROW(Strings::UTF8ToUTF16LE("\xe3\x83\x86\xe3\x82\xb9\xe3\x83"), Exception);
    BOOST_CHECK_THROW(Strings::UTF8ToUTF16LE("\xff"), Exception);
    BOOST_CHECK_THROW(Strings::UTF8ToUTF16LE("\xff\xff"), Exception);
    BOOST_CHECK_THROW(Strings::UTF8ToUTF16LE("\xff\xff\xff"), Exception);
  }
}

}
}
