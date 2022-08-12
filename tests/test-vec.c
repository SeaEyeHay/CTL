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

START_TEST (create_vector_test)
{
    struct int_v emptyVec = make_int_vec (0);

    ck_assert_ptr_null (emptyVec.store);
    ck_assert_int_eq (emptyVec.len, 0);
    ck_assert_int_eq (emptyVec.off, 0);
    ck_assert_int_eq (emptyVec.max, 0);

    // Allocate on add
    vec_add_int (&emptyVec, 0, 0);

    ck_assert_ptr_nonnull (emptyVec.store);
    ck_assert_int_gt (emptyVec.max, sizeof(int));


    struct int_v initVec = make_int_vec (7);
    
    ck_assert_ptr_nonnull (initVec.store);
    ck_assert_int_eq (initVec.len, 0);
    ck_assert_int_eq (initVec.off, 0);
    ck_assert_int_eq (initVec.max, 32);


    free_int_vec (&initVec);

    ck_assert_ptr_null (initVec.store);
    ck_assert_int_eq (initVec.max, 0);
}
END_TEST

START_TEST (push_and_pop_int_into_vecto_test)
{
    struct int_v testVec = make_int_vec (1);


    vec_push_int (&testVec, 0);
    vec_push_int (&testVec, 1);

    ck_assert_int_eq ( vec_get_int(&testVec, 0), 0 );
    ck_assert_int_eq ( vec_get_int(&testVec, 1), 1 );

    ck_assert_int_eq (testVec.len, 8);
    ck_assert_int_eq (testVec.max, 8);


    vec_push_front_int (&testVec, 2);
    vec_push_front_int (&testVec, 3);

    ck_assert_int_eq ( vec_get_int(&testVec, 0), 3 );
    ck_assert_int_eq ( vec_get_int(&testVec, 1), 2 );

    ck_assert_int_eq (testVec.len, 16);
    ck_assert_int_eq (testVec.max, 16);


    const int pop1 = vec_pop_int (&testVec);
    const int pop2 = vec_pop_int (&testVec);

    const int popFront1 = vec_pop_front_int (&testVec);
    const int popFront2 = vec_pop_front_int (&testVec);

    ck_assert_int_eq (pop1, 1);
    ck_assert_int_eq (pop2, 0);
    ck_assert_int_eq (popFront1, 3);
    ck_assert_int_eq (popFront2, 2);

    ck_assert_int_eq (testVec.len, 0);


    free_int_vec (&testVec);
}
END_TEST


static Suite* make_suite (void) {
    Suite* newSuite = suite_create ("Vector Testing Suite.");

    TCase* addCase = tcase_create ("Trying to add integers to vector");
    tcase_add_test (addCase, add_int_into_vector_test);
    suite_add_tcase (newSuite, addCase);

    TCase* getCase = tcase_create ("Trying to retreive integers from vector");
    tcase_add_test (getCase, get_int_from_vector_test);
    suite_add_tcase (newSuite, getCase);

    TCase* createCase = tcase_create ("Trying to create and delete vectors");
    tcase_add_test (createCase, create_vector_test);
    suite_add_tcase (newSuite, createCase);

    TCase* pushCase = tcase_create ("Trying to push and pop integers");
    tcase_set_tags (pushCase, "push");
    tcase_add_test (pushCase, push_and_pop_int_into_vecto_test);
    suite_add_tcase (newSuite, pushCase);


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

