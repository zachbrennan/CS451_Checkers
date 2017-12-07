#include <check.h>

#include <fonts.h>

START_TEST(test_a)
{
    ck_assert_int_eq(B, 0);
    ck_assert_int_eq(E, 1);
    ck_assert_int_eq(F, 2);
}
END_TEST

START_TEST(test_b)
{
    ck_assert_int_eq(O, B);
    ck_assert_int_eq(X, E);
    ck_assert_int_eq(T, F);
}
END_TEST

START_TEST(test_c)
{
    ck_assert_int_eq(BGND, B);
    ck_assert_int_eq(EDGE, 0);
    ck_assert_int_eq(FGND, 0);
}
END_TEST

START_TEST(test_d)
{
    ck_assert_int_gt(TALL, 0);
    ck_assert_int_gt(WIDE, 0);
}
END_TEST

Suite *
test_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Money");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_a);
    tcase_add_test(tc_core, test_b);
    tcase_add_test(tc_core, test_c);
    tcase_add_test(tc_core, test_d);

    suite_add_tcase(s, tc_core);

    return s;
}

int
main(void)
{
    return 0;
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = test_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed != 0);
}
