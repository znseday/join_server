#define BOOST_TEST_MODULE join_test_module

#include "common.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(join_server_test_suite)

BOOST_AUTO_TEST_CASE(join_server_test_boost_test)
{
    BOOST_CHECK(1 > 0);
}


BOOST_AUTO_TEST_SUITE_END()
