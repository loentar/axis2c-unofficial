#include <guththila.h>
#include <check.h>
#include <string.h>
#include "test.h"

START_TEST(test_attribute)
{
    int count = 0;;
    int c = 0;
    guththila_attr_t *att;
    red = guththila_reader_create_for_file("resources/om/evaluate.xml", env);
    parser =
        (guththila_t *) AXIS2_MALLOC(env->allocator, sizeof(guththila_t));
    guththila_init(parser, red, env);
    //guththila_read(parser, env);
    c = guththila_next(parser, env);
    while (!count)
    {
        c = guththila_next(parser, env);
        count = guththila_get_attribute_count(parser, env);
        if (count)
            att = guththila_get_attribute(parser, env);
    }
    fail_if(count == 0, "guththila attribute count failed");
    fail_unless(!strcmp
                (guththila_get_attribute_name(parser, att, env), "color"),
                "guththila attribute name failed");
    fail_unless(!strcmp
                (guththila_get_attribute_value(parser, att, env), "brown"),
                "guththila attribute value failed");
}

END_TEST
START_TEST(
    test_attribute_prefix)
{
    int count = 0;;
    int c = 0;
    guththila_attr_t *att;
    red =
        guththila_reader_create_for_file("resources/soap/soapmessage.xml", env);
    parser =
        (guththila_t *) AXIS2_MALLOC(env->allocator, sizeof(guththila_t));
    guththila_init(parser, red, env);
    //guththila_read(parser, env);
    c = guththila_next(parser, env);
    while (!count)
    {
        c = guththila_next(parser, env);
        count = guththila_get_attribute_count(parser, env);
        if (count)
            att = guththila_get_attribute(parser, env);
    }
    fail_if(count == 0, "guththila attribute count failed");
    fail_unless(!strcmp
                (guththila_get_attribute_prefix(parser, att, env), "soapenv"),
                "guththila attribute count failed");
    fail_unless(!strcmp
                (guththila_get_attribute_name(parser, att, env),
                 "mustUnderstand"), "guththila attribute count failed");
    fail_unless(!strcmp(guththila_get_attribute_value(parser, att, env), "0"),
                "guththila attribute count failed");
    count = 0;
    while (!count)
    {
        c = guththila_next(parser, env);
        count = guththila_get_attribute_count(parser, env);
        if (count)
            att = guththila_get_attribute(parser, env);
    }
    fail_unless(!strcmp
                (guththila_get_attribute_prefix(parser, att, env), "soapenv"),
                "guththila attribute count failed");
    fail_unless(!strcmp
                (guththila_get_attribute_name(parser, att, env),
                 "mustUnderstand"), "guththila attribute count failed");
    fail_if(!strcmp(guththila_get_attribute_value(parser, att, env), "1"),
            "guththila attribute count failed");
}
END_TEST Suite * guththila_attribute_suite(void)
{
    Suite *s = suite_create("Guththila_attribute");

    /* Core test case */
    TCase *tc_core = tcase_create("attribute");
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_attribute);
    tcase_add_test(tc_core, test_attribute_prefix);
    suite_add_tcase(s, tc_core);
    return s;
}
