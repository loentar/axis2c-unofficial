#include <axutil_date_time.h>
#include <time.h>
#include <axutil_date_time_util.h>
#include <stdio.h>
#include <axutil_env.h>
#include "../util/create_env.h"
#include "../test_common/axis2c_test_macros.h"

/** @brief test_rand 
 *  *   deserialize and serialize the time 
 *   */

void test_date_time_deserialize_time(axutil_env_t *env)
{
	START_TEST_CASE("test_date_time_deserialize_time");

	axutil_date_time_t *date_time = NULL;
	axis2_char_t *time_str = "22:45:12";
    axis2_status_t status = AXIS2_FAILURE;

    date_time = axutil_date_time_create(env);

    TEST_ASSERT_VOID(date_time);

    status = axutil_date_time_deserialize_time(date_time, env, time_str);

    EXPECT_EQ(status,AXIS2_SUCCESS);
    EXPECT_EQ(axutil_date_time_get_hour(date_time,env),22);
    EXPECT_EQ(axutil_date_time_get_minute(date_time,env),45);
    EXPECT_EQ(axutil_date_time_get_second(date_time,env),12);

    axutil_date_time_free(date_time,env);

    END_TEST_CASE();
}

void test_date_time_deserialize_date(axutil_env_t *env)
{
	START_TEST_CASE("test_date_time_deserialize_date");

	axutil_date_time_t *date_time = NULL;
    axis2_char_t *date_str = "2000-11-12";
    axis2_status_t status = AXIS2_FAILURE;

    date_time = axutil_date_time_create(env);

    TEST_ASSERT_VOID(date_time);

    status = axutil_date_time_deserialize_date(date_time, env, date_str);

    EXPECT_EQ(status,AXIS2_SUCCESS);
    EXPECT_EQ(axutil_date_time_get_year(date_time,env),2000);
    EXPECT_EQ(axutil_date_time_get_month(date_time,env),11);
    EXPECT_EQ(axutil_date_time_get_date(date_time,env),12);

    axutil_date_time_free(date_time,env);

    END_TEST_CASE();
}

void test_axutil_date_time_utc_to_local(axutil_env_t *env)
{
	START_TEST_CASE("test_axutil_date_time_utc_to_local");

	axutil_date_time_t *date_time = NULL;
	axis2_status_t status = AXIS2_FAILURE;

	date_time = axutil_date_time_create(env);

	TEST_ASSERT_VOID(date_time);

	status = axutil_date_time_set_date_time(date_time, env, 2000, 1, 1, 12, 18, 57, 799);
	axutil_date_time_set_time_zone(date_time, env,AXIS2_TRUE,10,0);

	EXPECT_EQ(status,AXIS2_SUCCESS);

	axutil_date_time_t *adjusted_time = axutil_date_time_utc_to_local(date_time,env,AXIS2_TRUE,10,00);

	TEST_ASSERT_VOID(adjusted_time);

	EXPECT_EQ(axutil_date_time_get_year(adjusted_time,env),2000);
	EXPECT_EQ(axutil_date_time_get_month(adjusted_time,env),1);
	EXPECT_EQ(axutil_date_time_get_date(adjusted_time,env),1);
	EXPECT_EQ(axutil_date_time_get_hour(adjusted_time,env),22);

	axutil_date_time_free(adjusted_time,env);

    adjusted_time = axutil_date_time_utc_to_local(date_time,env,AXIS2_FALSE,22,00);

	TEST_ASSERT_VOID(adjusted_time);

	EXPECT_EQ(axutil_date_time_get_year(adjusted_time,env),1999);
	EXPECT_EQ(axutil_date_time_get_month(adjusted_time,env),12);
	EXPECT_EQ(axutil_date_time_get_date(adjusted_time,env),31);
	EXPECT_EQ(axutil_date_time_get_hour(adjusted_time,env),14);

    axutil_date_time_free(adjusted_time,env);

    adjusted_time = axutil_date_time_utc_to_local(date_time,env,AXIS2_TRUE,12,00);

	TEST_ASSERT_VOID(adjusted_time);

	EXPECT_EQ(axutil_date_time_get_year(adjusted_time,env),2000);
	EXPECT_EQ(axutil_date_time_get_month(adjusted_time,env),1);
	EXPECT_EQ(axutil_date_time_get_date(adjusted_time,env),2);
	EXPECT_EQ(axutil_date_time_get_hour(adjusted_time,env),0);


	axutil_date_time_free(adjusted_time,env);

	END_TEST_CASE();
}


void test_date_time_deserialize_date_time(axutil_env_t *env)
{
	START_TEST_CASE("test_date_time_deserialize_date_time");
    axutil_date_time_t *date_time = NULL;
    axutil_date_time_t *ref = NULL;
	axutil_date_time_comp_result_t compare_res = AXIS2_DATE_TIME_COMP_RES_FAILURE;
    axis2_char_t *date_time_str = "2000-11-12T12:30:24";
    axis2_status_t status = AXIS2_FAILURE;

    date_time = axutil_date_time_create(env);

	TEST_ASSERT_VOID(date_time);

    status = axutil_date_time_deserialize_date_time(date_time, env, date_time_str);

    EXPECT_EQ(status,AXIS2_SUCCESS);

    ref = axutil_date_time_create(env);

    TEST_ASSERT_VOID(ref);

    compare_res = axutil_date_time_compare(date_time, env, ref);

    EXPECT_NEQ(compare_res,AXIS2_DATE_TIME_COMP_RES_FAILURE);

    status = axutil_date_time_deserialize_date_time(ref, env, date_time_str);

    EXPECT_EQ(status,AXIS2_SUCCESS);

    compare_res = axutil_date_time_compare(date_time, env, ref);

    EXPECT_EQ(compare_res,AXIS2_DATE_TIME_COMP_RES_EQUAL);

    axis2_char_t *date_time2_str = "2000-11-12T12:77:24";/*Error time*/

    status = axutil_date_time_deserialize_time_with_time_zone(date_time, env, date_time2_str);

    EXPECT_EQ(status,AXIS2_FAILURE);


    axutil_date_time_free(date_time,env);
    axutil_date_time_free(ref, env);

	END_TEST_CASE();
}

void test_date_time_deserialize_time_with_time_zone(axutil_env_t *env)
{
	START_TEST_CASE("test_axutil_date_time_deserialize_time_with_time_zone");
    axutil_date_time_t *date_time = NULL;
    axis2_char_t *date_time_str = "12:30:24+10:30";

    axis2_status_t status = AXIS2_FAILURE;

    date_time = axutil_date_time_create(env);

	TEST_ASSERT_VOID(date_time);

    status = axutil_date_time_deserialize_time_with_time_zone(date_time, env, date_time_str);

    EXPECT_EQ(status,AXIS2_SUCCESS);

    axis2_char_t *date_time2_str = "12:30:24+10:70";/*Error timezone*/

    status = axutil_date_time_deserialize_time_with_time_zone(date_time, env, date_time2_str);

    EXPECT_EQ(status,AXIS2_FAILURE);

    axis2_char_t *date_time3_str = "12:30:24+20:30";/*Error timezone*/

    status = axutil_date_time_deserialize_time_with_time_zone(date_time, env, date_time3_str);

    EXPECT_EQ(status,AXIS2_FAILURE);

    axutil_date_time_free(date_time,env);

	END_TEST_CASE();
}

void test_date_time(axutil_env_t *env)
{
	START_TEST_CASE("test_date_time");
    axutil_date_time_t *date_time = NULL;
    axutil_date_time_t *ref = NULL;
    axutil_date_time_t *date_time_offset = NULL;

    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *t_str = NULL, *d_str = NULL, *dt_str = NULL;
    int year , month , date , hour , min , sec , msec;

    date_time_offset = axutil_date_time_create_with_offset(env, 100);

    TEST_ASSERT_VOID(date_time_offset);

    ref = axutil_date_time_create(env);

    TEST_ASSERT_VOID(ref);

    date_time = axutil_date_time_create(env);

    TEST_ASSERT_VOID(date_time);

    /*Set date time*/
    status = axutil_date_time_set_date_time(date_time, env, 2008, 1, 8, 12, 18, 57, 799);
    
    EXPECT_EQ(status,AXIS2_SUCCESS);

    t_str = axutil_date_time_serialize_time(date_time, env);

    EXPECT_NOT_NULL(t_str);
    EXPECT_STREQ(t_str,"12:18:57.799Z");
    AXIS2_FREE(env->allocator,t_str);

    d_str = axutil_date_time_serialize_date(date_time, env);

    EXPECT_NOT_NULL(d_str);
    EXPECT_STREQ(d_str,"2008-01-08");
    AXIS2_FREE(env->allocator,d_str);

    dt_str = axutil_date_time_serialize_date_time(date_time, env);

    EXPECT_NOT_NULL(dt_str);
    EXPECT_STREQ(dt_str,"2008-01-08T12:18:57.799Z");
    AXIS2_FREE(env->allocator,dt_str);

    year = axutil_date_time_get_year(date_time,env);
    month=axutil_date_time_get_month(date_time,env);
    date = axutil_date_time_get_date(date_time,env);
    hour = axutil_date_time_get_hour(date_time,env);
    min  = axutil_date_time_get_minute(date_time,env);
    sec  = axutil_date_time_get_second(date_time,env);
    msec = axutil_date_time_get_msec(date_time,env);

    EXPECT_EQ(year,2008);
    EXPECT_EQ(month,1);
    EXPECT_EQ(date,8);
    EXPECT_EQ(hour,12);
    EXPECT_EQ(min,18);
    EXPECT_EQ(sec,57);
    EXPECT_EQ(msec,799);

    axutil_date_time_free(date_time,env);
    axutil_date_time_free(ref, env);
    axutil_date_time_free(date_time_offset, env);

    END_TEST_CASE();
}

int main()
{
    START_TEST();
    axutil_env_t * env = create_environment();

    test_date_time(env);
    test_date_time_deserialize_time(env);
    test_date_time_deserialize_date(env);
    test_date_time_deserialize_date_time(env);
    test_date_time_deserialize_time_with_time_zone(env);
    test_axutil_date_time_utc_to_local(env);

    axutil_env_free(env);
    END_TEST();
    return 0;
}

