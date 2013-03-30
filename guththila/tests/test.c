#include <check.h>
#include <string.h>
#include <guththila.h>
#include "guththila_defines.h"
#include "test.h"

void
setup(
    void)
{
    allocator = axutil_allocator_init(NULL);
    env = axutil_env_create(allocator);
}

void
teardown(
    void)
{
    guththila_reader_free(red, env);
    guththila_un_init(parser, env);
    axutil_env_free(env);
}

START_TEST(test_guththila)
{
    red = guththila_reader_create_for_file("resources/om/axis.xml", env);
    parser =
        (guththila_t *) AXIS2_MALLOC(env->allocator, sizeof(guththila_t));
    int init_status = guththila_init(parser, red, env);
    fail_if(red == NULL, "guththila reader failed");
    fail_if(parser == NULL, "guththila parser failed");
    fail_if(init_status != GUTHTHILA_SUCCESS, "guththila parser failed");
}

END_TEST
START_TEST(
    test_guththila_start_element)
{
    int c = 0;
    char *p;
    red = guththila_reader_create_for_file("resources/om/axis.xml", env);
    parser =
        (guththila_t *) AXIS2_MALLOC(env->allocator, sizeof(guththila_t));
    guththila_init(parser, red, env);
    //guththila_read(env, parser);
    c = guththila_next(parser, env);

    while ((c != GUTHTHILA_START_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless((c == GUTHTHILA_START_ELEMENT), "no start element found");
    fail_if((p == NULL), "no name found");
    fail_unless(!strcmp(p, "root"), "root element differed");
    AXIS2_FREE(env->allocator, p);
    c = 0;

    while ((c != GUTHTHILA_START_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless((c == GUTHTHILA_START_ELEMENT), "no start element found");
    fail_if((p == NULL), "no name found");
    fail_unless(!strcmp(p, "a"), "a element differed");
    AXIS2_FREE(env->allocator, p);

    c = 0;
    while ((c != GUTHTHILA_START_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless(!strcmp(p, "b"), "b element differed");
    AXIS2_FREE(env->allocator, p);
}

END_TEST
START_TEST(
    test_guththila_empty_element)
{
    int c = 0;
    char *p;
    red = guththila_reader_create_for_file("resources/om/axis.xml", env);
    parser =
        (guththila_t *) AXIS2_MALLOC(env->allocator, sizeof(guththila_t));
    guththila_init(parser, red, env);
    //guththila_read(env, parser);
    c = guththila_next(parser, env);

    while ((c != GUTHTHILA_EMPTY_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless((c == GUTHTHILA_EMPTY_ELEMENT), "no empty element found");
    fail_if((p == NULL), "no name found");
    fail_unless(!strcmp(p, "a.1"), "a.1 element differed");
    AXIS2_FREE(env->allocator, p);

    c = 0;

    while ((c != GUTHTHILA_EMPTY_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless((c == GUTHTHILA_EMPTY_ELEMENT), "no empty element found");
    fail_if((p == NULL), "no name found");
    fail_unless(!strcmp(p, "a.2"), "a.2 element differed");
    AXIS2_FREE(env->allocator, p);

    c = 0;
    while ((c != GUTHTHILA_START_ELEMENT))
        c = guththila_next(parser, env);

    c = 0;
    while ((c != GUTHTHILA_EMPTY_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless((c == GUTHTHILA_EMPTY_ELEMENT), "no empty element found");
    fail_if((p == NULL), "no name found");
    fail_unless(!strcmp(p, "b.1"), "b.1 element differed");
    AXIS2_FREE(env->allocator, p);
}

END_TEST
START_TEST(
    test_guththila_end_element)
{
    int c = 0;
    char *p;
    red = guththila_reader_create_for_file("resources/om/axis.xml", env);
    parser =
        (guththila_t *) AXIS2_MALLOC(env->allocator, sizeof(guththila_t));
    guththila_init(parser, red, env);
    //guththila_read(env, parser);
    c = guththila_next(parser, env);

    while ((c != GUTHTHILA_END_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless((c == GUTHTHILA_END_ELEMENT), "no end element found");
    fail_if((p == NULL), "no name found");
    fail_unless(!strcmp(p, "a"), "a element differed");
    AXIS2_FREE(env->allocator, p);

    c = 0;
    while ((c != GUTHTHILA_END_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless((c == GUTHTHILA_END_ELEMENT), "no endelement found");
    fail_if((p == NULL), "no name found");
    fail_unless(!strcmp(p, "b"), "b element differed");
    AXIS2_FREE(env->allocator, p);

    c = 0;
    while ((c != GUTHTHILA_END_ELEMENT))
        c = guththila_next(parser, env);
    p = guththila_get_name(parser, env);
    fail_unless((c == GUTHTHILA_END_ELEMENT), "no empty element found");
    fail_if((p == NULL), "no name found");
    fail_unless(!strcmp(p, "root"), "root element differed");
    AXIS2_FREE(env->allocator, p);
}

END_TEST
START_TEST(
    test_guththila_character)
{
    int c = 0;
    int i = 0;
    char *p;
    red = guththila_reader_create_for_file("resources/om/numbers.xml", env);
    parser =
        (guththila_t *) AXIS2_MALLOC(env->allocator, sizeof(guththila_t));
    guththila_init(parser, red, env);
    //guththila_read(env, parser);
    c = guththila_next(parser, env);
    while (i < 3)
    {
        if (c == GUTHTHILA_START_ELEMENT)
            i++;
        c = guththila_next(parser, env);
    }

    if (c == GUTHTHILA_CHARACTER)
        p = guththila_get_value(parser, env);
    fail_unless(!strcmp(p, "3"), "3 not found");
    AXIS2_FREE(env->allocator, p);

    c = 0;
    while ((c != GUTHTHILA_CHARACTER) || (c == GUTHTHILA_SPACE))
        c = guththila_next(parser, env);
    p = guththila_get_value(parser, env);
    fail_unless(!strcmp(p, "24"), "24 not found");
    AXIS2_FREE(env->allocator, p);
}
END_TEST Suite * guththila_suite(void)
{
    Suite *s = suite_create("Guththila");

    /* Core test case */
    TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_guththila);
    tcase_add_test(tc_core, test_guththila_start_element);
    tcase_add_test(tc_core, test_guththila_empty_element);
    tcase_add_test(tc_core, test_guththila_end_element);
    tcase_add_test(tc_core, test_guththila_character);
    suite_add_tcase(s, tc_core);
    return s;
}

int
main(
    void)
{
    int number_failed;
    Suite *s = guththila_suite();
    Suite *att = guththila_attribute_suite();
    Suite *utf8_encode = guththila_utf8_encoder_suite();
    Suite *utf8_decode = guththila_utf8_decoder_suite();
    SRunner *sr = srunner_create(s);
    srunner_add_suite(sr, att);
    srunner_add_suite(sr, utf8_encode);
    srunner_add_suite(sr, utf8_decode);
    srunner_set_log(sr, "guththila.log");
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
