#include <stdio.h>
#include <stdlib.h>

#include "check.h"


#define CTL_TYPE int
#include "vector.h"


START_TEST (add_int_into_vector_test)
{
    struct int_v testVec = {
        .store=malloc (64),
        .len=0, .off=0, .max=64
    };

    vec_add_int (&testVec, 0, 0);
    vec_add_int (&testVec, 1, 1);
    vec_add_int (&testVec, 0, 2);
    vec_add_int (&testVec, 2, 3);
    vec_add_int (&testVec, 1, 4);

    ck_assert_int_eq (testVec.store[0], 0);
    ck_assert_int_eq (testVec.store[1], 3);
    ck_assert_int_eq (testVec.store[2], 1);
    ck_assert_int_eq (testVec.store[14], 2);
    ck_assert_int_eq (testVec.store[15], 4);


    free (testVec.store);
}
END_TEST

START_TEST (get_int_from_vector_test)
{
    int VALUES[16];
    memcpy (VALUES, (int[]) {0, 1}, 2 * sizeof(int));
    memcpy (VALUES + 14, (int[]) {14, 15}, 2 * sizeof(int));

    struct int_v testVec = {
        .store = VALUES,
        .len=16, .off=56, .max=64
    };

    ck_assert_int_eq ( vec_get_int (&testVec, 0), 14 );
    ck_assert_int_eq ( vec_get_int (&testVec, 1), 15 );
    ck_assert_int_eq ( vec_get_int (&testVec, 2), 0 );
    ck_assert_int_eq ( vec_get_int (&testVec, 3), 1 );
}
END_TEST


static Suite* make_suite (void) {
    Suite* newSuite = suite_create ("Vector Testing Suite.");

    TCase* addCase = tcase_create ("Trying to add integers to vector");
    tcase_add_test (addCase, add_int_into_vector_test);
    suite_add_tcase (newSuite, addCase);

    TCase* getCase = tcase_create ("Trying to retrive integers from vector");
    tcase_add_test (getCase, get_int_from_vector_test);
    suite_add_tcase (newSuite, getCase);


    return newSuite;
}

int main (void) {
    Suite* testSuite = make_suite ();
    SRunner* runner = srunner_create (testSuite);

    srunner_run_all (runner, CK_VERBOSE);
    const int failed = srunner_ntests_failed (runner);

    srunner_free (runner);

    return failed;
}

