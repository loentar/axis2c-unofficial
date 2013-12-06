#include <stdio.h>
#include <axutil_env.h>
#include <axutil_properties.h>
#include "../test_common/axis2c_test_macros.h"

#define THREAD_AMMOUNT 100

void test_create_env()
{
    START_TEST_CASE("create_env");
    axutil_allocator_t *allocator = NULL;
    axutil_log_t *log = NULL;
    axutil_error_t *error = NULL;
    axutil_env_t *env = NULL;

    allocator = axutil_allocator_init(NULL);
    log = axutil_log_create(allocator, NULL, NULL);
    error = axutil_error_create(allocator);

    EXPECT_NULL(axutil_env_create_with_error_log(NULL, error, log));
    EXPECT_NULL(axutil_env_create_with_error_log(allocator, NULL, log));

    env = axutil_env_create_with_error_log(allocator, error, log);

    EXPECT_NOT_NULL(env);

    axutil_env_free(env);

    END_TEST_CASE();
}

void test_log_env()
{
    START_TEST_CASE("test_log_env");
    axutil_env_t *env = NULL;
    env = axutil_env_create_all("test_log_env.log", AXIS2_LOG_LEVEL_TRACE);

    TEST_ASSERT_VOID(env);

    AXIS2_LOG_CRITICAL(env->log, AXIS2_LOG_SI, "log1 %s", "test1");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "log2 %d", 2);
    AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "log3 %s", "test3");
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "log4 %s %s", "info1", "info2");
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "log5 %s %d", "test", 5);

    axutil_env_free(env);
    END_TEST_CASE();
}

void test_thread_env()
{
    START_TEST_CASE("test_thread_env");

    axutil_env_t *env = NULL;
    axutil_env_t *thread_env1 = NULL;
    axutil_env_t *thread_env2 = NULL;
    axutil_env_t *thread_env3 = NULL;

    env = axutil_env_create_all("test_thread_env.log", AXIS2_LOG_LEVEL_TRACE);

    EXPECT_NOT_NULL(env);

    thread_env1 = axutil_init_thread_env(env);
    thread_env2 = axutil_init_thread_env(env);

    EXPECT_NOT_NULL(thread_env1);
    EXPECT_NOT_NULL(thread_env2);

    thread_env3 = axutil_init_thread_env(thread_env1);

    axutil_free_thread_env(thread_env1);
    axutil_free_thread_env(thread_env2);

    AXIS2_LOG_INFO(thread_env3->log, AXIS2_LOG_SI, "log4 %s %s", "info1", "info2");

    axutil_free_thread_env(thread_env3);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "log4 %s %s", "info1", "info2");

    axutil_env_free(env);

    END_TEST_CASE();
}

void *AXIS2_CALL
test_ref(
    axutil_thread_t * td,
    void *param)
{
    axutil_env_t *env;
    env = ((axutil_env_t *) param);


    axutil_env_increment_ref(env);
    axutil_env_free(env);
    axutil_env_increment_ref(env);
    axutil_env_free(env);

    axutil_thread_exit(td, env->allocator);

    return (void *) 1;
}

void test_thread_env_reference_count()
{
    START_TEST_CASE("test_thread_env_reference_count");

    axutil_env_t *env = NULL;

    env = axutil_env_create_all("test_thread_env_reference_count.log", AXIS2_LOG_LEVEL_TRACE);

    axis2_status_t rv = AXIS2_FAILURE;
    axutil_allocator_t *allocator = NULL;
    int i;

    axutil_thread_t *t1[THREAD_AMMOUNT];

    allocator = env->allocator;

    for(i=0;i < THREAD_AMMOUNT; i++)
    {
        t1[i] = axutil_thread_create(allocator, NULL, test_ref, (void *) env);
        EXPECT_NOT_NULL(t1[i]);
        axutil_thread_detach(t1[i]);
    }

    sleep(1);

    axutil_env_free(env);

    END_TEST_CASE();
}

void test_multiple_envs()
{
    START_TEST_CASE("test_multiple_envs");

    axutil_env_t *env1 = NULL;
    axutil_env_t *env2 = NULL;

    env1 = axutil_env_create_all("test_multiple_envs1.log", AXIS2_LOG_LEVEL_TRACE);
    env2 = axutil_env_create_all("test_multiple_envs2.log", AXIS2_LOG_LEVEL_TRACE);

    TEST_ASSERT_VOID(env1);
    TEST_ASSERT_VOID(env2);

    axutil_env_free(env1);

    AXIS2_ERROR_SET_STATUS_CODE(env2->error,1234);

    EXPECT_EQ(axutil_env_check_status(env2),1234);
    AXIS2_LOG_DEBUG(env2->log, AXIS2_LOG_SI, "log5 %s %d", "test", 5);

    axutil_env_free(env2);
    END_TEST_CASE();
}


void test_reference_count()
{
    START_TEST_CASE("test_reference_count");
    axutil_allocator_t *allocator = NULL;
    axutil_log_t *log = NULL;
    axutil_error_t *error = NULL;
    axutil_env_t *env = NULL;
    axutil_env_t *thread_env = NULL;

    allocator = axutil_allocator_init(NULL);
    log = axutil_log_create(allocator, NULL, NULL);
    error = axutil_error_create(allocator);

    env = axutil_env_create_with_error_log(allocator, error, log);

    EXPECT_NOT_NULL(env);

    thread_env = axutil_init_thread_env(env);

    EXPECT_EQ(env->ref,1);

    axutil_env_increment_ref(env);
    axutil_env_increment_ref(env);
    axutil_env_increment_ref(env);

    EXPECT_EQ(env->ref,4);

    axutil_env_free(env);
    axutil_env_free(env);
    axutil_env_free(env);

    EXPECT_EQ(env->ref,1);

    axutil_free_thread_env(thread_env);

    axutil_env_free(env);

    END_TEST_CASE();
}

int main()
{
    START_TEST();

    test_create_env();
    test_log_env();
    test_thread_env();
    test_reference_count();
    test_thread_env_reference_count();
    test_multiple_envs();

    END_TEST();
    return 0;
}









