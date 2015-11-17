/**
 * @file TestLib/test.h
 * Библиотека тестирования.
 *
 * $Revision: 971 $
 * $Author: stabur $
 * $Date: 2015-05-25 19:49:57 +0300 (Пн, 25 май 2015) $
 */

#ifndef TESTLIB_H
#define TESTLIB_H

#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Assert Macros
 */
#define assert(expression) \
    ( testlib_assert_result((expression), ("assert(" #expression ")"), __FILE__, __LINE__) )
#define assert_true(value) \
    ( testlib_assert_result((value), ("assert_true(" #value ")"), __FILE__, __LINE__) )
#define assert_false(value) \
    ( testlib_assert_result(!(value), ("assert_false(" #value ")"), __FILE__, __LINE__) )
#define assert_equal(actual, expected) \
    ( testlib_assert_result((actual) == (expected), ("assert_equal(" #actual ", " #expected ")"), __FILE__, __LINE__) )
#define assert_not_equal(actual, expected) \
    ( testlib_assert_result((actual) != (expected), ("assert_not_equal(" #actual ", " #expected ")"), __FILE__, __LINE__) )
#define assert_null(value) \
    ( testlib_assert_result((void*)(value) == 0, ("assert_null(" #value ")"), __FILE__, __LINE__) )
#define assert_not_null(value) \
    ( testlib_assert_result((void*)(value) != 0, ("assert_not_null(" #value ")"), __FILE__, __LINE__) )
#define assert_string_equal(actual, expected) \
    ( testlib_assert_result(!(strcmp((const char*)(actual), (const char*)(expected))), ("assert_string_equal(" #actual ", " #expected ")"), __FILE__, __LINE__) )
#define assert_string_not_equal(actual, expected) \
    ( testlib_assert_result((strcmp((const char*)(actual), (const char*)(expected))), ("assert_string_not_equal(" #actual ", " #expected ")"), __FILE__, __LINE__) )
#define assert_double_equal(actual, expected, delta) \
    ( testlib_assert_result(((fabs((double)(actual) - (expected)) <= fabs((double)(delta)))), ("assert_double_equal(" #actual ", " #expected ", " #delta ")"), __FILE__, __LINE__) )
#define assert_double_not_equal(actual, expected, delta) \
    ( testlib_assert_result(((fabs((double)(actual) - (expected)) > fabs((double)(delta)))), ("assert_double_not_equal(" #actual ", " #expected ", " #delta ")"), __FILE__, __LINE__) )
#define assert_binary_equal(actual, expected, size) \
    ( testlib_assert_result(!(memcmp((const void*)(actual), (const void*)(expected), (size_t)(size))), ("assert_binary_equal(" #actual ", " #expected ", " #size ")"), __FILE__, __LINE__) )
#define assert_binary_not_equal(actual, expected, size) \
    ( testlib_assert_result((memcmp((const void*)(actual), (const void*)(expected), (size_t)(size))), ("assert_binary_not_equal(" #actual ", " #expected ", " #size ")"), __FILE__, __LINE__) )
#define assert_fail(message) \
    ( testlib_assert_result(0, ("assert_fail(" #message ")"), __FILE__, __LINE__) )

/*
 * Tests Management
 */
#define suite_setup(setup) \
    ( testlib_suite_setup((testlib_void_function)setup) )
#define suite_teardown(teardown) \
    ( testlib_suite_teardown((testlib_void_function)teardown) )
#define fixture_setup(setup) \
    ( testlib_fixture_setup((testlib_void_function)setup) )
#define fixture_teardown(teardown) \
    ( testlib_fixture_teardown((testlib_void_function)teardown) )
#define run_test(test) \
    ( testlib_run_test((testlib_void_function)test, #test) )
#define run_fixture(fixture) \
    ( testlib_run_fixture((testlib_void_function)fixture, #fixture) )
#define run_tests(tests) \
    ( testlib_run_tests((testlib_void_function)tests) )

/*
 * Internal
 */
typedef void (*testlib_assert_result_function)(int expression, const char* message, const char* file, unsigned int line);
typedef void (*testlib_void_function)(void);

extern testlib_assert_result_function testlib_assert_result;

void testlib_suite_setup(testlib_void_function setup);
void testlib_suite_teardown(testlib_void_function teardown);
void testlib_fixture_setup(testlib_void_function setup);
void testlib_fixture_teardown(testlib_void_function teardown);
void testlib_run_test(testlib_void_function test, const char* name);
void testlib_run_fixture(testlib_void_function fixture, const char* name);
int  testlib_run_tests(testlib_void_function tests);

#ifdef __cplusplus
}
#endif

#endif  /* TEST_LIB_H */
