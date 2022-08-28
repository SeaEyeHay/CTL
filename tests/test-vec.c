#include <stdio.h>
#include <stdlib.h>

#include "check.h"


#define CTL_TYPE int
#include "deque.h"


START_TEST (add_int_into_deqtor_test)
{
    struct int_d testVec = {
        .store=malloc (64),
        .len=0, .off=0, .max=64
    };

    deq_add_int (&testVec, 0, 0);
    deq_add_int (&testVec, 1, 1);
    deq_add_int (&testVec, 0, 2);
    deq_add_int (&testVec, 2, 3);
    deq_add_int (&testVec, 1, 4);

    ck_assert_int_eq (testVec.store[0], 0);
    ck_assert_int_eq (testVec.store[1], 3);
    ck_assert_int_eq (testVec.store[2], 1);
    ck_assert_int_eq (testVec.store[14], 2);
    ck_assert_int_eq (testVec.store[15], 4);


    free (testVec.store);
}
END_TEST

START_TEST (get_int_from_deqtor_test)
{
    int VALUES[16];
    memcpy (VALUES, (int[]) {0, 1}, 2 * sizeof(int));
    memcpy (VALUES + 14, (int[]) {14, 15}, 2 * sizeof(int));

    struct int_d testVec = {
        .store = VALUES,
        .len=16, .off=56, .max=64
    };

    ck_assert_int_eq ( deq_get_int (&testVec, 0), 14 );
    ck_assert_int_eq ( deq_get_int (&testVec, 1), 15 );
    ck_assert_int_eq ( deq_get_int (&testVec, 2), 0 );
    ck_assert_int_eq ( deq_get_int (&testVec, 3), 1 );
}
END_TEST

START_TEST (create_deqtor_test)
{
    struct int_d emptyVec = make_int_deq (0);

    ck_assert_ptr_null (emptyVec.store);
    ck_assert_int_eq (emptyVec.len, 0);
    ck_assert_int_eq (emptyVec.off, 0);
    ck_assert_int_eq (emptyVec.max, 0);

    // Allocate on add
    deq_add_int (&emptyVec, 0, 0);

    ck_assert_ptr_nonnull (emptyVec.store);
    ck_assert_int_gt (emptyVec.max, sizeof(int));


    struct int_d initVec = make_int_deq (7);
    
    ck_assert_ptr_nonnull (initVec.store);
    ck_assert_int_eq (initVec.len, 0);
    ck_assert_int_eq (initVec.off, 0);
    ck_assert_int_eq (initVec.max, 32);


    free_int_deq (&initVec);

    ck_assert_ptr_null (initVec.store);
    ck_assert_int_eq (initVec.max, 0);
}
END_TEST

START_TEST (push_and_pop_int_into_deqto_test)
{
    struct int_d testVec = make_int_deq (1);


    deq_push_int (&testVec, 0);
    deq_push_int (&testVec, 1);

    ck_assert_int_eq ( deq_get_int(&testVec, 0), 0 );
    ck_assert_int_eq ( deq_get_int(&testVec, 1), 1 );

    ck_assert_int_eq (testVec.len, 8);
    ck_assert_int_eq (testVec.max, 8);


    deq_push_front_int (&testVec, 2);
    deq_push_front_int (&testVec, 3);

    ck_assert_int_eq ( deq_get_int(&testVec, 0), 3 );
    ck_assert_int_eq ( deq_get_int(&testVec, 1), 2 );

    ck_assert_int_eq (testVec.len, 16);
    ck_assert_int_eq (testVec.max, 16);


    const int pop1 = deq_pop_int (&testVec);
    const int pop2 = deq_pop_int (&testVec);

    const int popFront1 = deq_pop_front_int (&testVec);
    const int popFront2 = deq_pop_front_int (&testVec);

    ck_assert_int_eq (pop1, 1);
    ck_assert_int_eq (pop2, 0);
    ck_assert_int_eq (popFront1, 3);
    ck_assert_int_eq (popFront2, 2);

    ck_assert_int_eq (testVec.len, 0);


    free_int_deq (&testVec);
}
END_TEST


static Suite* make_suite (void) {
    Suite* newSuite = suite_create ("Vector Testing Suite.");

    TCase* addCase = tcase_create ("Trying to add integers to deqtor");
    tcase_add_test (addCase, add_int_into_deqtor_test);
    suite_add_tcase (newSuite, addCase);

    TCase* getCase = tcase_create ("Trying to retreive integers from deqtor");
    tcase_add_test (getCase, get_int_from_deqtor_test);
    suite_add_tcase (newSuite, getCase);

    TCase* createCase = tcase_create ("Trying to create and delete deqtors");
    tcase_add_test (createCase, create_deqtor_test);
    suite_add_tcase (newSuite, createCase);

    TCase* pushCase = tcase_create ("Trying to push and pop integers");
    tcase_set_tags (pushCase, "push");
    tcase_add_test (pushCase, push_and_pop_int_into_deqto_test);
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

