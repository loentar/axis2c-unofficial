
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <axutil_error_default.h>
#include <axutil_log.h>
#include <axutil_log_default.h>
#include <axutil_allocator.h>
#include <axutil_utils.h>
#include "test_thread.h"
#include <unistd.h>
#include "../test_common/axis2c_test_macros.h"

#define THREAD_AMMOUNT 100

const axutil_env_t *env = NULL;
static axutil_thread_mutex_t *thread_lock = NULL;
static axutil_thread_once_t *control = NULL;
static int x = 0;
static int value = 0;

static axutil_thread_t *t1 = NULL;
static axutil_thread_t *t2 = NULL;

/*for detach tests*/
static axutil_thread_t *t3 = NULL;
static axutil_thread_t *t4 = NULL;

void
init_func(
    void)
{
    value++;
}

void
thread_init(
    const axutil_env_t * env)
{
    START_TEST_CASE("thread_init");
    axutil_allocator_t *allocator = NULL;

    allocator = env->allocator;

    control = axutil_thread_once_init(allocator);

    EXPECT_NOT_NULL(control);

    thread_lock =
        axutil_thread_mutex_create(allocator, AXIS2_THREAD_MUTEX_DEFAULT);

    EXPECT_NOT_NULL(thread_lock);

    END_TEST_CASE();
}

void *AXIS2_CALL
test_env(
    axutil_thread_t * td,
    void *param)
{
    axutil_env_t *env;
    env = ((axutil_env_t *) param);

    axutil_env_t *thread_env = NULL;

    thread_env = axutil_init_thread_env(env);

    axutil_free_thread_env(thread_env);

    axutil_thread_exit(td, env->allocator);

    return (void *) 1;
}

void *AXIS2_CALL
test_function1(
    axutil_thread_t * td,
    void *param)
{
    int i;
    i = *((int *) param);
    /*printf("thread data = %d \n", i);*/

    axutil_thread_once(control, init_func);

    axutil_thread_mutex_lock(thread_lock);
    /*printf("x = %d \n", ++x);*/
    ++x;
    axutil_thread_mutex_unlock(thread_lock);

    /*axutil_thread_exit(td, env->allocator); */

    return (void *) 1;
}


void
test_axutil_thread_create(
    const axutil_env_t * env)
{
    START_TEST_CASE("test_axutil_thread_create");
    axis2_status_t rv = AXIS2_FAILURE;
    axutil_allocator_t *allocator = NULL;
    int *i = NULL,
        *j = NULL;

    allocator = env->allocator;
    i = AXIS2_MALLOC(allocator, sizeof(int));
    *i = 5;
    t1 = axutil_thread_create(allocator, NULL, test_function1, (void *) i);

    EXPECT_NOT_NULL(t1);

    j = AXIS2_MALLOC(allocator, sizeof(int));
    *j = 25;

    t2 = axutil_thread_create(allocator, NULL, test_function1, (void *) j);

    EXPECT_NOT_NULL(t2);

    rv = axutil_thread_join(t1);

    EXPECT_EQ(rv, AXIS2_SUCCESS);

    rv = axutil_thread_join(t2);

    EXPECT_EQ(rv, AXIS2_SUCCESS);

    /*Locks*/
    EXPECT_EQ(1,value);

    /*Thread once*/
    EXPECT_EQ(2,x);

    END_TEST_CASE();
}

void *AXIS2_CALL
test_function2(
    axutil_thread_t * td,
    void *param)
{
    /*printf("thread \n");*/
    /*axutil_thread_exit(td, env->allocator); */

    return (void *) 1;
}

void
test_axutil_thread_env(
    const axutil_env_t * env)
{
    START_TEST_CASE("test_axutil_thread_env");
    axutil_allocator_t *allocator = NULL;
    int i;

    axutil_thread_t *t1[THREAD_AMMOUNT];

    allocator = env->allocator;

    for(i=0;i < THREAD_AMMOUNT; i++)
    {
        t1[i] = axutil_thread_create(allocator, NULL, test_env, (void *) env);
        EXPECT_NOT_NULL(t1[i]);
        axutil_thread_detach(t1[i]);
    }

    END_TEST_CASE();
}

void
test_axutil_thread_detach(
    const axutil_env_t * env)
{
    START_TEST_CASE("test_axutil_thread_detach");
    axutil_threadattr_t *attr = NULL;
    axutil_allocator_t *allocator = NULL;
    axis2_status_t rv = AXIS2_FAILURE;

    allocator = env->allocator;
    attr = axutil_threadattr_create(allocator);

    EXPECT_NOT_NULL(attr);

    rv = axutil_threadattr_detach_set(attr, 1);

    EXPECT_EQ(rv, AXIS2_SUCCESS);

    t3 = axutil_thread_create(allocator, attr, test_function2, NULL);

    EXPECT_NOT_NULL(t3);

    /*
     * thread is already detached - should return AXIS2_FAILURE
     */
    rv = axutil_thread_detach(t3);

    EXPECT_EQ(rv, AXIS2_FAILURE);
    /*
     * thread is already detached - should return AXIS2_FAILURE
     * cannot join detached threads
     */
    rv = axutil_thread_join(t3);

    EXPECT_EQ(rv, AXIS2_FAILURE);

    AXIS2_FREE(env->allocator, attr);

    END_TEST_CASE();
}

void
test_axutil_thread_detach2(
    const axutil_env_t * env)
{
    START_TEST_CASE("test_axutil_thread_detach2");

    axutil_threadattr_t *attr = NULL;
    axutil_allocator_t *allocator = NULL;
    axis2_status_t rv = AXIS2_FAILURE;

    allocator = env->allocator;
    attr = axutil_threadattr_create(allocator);

    EXPECT_NOT_NULL(attr);

    t4 = axutil_thread_create(allocator, attr, test_function2, NULL);

    EXPECT_NOT_NULL(t4);

    /*
     * thread is not detached yet - should return AXIS2_SUCCESS
     */
    rv = axutil_thread_detach(t4);

    EXPECT_EQ(rv, AXIS2_SUCCESS);

    /*
     * thread is already detached - should return AXIS2_FAILURE
     * cannot join detached threads
     */
    rv = axutil_thread_join(t4);

    EXPECT_EQ(rv, AXIS2_FAILURE);

    AXIS2_FREE(env->allocator, attr);

    END_TEST_CASE();
}

void
run_test_thread(
    const axutil_env_t * env)
{
    thread_init(env);
    test_axutil_thread_create(env);
    test_axutil_thread_detach(env);
    test_axutil_thread_detach2(env);
    test_axutil_thread_env(env);

#if defined (WIN32)
    Sleep(1000);                /*to give time for detached threads to execute */
#else
    sleep(2);
#endif

    axutil_thread_mutex_destroy(thread_lock);
}

const axutil_env_t *
create_env_with_error_log(
    )
{
    axutil_error_t *error = NULL;
    axutil_log_t *log22 = NULL;
    const axutil_env_t *env = NULL;
    axutil_allocator_t *allocator = axutil_allocator_init(NULL);
    if (!allocator)
    {
        printf("allocator is NULL\n");
        return NULL;
    }
    error = axutil_error_create(allocator);
    if (!error)
    {
        printf("cannot create error\n");
        return NULL;
    }

    log22 = axutil_log_create(allocator, NULL, "test123.log");
    if (!log22)
    {
        printf("cannot create log\n");
        return NULL;
    }
    /*
     * allow all types of logs
     */
    log22->level = AXIS2_LOG_LEVEL_DEBUG;
    /*   log22->enabled = 0; */
    env = axutil_env_create_with_error_log(allocator, error, log22);
    if (!env)
    {
        printf("cannot create env with error and log\n");
        return NULL;
    }
    return env;
}

int
main(
    void)
{
    START_TEST();
    env = create_env_with_error_log();

    if (!env)
        return -1;
    run_test_thread(env);

    END_TEST();
    return 0;
}
