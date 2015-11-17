/**
 * @file TestLib/test.c
 * Библиотека тестирования.
 *
 * $Revision: 971 $
 * $Author: stabur $
 * $Date: 2015-05-25 19:49:57 +0300 (Пн, 25 май 2015) $
 */

#include <TestLib/test.h>

#include <stdio.h>

/* Compiler specific error format */
#ifdef _MSC_VER
const char testlib_error_format[] = "%s(%u) : error C2999: %s in %s\n";
#else
const char testlib_error_format[] = "%s:%u: error: %s in %s\n";
#endif

const char testlib_fixture_delimiter[] = "------------------------------\n";
const char testlib_suite_delimiter[]   = "==============================\n";

/*
 * Variables
 */
static int testlib_asserts_failed = 0;
static int testlib_asserts_passed = 0;

static int testlib_tests_failed = 0;
static int testlib_tests_passed = 0;

static int testlib_fixtures_failed = 0;
static int testlib_fixtures_passed = 0;

static testlib_void_function testlib_suite_setup_func      = 0;
static testlib_void_function testlib_suite_teardown_func   = 0;
static testlib_void_function testlib_fixture_setup_func    = 0;
static testlib_void_function testlib_fixture_teardown_func = 0;


/*
 * Error reporting
 */
void testlib_assert_result_log(int expression, const char* message, const char* file, const char* function, unsigned int line)
{
    if (!expression)
    {
        printf(testlib_error_format, file, line, message, function);
        testlib_asserts_failed++;
    }
    else
    {
        testlib_asserts_passed++;
    }
}

testlib_assert_result_function testlib_assert_result = testlib_assert_result_log;

/*
 * Tests Management
 */
void testlib_suite_setup(testlib_void_function setup)
{
    testlib_suite_setup_func = setup;
}

void testlib_suite_teardown(testlib_void_function teardown)
{
    testlib_suite_teardown_func = teardown;
}

void testlib_fixture_setup(testlib_void_function setup)
{
    testlib_fixture_setup_func = setup;
}

void testlib_fixture_teardown(testlib_void_function teardown)
{
    testlib_fixture_teardown_func = teardown;
}

void testlib_run_test(testlib_void_function test, const char* name)
{
    int testlib_init_asserts_failed = testlib_asserts_failed;
    int testlib_init_asserts_passed = testlib_asserts_passed;
    int testlib_test_asserts_failed = 0;
    int testlib_test_asserts_passed = 0;
    int testlib_test_asserts_total  = 0;

    if (testlib_suite_setup_func != 0) testlib_suite_setup_func();
    if (testlib_fixture_setup_func != 0) testlib_fixture_setup_func();
    test();
    if (testlib_suite_teardown_func != 0) testlib_suite_teardown_func();
    if (testlib_fixture_teardown_func != 0) testlib_fixture_teardown_func();

    testlib_test_asserts_failed = testlib_asserts_failed - testlib_init_asserts_failed;
    testlib_test_asserts_passed = testlib_asserts_passed - testlib_init_asserts_passed;
    testlib_test_asserts_total  = testlib_test_asserts_failed + testlib_test_asserts_passed;

    if (testlib_test_asserts_failed > 0)
    {
        printf("Test %s: FAILED %d asserts of %d\n", name, testlib_test_asserts_failed, testlib_test_asserts_total);
        testlib_tests_failed++;
    }
    else
    {
        printf("Test %s: PASSED %d asserts of %d\n", name, testlib_test_asserts_passed, testlib_test_asserts_total);
        testlib_tests_passed++;
    }
}

void testlib_run_fixture(testlib_void_function fixture, const char* name)
{
    int testlib_init_tests_failed = testlib_tests_failed;
    int testlib_init_tests_passed = testlib_tests_passed;
    int testlib_fixture_tests_failed = 0;
    int testlib_fixture_tests_passed = 0;
    int testlib_fixture_tests_total  = 0;

    testlib_fixture_setup_func    = 0;
    testlib_fixture_teardown_func = 0;

    printf("Fixture %s\n", name);
    printf(testlib_fixture_delimiter);

    fixture();

    testlib_fixture_tests_failed = testlib_tests_failed - testlib_init_tests_failed;
    testlib_fixture_tests_passed = testlib_tests_passed - testlib_init_tests_passed;
    testlib_fixture_tests_total  = testlib_fixture_tests_failed + testlib_fixture_tests_passed;

    printf(testlib_fixture_delimiter);
    if (testlib_fixture_tests_failed > 0)
    {
        printf("FAILED %d tests of %d\n", testlib_fixture_tests_failed, testlib_fixture_tests_total);
        testlib_fixtures_failed++;
    }
    else
    {
        printf("PASSED %d tests of %d\n", testlib_fixture_tests_passed, testlib_fixture_tests_total);
        testlib_fixtures_passed++;
    }
}

int testlib_run_tests(testlib_void_function tests)
{
    tests();

    printf(testlib_suite_delimiter);
    if (testlib_asserts_failed > 0)
    {
        printf("Suite FAILED\n");
    }
    else
    {
        printf("Suite PASSED\n");
    }

    return (testlib_asserts_failed != 0);
}
