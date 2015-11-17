#include <TestLib/test.h>

#define UNUSED(x) (void)(x)
#define without_logging(X) testlib_disable_logging(); X; testlib_enable_logging();
#define assert_test_passes(X) without_logging(X); assert(testlib_passed);
#define assert_test_fails(X)  without_logging(X); assert(!testlib_passed);

static int testlib_passed = 0;
static testlib_assert_result_function testlib_assert_result_log;

void testlib_assert_result_nolog(int expression, const char* message, const char* file, const char* function, unsigned int line)
{
    UNUSED(message);
    UNUSED(file);
    UNUSED(function);
    UNUSED(line);

    testlib_passed = expression;
}

void testlib_disable_logging()
{
    testlib_assert_result_log = testlib_assert_result;
    testlib_assert_result = testlib_assert_result_nolog;
}

void testlib_enable_logging()
{
    testlib_assert_result = testlib_assert_result_log;
}

/*
 * Tests
 */

void test_assert_true()
{
    assert_test_passes(assert_true(1));
    assert_test_passes(assert_true(2));
    assert_test_passes(assert_true(-1));

    assert_test_fails(assert_true(0));
}

void test_assert_false()
{
    assert_test_passes(assert_false(0));

    assert_test_fails(assert_false(1));
    assert_test_fails(assert_false(2));
    assert_test_fails(assert_false(-1));
}

void test_assert_equal()
{
    assert_test_passes(assert_equal(0, 0));
    assert_test_passes(assert_equal(1, 1));
    assert_test_passes(assert_equal(-1, -1));
    assert_test_passes(assert_equal(9223372036854775807L, 9223372036854775807L));
    assert_test_passes(assert_equal(18446744073709551614UL, 18446744073709551614UL));

    assert_test_fails(assert_equal(1, 0));
    assert_test_fails(assert_equal(1, -1));
    assert_test_fails(assert_equal(9223372036854775807L, -9223372036854775807L));
    assert_test_fails(assert_equal(18446744073709551614UL, 0));
}

void test_assert_not_equal()
{
    assert_test_passes(assert_not_equal(1, 0));
    assert_test_passes(assert_not_equal(1, -1));
    assert_test_passes(assert_not_equal(9223372036854775807L, -9223372036854775807L));
    assert_test_passes(assert_not_equal(18446744073709551614UL, 0));

    assert_test_fails(assert_not_equal(0, 0));
    assert_test_fails(assert_not_equal(1, 1));
    assert_test_fails(assert_not_equal(-1, -1));
    assert_test_fails(assert_not_equal(9223372036854775807L, 9223372036854775807L));
    assert_test_fails(assert_not_equal(18446744073709551614UL, 18446744073709551614UL));

}

void test_assert_null()
{
    int t;
    int *int_ptr  = &t;
    int *null_ptr = 0;

    assert_test_passes(assert_null(0));
    assert_test_passes(assert_null(NULL));
    assert_test_passes(assert_null(null_ptr));

    assert_test_fails(assert_null(int_ptr));
}

void test_assert_not_null()
{
    int t;
    int *int_ptr  = &t;
    int *null_ptr = 0;

    assert_test_passes(assert_not_null(int_ptr));

    assert_test_fails(assert_not_null(0));
    assert_test_fails(assert_not_null(NULL));
    assert_test_fails(assert_not_null(null_ptr));
}

void test_assert_string_equal()
{
    char buf[32] = "foo";

    assert_test_passes(assert_string_equal("", ""));
    assert_test_passes(assert_string_equal("foo", "foo"));
    assert_test_passes(assert_string_equal("foo", buf));

    assert_test_fails(assert_string_equal("foo", ""));
    assert_test_fails(assert_string_equal("foo", "bar"));
    assert_test_fails(assert_string_equal("bar", buf));
    assert_test_fails(assert_string_equal("foo", "Foo"));
    assert_test_fails(assert_string_equal("foo", "foo\n"));
}

void test_assert_string_not_equal()
{
    char buf[32] = "foo";

    assert_test_passes(assert_string_not_equal("foo", ""));
    assert_test_passes(assert_string_not_equal("foo", "bar"));
    assert_test_passes(assert_string_not_equal("bar", buf));
    assert_test_passes(assert_string_not_equal("foo", "Foo"));
    assert_test_passes(assert_string_not_equal("foo", "foo\n"));

    assert_test_fails(assert_string_not_equal("", ""));
    assert_test_fails(assert_string_not_equal("foo", "foo"));
    assert_test_fails(assert_string_not_equal("foo", buf));
}

void test_assert_double_equal()
{
    assert_test_passes(assert_double_equal(0.0, 0.0, 0.0));
    assert_test_passes(assert_double_equal(1e-10, 0.0, 1e-10));
    assert_test_passes(assert_double_equal(0.1, -0.1, 0.2));

    assert_test_fails(assert_double_equal(0.1, 0.0, 0.0));
    assert_test_fails(assert_double_equal(1e-9, 0.0, 1e-10));
    assert_test_fails(assert_double_equal(0.1, -0.1, 0.1));
}

void test_assert_double_not_equal()
{
    assert_test_passes(assert_double_not_equal(0.1, 0.0, 0.0));
    assert_test_passes(assert_double_not_equal(1e-9, 0.0, 1e-10));
    assert_test_passes(assert_double_not_equal(0.1, -0.1, 0.1));

    assert_test_fails(assert_double_not_equal(0.0, 0.0, 0.0));
    assert_test_fails(assert_double_not_equal(1e-10, 0.0, 1e-10));
    assert_test_fails(assert_double_not_equal(0.1, -0.1, 0.2));
}

void test_assert_binary_equal()
{
    char char_buf1[3] = { 1, 2, 3 };
    char char_buf2[3] = { 1, 2, 4 };

    int int_buf1[3] = { 1, 2, 3 };
    int int_buf2[3] = { 1, 2, 4 };

    assert_test_passes(assert_binary_equal(char_buf1, char_buf2, sizeof(char) * 2));
    assert_test_passes(assert_binary_equal(int_buf1, int_buf2, sizeof(int) * 2));
    assert_test_passes(assert_binary_equal(int_buf1, char_buf1, sizeof(char)));

    assert_test_fails(assert_binary_equal(char_buf1, char_buf2, sizeof(char) * 3));
    assert_test_fails(assert_binary_equal(int_buf1, int_buf2, sizeof(int) * 3));
    assert_test_fails(assert_binary_equal(int_buf1, char_buf1, sizeof(int)));
}

void test_assert_binary_not_equal()
{
    char char_buf1[3] = { 1, 2, 3 };
    char char_buf2[3] = { 1, 2, 4 };

    int int_buf1[3] = { 1, 2, 3 };
    int int_buf2[3] = { 1, 2, 4 };

    assert_test_passes(assert_binary_not_equal(char_buf1, char_buf2, sizeof(char) * 3));
    assert_test_passes(assert_binary_not_equal(int_buf1, int_buf2, sizeof(int) * 3));
    assert_test_passes(assert_binary_not_equal(int_buf1, char_buf1, sizeof(int)));

    assert_test_fails(assert_binary_not_equal(char_buf1, char_buf2, sizeof(char) * 2));
    assert_test_fails(assert_binary_not_equal(int_buf1, int_buf2, sizeof(int) * 2));
    assert_test_fails(assert_binary_not_equal(int_buf1, char_buf1, sizeof(char)));
}

void tests()
{
    run_test(test_assert_true);
    run_test(test_assert_false);

    run_test(test_assert_equal);
    run_test(test_assert_not_equal);

    run_test(test_assert_null);
    run_test(test_assert_not_null);

    run_test(test_assert_string_equal);
    run_test(test_assert_string_not_equal);

    run_test(test_assert_double_equal);
    run_test(test_assert_double_not_equal);

    run_test(test_assert_binary_equal);
    run_test(test_assert_binary_not_equal);
}

int main()
{
    return run_tests(tests);
}
