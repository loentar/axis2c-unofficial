
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
#include <guththila.h>
#include <check.h>
#include <string.h>
#include "test.h"

static guththila_utf8_encoder_t *encoder = NULL;

void
setup_utf8_encoder(
    void)
{
    allocator = axutil_allocator_init(NULL);
    env = axutil_env_create(allocator);
    encoder = guththila_utf8_encoder_create(env);
}

void
teardown_utf8_encoder(
    void)
{
    guththila_utf8_encoder_free(encoder, env);
    axutil_env_free(env);
}

START_TEST(test_encode_1_byte_letter)
{
    int code_point = 'A';
    guththila_char_t buffer[10];
    int buffer_length = 10;
    int is_done = guththila_utf8_encoder_encode(encoder, code_point, buffer,
                                                &buffer_length, env);
    fail_if(is_done == 0, "guththila encoder is not done");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_ERROR(encoder) != 0,
            "guththila encoder detected error");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(encoder) == 0,
            "guththila encoder has no buffer space");
    int encoded_length = 10 - buffer_length;
    fail_if(encoded_length != 1, "guththila encoder encoding wrong length");
    fail_if(buffer[0] != 'A', "guththila encoder encoding wrong byte 0");
}
END_TEST

START_TEST(test_encode_2_byte_letter)
{
    //       Greek Alpha:   Code point: 913 UTF-8: \xCE\x91
    int code_point = 913;
    guththila_char_t buffer[10];
    int buffer_length = 10;
    int is_done = guththila_utf8_encoder_encode(encoder, code_point, buffer,
                                                &buffer_length, env);
    fail_if(is_done == 0, "guththila encoder is not done");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_ERROR(encoder) != 0,
            "guththila encoder detected error");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(encoder) == 0,
            "guththila encoder has no buffer space");
    int encoded_length = 10 - buffer_length;
    fail_if(encoded_length != 2, "guththila encoder encoding wrong length");
    fail_if(buffer[0] != '\xCE', "guththila encoder encoding wrong byte 0");
    fail_if(buffer[1] != '\x91', "guththila encoder encoding wrong byte 1");
}
END_TEST

START_TEST(test_encode_3_byte_letter)
{
    //       Thai Sara U:   Code point: 3640 UTF-8: \xE0\xB8\xB8
    int code_point = 3640;
    guththila_char_t buffer[10];
    int buffer_length = 10;
    int is_done = guththila_utf8_encoder_encode(encoder, code_point, buffer,
                                                &buffer_length, env);
    fail_if(is_done == 0, "guththila encoder is not done");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_ERROR(encoder) != 0,
            "guththila encoder detected error");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(encoder) == 0,
            "guththila encoder has no buffer space");
    int encoded_length = 10 - buffer_length;
    fail_if(encoded_length != 3, "guththila encoder encoding wrong length");
    fail_if(buffer[0] != '\xE0', "guththila encoder encoding wrong byte 0");
    fail_if(buffer[1] != '\xB8', "guththila encoder encoding wrong byte 1");
    fail_if(buffer[2] != '\xB8', "guththila encoder encoding wrong byte 2");
}
END_TEST

START_TEST(test_encode_4_byte_letter)
{
    // Chinese Treestump:   Code point: 144308 UTF-8: \xF0\xA3\x8E\xB4
    int code_point = 144308;
    guththila_char_t buffer[10];
    int buffer_length = 10;
    int is_done = guththila_utf8_encoder_encode(encoder, code_point, buffer,
                                                &buffer_length, env);
    fail_if(is_done == 0, "guththila encoder is not done");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_ERROR(encoder) != 0,
            "guththila encoder detected error");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(encoder) == 0,
            "guththila encoder has no buffer space");
    int encoded_length = 10 - buffer_length;
    fail_if(encoded_length != 4, "guththila encoder encoding wrong length");
    fail_if(buffer[0] != '\xF0', "guththila encoder encoding wrong byte 0");
    fail_if(buffer[1] != '\xA3', "guththila encoder encoding wrong byte 1");
    fail_if(buffer[2] != '\x8E', "guththila encoder encoding wrong byte 2");
    fail_if(buffer[3] != '\xB4', "guththila encoder encoding wrong byte 3");
}
END_TEST

START_TEST(test_encode_4_byte_letter_overflow)
{
    // Chinese Treestump:   Code point: 144308 UTF-8: \xF0\xA3\x8E\xB4
    int code_point = 144308;
    guththila_char_t buffer[10];
    int buffer_length = 2;
    int is_done = guththila_utf8_encoder_encode(encoder, code_point, buffer,
                                                &buffer_length, env);
    fail_if(is_done != 0, "guththila encoder already done");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_ERROR(encoder) != 0,
            "guththila encoder detected error");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(encoder) != 0,
            "guththila encoder has buffer space");
    int encoded_length = 2 - buffer_length;
    fail_if(encoded_length != 2, "guththila encoder encoding wrong length");
    fail_if(buffer[0] != '\xF0', "guththila encoder encoding wrong byte 0");
    fail_if(buffer[1] != '\xA3', "guththila encoder encoding wrong byte 1");
    buffer_length = 2;
    is_done = guththila_utf8_encoder_encode(encoder, code_point, buffer,
                                                &buffer_length, env);
    fail_if(is_done == 0, "guththila encoder is not done");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_ERROR(encoder) != 0,
            "guththila encoder detected error");
    fail_if(GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(encoder) == 0,
            "guththila encoder has no more buffer space");
    encoded_length = 2 - buffer_length;
    fail_if(encoded_length != 2, "guththila encoder encoding wrong length");
    fail_if(buffer[0] != '\x8E', "guththila encoder encoding wrong byte 2");
    fail_if(buffer[1] != '\xB4', "guththila encoder encoding wrong byte 3");
}
END_TEST

Suite * guththila_utf8_encoder_suite(void)
{
    Suite *s = suite_create("Guththila_utf8_encoder");

    /* Core test case */
    TCase *tc_core = tcase_create("utf8_encoder");
    tcase_add_checked_fixture(tc_core, setup_utf8_encoder, teardown_utf8_encoder);
    tcase_add_test(tc_core, test_encode_1_byte_letter);
    tcase_add_test(tc_core, test_encode_2_byte_letter);
    tcase_add_test(tc_core, test_encode_3_byte_letter);
    tcase_add_test(tc_core, test_encode_4_byte_letter);
    tcase_add_test(tc_core, test_encode_4_byte_letter_overflow);
    suite_add_tcase(s, tc_core);
    return s;
}
