
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

static guththila_utf8_decoder_t *decoder = NULL;

void
setup_utf8_decoder(
    void)
{
    allocator = axutil_allocator_init(NULL);
    env = axutil_env_create(allocator);
    decoder = guththila_utf8_decoder_create(env);
}

void
teardown_utf8_decoder(
    void)
{
    guththila_utf8_decoder_free(decoder, env);
    axutil_env_free(env);
}

START_TEST(test_decode_1_byte_letter)
{
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, 'A', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 'A', "guththila code_point not A");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 'A',
            "guththila decoder code_point not A");
}
END_TEST

START_TEST(test_decode_2_byte_letter)
{
    //       Greek Alpha:   Code point: 913 UTF-8: \xCE\x91
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xCE', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x91', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 913, "guththila code_point not Greek alpha");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 913,
            "guththila decoder code_point not Greek alpha");
}
END_TEST

START_TEST(test_decode_3_byte_letter)
{
    //       Thai Sara U:   Code point: 3640 UTF-8: \xE0\xB8\xB8
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xE0', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xB8', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xB8', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 3640, "guththila code_point not Thai Sara U");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 3640,
            "guththila decoder code_point not Thai Sara U");
}
END_TEST

START_TEST(test_decode_4_byte_letter)
{
    // Chinese Treestump:   Code point: 144308 UTF-8: \xF0\xA3\x8E\xB4
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xF0', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xA3', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x8E', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xB4', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 144308, "guththila code_point not Chinese Treestump");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 144308,
            "guththila decoder code_point not Chinese Treestump");
}
END_TEST

START_TEST(test_decode_1_byte_lower_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * From         0  0x00000000   0000 0000  0000 0000  0000 0000
 *                 0
 *                 0x00
 *                 0000 0000
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\0', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 0x00000000, "guththila code_point not 0x00000000");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 0x00000000,
            "guththila decoder code_point not 0x00000000");
}
END_TEST

START_TEST(test_decode_1_byte_upper_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * To         127  0x0000007f   0000 0000  0000 0000  0111 1111
 *                 127
 *                 0x7f
 *                 0111 1111
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\x7F', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 0x0000007f, "guththila code_point not 0x0000007f");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 0x0000007f,
            "guththila decoder code_point not 0x0000007f");
}
END_TEST

START_TEST(test_decode_2_byte_lower_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * From       128  0x00000080   0000 0000  0000 0000  1000 0000
 *                 194        128
 *                 0xc2       0x80
 *                 1100 0010  1000 0000
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xC2', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x80', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 0x00000080, "guththila code_point not 0x00000080");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 0x00000080,
            "guththila decoder code_point not 0x00000080");
}
END_TEST

START_TEST(test_decode_2_byte_upper_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * To       2,047  0x000007ff   0000 0000  0000 0111  1111 1111
 *                 223        191
 *                 0xdf       0xbf
 *                 1101 1111  1011 1111
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xDF', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xBF', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 0x000007ff, "guththila code_point not 0x000007ff");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 0x000007ff,
            "guththila decoder code_point not 0x000007ff");
}
END_TEST

START_TEST(test_decode_3_byte_lower_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * From     2,048  0x00000800   0000 0000  0000 1000  0000 0000
 *                 224        160        128
 *                 0xe0       0xa0       0x80
 *                 1110 0000  1010 0000  1000 0000
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xE0', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xA0', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x80', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 0x00000800, "guththila code_point not 0x00000800");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 0x00000800,
            "guththila decoder code_point not 0x00000800");
}
END_TEST

START_TEST(test_decode_3_byte_upper_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * To      65,535  0x0000ffff   0000 0000  1111 1111  1111 1111
 *                 239        191        191
 *                 0xef       0xbf       0xbf
 *                 1110 1111  1011 1111  1011 1111
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xEF', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xBF', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xBF', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 0x0000ffff, "guththila code_point not 0x0000ffff");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 0x0000ffff,
            "guththila decoder code_point not 0x0000ffff");
}
END_TEST

START_TEST(test_decode_4_byte_lower_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * From    65,536  0x00010000   0000 0001  0000 0000  0000 0000
 *                 240        144        128        128
 *                 0xf0       0x90       0x80       0x80
 *                 1111 0000  1001 0000  1000 0000  1000 0000
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xF0', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x90', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x80', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x80', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 0x00010000, "guththila code_point not 0x00010000");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 0x00010000,
            "guththila decoder code_point not 0x00010000");
}
END_TEST

START_TEST(test_decode_4_byte_upper_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * To   1,114,111  0x0010ffff   0001 0000  1111 1111  1111 1111
 *                 244        143        191        191
 *                 0xf4       0x8f       0xbf       0xbf
 *                 1111 0100  1000 1111  1011 1111  1011 1111
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xF4', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x8F', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xBF', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\xBF', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 0x0010ffff, "guththila code_point not 0x0010ffff");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 0x0010ffff,
            "guththila decoder code_point not 0x0010ffff");
}
END_TEST

START_TEST(test_decode_2_byte_overlong)
{
    //            Overlong 0x0000: \xC0\x80
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xC0', &code_point, env);
    fail_if(is_done == 0, "guththila decoder is not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) == 0,
            "guththila decoder not detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder has code point");
}
END_TEST

START_TEST(test_decode_4_byte_over_upper_bound)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * To   1,114,112  0x00110000   0001 0001  0000 0000  0000 0000
 *                 244        143        128        128
 *                 0xf4       0x90       0x80       0x80
 *                 1111 0100  1001 0000  1000 0000  1000 0000
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xF4', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x90', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x80', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, '\x80', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) == 0,
            "guththila decoder not detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder has code point");
}
END_TEST

START_TEST(test_decode_2_byte_missing_continuation)
{
/*
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 * To       2,047  0x000007ff   0000 0000  0000 0111  1111 1111
 *                 223        191
 *                 0xdf       0xbf
 *                 1101 1111  1011 1111
 */
    int code_point;
    int is_done =
        guththila_utf8_decoder_decode(decoder, '\xDF', &code_point, env);
    fail_if(is_done != 0, "guththila decoder already done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
            "guththila decoder detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
            "guththila decoder already has code point");
    is_done =
        guththila_utf8_decoder_decode(decoder, 'A', &code_point, env);
    fail_if(is_done == 0, "guththila decoder not done");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) == 0,
            "guththila decoder not detected error");
    fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
            "guththila decoder has no code point");
    fail_if(code_point != 'A', "guththila code_point not A");
    fail_if(GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(decoder) != 'A',
            "guththila decoder code_point not A");
}
END_TEST

START_TEST(test_encode_decode_all_1_to_3_byte)
{
    int code_point;
    guththila_utf8_encoder_t *encoder = guththila_utf8_encoder_create(env);
    for (code_point = 0; code_point <= 0x0000ffff; code_point++)
    {
        //fprintf(stderr, "CODE POINT: %d\n", code_point);
        guththila_char_t buffer[10];
        int buffer_length = 10;
        int is_done =
            guththila_utf8_encoder_encode(encoder, code_point, buffer,
                                          &buffer_length, env);
        fail_if(is_done == 0, "guththila encoder is not done");
        fail_if(GUTHTHILA_UTF8_ENCODER_IS_ERROR(encoder) != 0,
                "guththila encoder detected error");
        fail_if(GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(encoder) == 0,
                "guththila encoder has no buffer space");
        int encoded_length = 10 - buffer_length;
        int decoded_code_point = 0;
        int index;
        //fprintf(stderr, "Encoding: %d = ", encoded_length);
        //for (index = 0; index < encoded_length; index++)
        //{
        //    fprintf(stderr, " %02x", (0x00ff & buffer[index]));
        //}
        //fprintf(stderr, "\n");
        for (index = 0; index < encoded_length; index++)
        {
            is_done = guththila_utf8_decoder_decode(decoder, buffer[index],
                &decoded_code_point, env);
            fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
                    "guththila decoder detected error");
            if (index == encoded_length - 1)
            {
                fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
                        "guththila decoder has no code point");
            }
            else
            {
                fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
                        "guththila decoder already has code point");
            }
        }
        fail_if(decoded_code_point != code_point,
                "guththila decoded code point not the same");
    }
    guththila_utf8_encoder_free(encoder, env);
}
END_TEST

START_TEST(test_encode_decode_all_4_byte)
{
    int code_point;
    guththila_utf8_encoder_t *encoder = guththila_utf8_encoder_create(env);
    for (code_point = 0x00010000; code_point <= 0x0010ffff; code_point++)
    {
        //fprintf(stderr, "CODE POINT: %d\n", code_point);
        guththila_char_t buffer[10];
        int buffer_length = 10;
        int is_done =
            guththila_utf8_encoder_encode(encoder, code_point, buffer,
                                          &buffer_length, env);
        fail_if(is_done == 0, "guththila encoder is not done");
        fail_if(GUTHTHILA_UTF8_ENCODER_IS_ERROR(encoder) != 0,
                "guththila encoder detected error");
        fail_if(GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(encoder) == 0,
                "guththila encoder has no buffer space");
        int encoded_length = 10 - buffer_length;
        int decoded_code_point = 0;
        int index;
        //fprintf(stderr, "Encoding: %d = ", encoded_length);
        //for (index = 0; index < encoded_length; index++)
        //{
        //    fprintf(stderr, " %02x", (0x00ff & buffer[index]));
        //}
        //fprintf(stderr, "\n");
        for (index = 0; index < encoded_length; index++)
        {
            is_done = guththila_utf8_decoder_decode(decoder, buffer[index],
                &decoded_code_point, env);
            fail_if(GUTHTHILA_UTF8_DECODER_IS_ERROR(decoder) != 0,
                    "guththila decoder detected error");
            if (index == encoded_length - 1)
            {
                fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) == 0,
                        "guththila decoder has no code point");
            }
            else
            {
                fail_if(GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(decoder) != 0,
                        "guththila decoder already has code point");
            }
        }
        fail_if(decoded_code_point != code_point,
                "guththila decoded code point not the same");
    }
    guththila_utf8_encoder_free(encoder, env);
}
END_TEST

Suite * guththila_utf8_decoder_suite(void)
{
    Suite *s = suite_create("Guththila_utf8_decoder");

    /* Core test case */
    TCase *tc_core = tcase_create("utf8_decoder");
    tcase_add_checked_fixture(tc_core, setup_utf8_decoder, teardown_utf8_decoder);
    tcase_add_test(tc_core, test_decode_1_byte_letter);
    tcase_add_test(tc_core, test_decode_2_byte_letter);
    tcase_add_test(tc_core, test_decode_3_byte_letter);
    tcase_add_test(tc_core, test_decode_4_byte_letter);
    tcase_add_test(tc_core, test_decode_1_byte_lower_bound);
    tcase_add_test(tc_core, test_decode_1_byte_upper_bound);
    tcase_add_test(tc_core, test_decode_2_byte_lower_bound);
    tcase_add_test(tc_core, test_decode_2_byte_upper_bound);
    tcase_add_test(tc_core, test_decode_3_byte_lower_bound);
    tcase_add_test(tc_core, test_decode_3_byte_upper_bound);
    tcase_add_test(tc_core, test_decode_4_byte_lower_bound);
    tcase_add_test(tc_core, test_decode_4_byte_upper_bound);
    tcase_add_test(tc_core, test_decode_2_byte_overlong);
    tcase_add_test(tc_core, test_decode_4_byte_over_upper_bound);
    tcase_add_test(tc_core, test_decode_2_byte_missing_continuation);
    tcase_add_test(tc_core, test_encode_decode_all_1_to_3_byte);
    tcase_set_timeout(tc_core, 600);
    tcase_add_test(tc_core, test_encode_decode_all_4_byte);
    suite_add_tcase(s, tc_core);
    return s;
}
