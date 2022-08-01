#include "check.h"
#include "circ-buffer.h"


START_TEST (testing_size_calculation)
{
    const size_t inputs[] = { 0, 8, 10 };

#if defined(CTL_USE_STD)
    const size_t outputs[] = { 0, 8, 16 };
#elif defined(CTL_USE_NOSTD)
    // TODO
#endif

    // Calculate the size of buffer fron input
    size_t results[3];
    for (int i = 0; i < 3; i++) results[i] = calc_buf_size (inputs[i]);

    // Assert the result of calculation is the one expected
    for (int i = 0; i < 3; i++) ck_assert_uint_eq (results[i], outputs[i]);
}
END_TEST

START_TEST (test_buffer_wrap)
{
    char* buffer = alloc_cbuf (16);
    char content[8] = "01234567";

    move_b_cbuf (buffer, 16, 0, content, sizeof(content), 4, sizeof(content));
    ck_assert_mem_eq (buffer, "45670123", sizeof(content));

    move_f_cbuf (buffer, 16, 2, buffer, 16, 4, 4);
    ck_assert_mem_eq (buffer, "6767", 4);
    ck_assert_mem_eq (buffer + 14, "45", 2);
}
END_TEST


Suite* make_suite (void) {
    Suite* suite = suite_create ("Circular Buffer test suite.");

    TCase* sizeCase = tcase_create ("Testing size calculation");
    tcase_add_test (sizeCase, testing_size_calculation);
    suite_add_tcase (suite, sizeCase);

    TCase* wrapCase = tcase_create ("Testing the memory circular wrapping.");
    tcase_add_test (wrapCase, test_buffer_wrap);
    suite_add_tcase (suite, wrapCase);


    return suite;
}

int main (void) {
    Suite* suite = make_suite ();
    SRunner* runner = srunner_create (suite);

    srunner_run_all (runner, CK_VERBOSE);
    const int failures = srunner_ntests_failed (runner);
    srunner_free (runner);


    return failures;
}

