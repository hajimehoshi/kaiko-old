#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#ifdef _MSC_VER

#include <iostream>

static class Finalizer {
public:
  ~Finalizer() {
    std::cout.flush();
    std::cin.get();
  }
} finalizer;

#endif

