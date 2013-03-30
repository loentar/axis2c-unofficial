
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <guththila_utf8.h>

/**
 * There are 4 separate valid ranges of code points, based on the number of
 * bytes each takes to be encoded.
 *
 *       Code Point
 *      Decimal    Hex          Binary
 *                 UTF-8 Encoding
 *
 * 1 Byte Encoding:
 * From         0  0x00000000   0000 0000  0000 0000  0000 0000
 *                 0
 *                 0x00
 *                 0000 0000
 *
 * To         127  0x0000007f   0000 0000  0000 0000  0111 1111
 *                 127
 *                 0x7f
 *                 0111 1111
 *
 *
 * 2 Byte Encoding:
 * From       128  0x00000080   0000 0000  0000 0000  1000 0000
 *                 194        128
 *                 0xc2       0x80
 *                 1100 0010  1000 0000
 *
 * To       2,047  0x000007ff   0000 0000  0000 0111  1111 1111
 *                 223        191
 *                 0xdf       0xbf
 *                 1101 1111  1011 1111
 *
 *
 * 3 Byte Encoding:
 * From     2,048  0x00000800   0000 0000  0000 1000  0000 0000
 *                 224        160        128
 *                 0xe0       0xa0       0x80
 *                 1110 0000  1010 0000  1000 0000
 *
 * To      65,535  0x0000ffff   0000 0000  1111 1111  1111 1111
 *                 239        191        191
 *                 0xef       0xbf       0xbf
 *                 1110 1111  1011 1111  1011 1111
 *
 *
 * 4 Byte Encoding:
 * From    65,536  0x00010000   0000 0001  0000 0000  0000 0000
 *                 240        144        128        128
 *                 0xf0       0x90       0x80       0x80
 *                 1111 0000  1001 0000  1000 0000  1000 0000
 *
 * To   1,114,111  0x0010ffff   0001 0000  1111 1111  1111 1111
 *                 244        143        191        191
 *                 0xf4       0x8f       0xbf       0xbf
 *                 1111 0100  1000 1111  1011 1111  1011 1111
 */

GUTHTHILA_EXPORT guththila_utf8_decoder_t * GUTHTHILA_CALL
guththila_utf8_decoder_create(const axutil_env_t * env)
{
    guththila_utf8_decoder_t * decoder =
        (guththila_utf8_decoder_t *) AXIS2_MALLOC(env->allocator,
                                            sizeof(guththila_utf8_decoder_t));
    if (decoder)
    {
        decoder->remaining_bytes = 0;
        decoder->used_bytes = 0;
        decoder->error_bytes = 0;
        decoder->used_buffer[0] = '\0';
        decoder->error_buffer[0] = '\0';
        decoder->code_point = 0;
        decoder->replacement_code_point = '?';
    }
    return decoder;
}

GUTHTHILA_EXPORT void GUTHTHILA_CALL 
guththila_utf8_decoder_free(
    guththila_utf8_decoder_t * decoder,
    const axutil_env_t * env) 
{
    AXIS2_FREE(env->allocator, decoder);
}

GUTHTHILA_EXPORT void GUTHTHILA_CALL 
guththila_utf8_decoder_clear(
    guththila_utf8_decoder_t * decoder,
    const axutil_env_t * env) 
{
    decoder->remaining_bytes = 0;
    decoder->used_bytes = 0;
    decoder->error_bytes = 0;
    decoder->used_buffer[0] = '\0';
    decoder->error_buffer[0] = '\0';
    decoder->code_point = 0;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_utf8_decoder_decode(
    guththila_utf8_decoder_t * decoder,
    guththila_char_t inchar,
    int *code_point,
    const axutil_env_t * env)
{
    decoder->error_bytes = 0;
    decoder->used_buffer[decoder->used_bytes] = inchar;
    unsigned int inbyte = (unsigned char) inchar;
    if (inbyte < 0x0080) /* 0x0080=128: 0xxx xxxx */
    {
        if (decoder->remaining_bytes > 0)
        {
            /* error: missing required follow-on byte */
            /* The bytes before this are bad, but this 1 may be good. */
            decoder->error_bytes = decoder->used_bytes;
            memcpy(decoder->error_buffer, decoder->used_buffer,
                decoder->error_bytes);
        }
        decoder->code_point = inbyte;
        decoder->used_bytes = 1;
        decoder->remaining_bytes = 0;
    }
    else if (inbyte < 0x00c0) /* 0x00c0=192: 10xx xxxx */
    {
        /* Follow-on byte */
        if (decoder->remaining_bytes > 0)
        {
            decoder->code_point <<= 6;
            decoder->code_point |= inbyte & 0x3f;
            decoder->used_bytes++;
            decoder->remaining_bytes--;
            if (decoder->remaining_bytes == 0)
            {
                /* We're done, but make sure not overlong encoding. */
                if (decoder->code_point <= 0x0000007f)
                {
                    if (decoder->used_bytes != 1)
                    {
                        /* error: overlong bytes */
                        decoder->error_bytes = decoder->used_bytes;
                        memcpy(decoder->error_buffer, decoder->used_buffer,
                               decoder->error_bytes);
                        decoder->code_point = decoder->replacement_code_point;
                        decoder->used_bytes = 0;
                        decoder->remaining_bytes = 0;
                    }
                }
                else if (decoder->code_point <= 0x000007ff)
                {
                    if (decoder->used_bytes != 2)
                    {
                        /* error: overlong bytes */
                        decoder->error_bytes = decoder->used_bytes;
                        memcpy(decoder->error_buffer, decoder->used_buffer,
                               decoder->error_bytes);
                        decoder->code_point = decoder->replacement_code_point;
                        decoder->used_bytes = 0;
                        decoder->remaining_bytes = 0;
                    }
                }
                else if (decoder->code_point <= 0x0000ffff)
                {
                    if (decoder->used_bytes != 3)
                    {
                        /* error: overlong bytes */
                        decoder->error_bytes = decoder->used_bytes;
                        memcpy(decoder->error_buffer, decoder->used_buffer,
                               decoder->error_bytes);
                        decoder->code_point = decoder->replacement_code_point;
                        decoder->used_bytes = 0;
                        decoder->remaining_bytes = 0;
                    }
                }
                else if (decoder->code_point <= 0x0010ffff)
                {
                    if (decoder->used_bytes != 4)
                    {
                        /* error: overlong bytes */
                        decoder->error_bytes = decoder->used_bytes;
                        memcpy(decoder->error_buffer, decoder->used_buffer,
                               decoder->error_bytes);
                        decoder->code_point = decoder->replacement_code_point;
                        decoder->used_bytes = 0;
                        decoder->remaining_bytes = 0;
                    }
                }
                else
                {
                    /* error: invalid not defined by Unicode */
                    decoder->error_bytes = decoder->used_bytes;
                    decoder->code_point = decoder->replacement_code_point;
                    decoder->used_bytes = 0;
                    decoder->remaining_bytes = 0;
                }
            }
        }
        else
        {
            /* error: unexpected follow-on byte. */
            /* The byte is always bad when it is not expected. */
            decoder->error_bytes = 1;
            memcpy(decoder->error_buffer, decoder->used_buffer,
                   decoder->error_bytes);
            decoder->code_point = decoder->replacement_code_point;
            decoder->used_bytes = 0;
            decoder->remaining_bytes = 0;
        }
    }
    else if (inbyte < 0x00c2) /* 0x00c2=194 */
    {
        /* error: overlong encoding for 0xxx xxxx */
        /* The byte is always bad. */
        decoder->error_bytes = decoder->used_bytes + 1;
        memcpy(decoder->error_buffer, decoder->used_buffer,
               decoder->error_bytes);
        decoder->code_point = decoder->replacement_code_point;
        decoder->used_bytes = 0;
        decoder->remaining_bytes = 0;
    }
    else if (inbyte < 0x00e0) /* 0x00e0=224: 110x xxxx */
    {
        /* Start of a 2-byte character: */
        /* 110y yyxx 10xx xxxx => */
        /* 0000 0yyy xxxx xxxx */
        if (decoder->remaining_bytes > 0)
        {
            /* error: missing required follow-on byte: 10xx xxxx */
            /* The bytes before this are bad, but this 1 may be good. */
            decoder->error_bytes = decoder->used_bytes;
            memcpy(decoder->error_buffer, decoder->used_buffer,
                   decoder->error_bytes);
        }
        decoder->code_point = inbyte & 0x1f;
        decoder->used_bytes = 1;
        decoder->remaining_bytes = 1;
    }
    else if (inbyte < 0x00f0) /* 0x00f0=240: 1110 xxxx */
    {
        /* Start of a 3-byte character: */
        /* 1110 yyyy 10yy yyxx 10xx xxxx => */
        /* yyyy yyyy xxxx xxxx */
        if (decoder->remaining_bytes > 0)
        {
            /* error: expected follow-on byte instead of new 3-byte character. */
            /* The bytes before this are bad, but this 1 may be good. */
            decoder->error_bytes = decoder->used_bytes;
            memcpy(decoder->error_buffer, decoder->used_buffer,
                   decoder->error_bytes);
        }
        decoder->code_point = inbyte & 0x0f;
        decoder->used_bytes = 1;
        decoder->remaining_bytes = 2;
    }
    else if (inbyte < 0x00f5) /* 0x00f5=245: 1111 0xxx */
    {
        /* Start of a 4-byte character: */
        /* 1111 0zzz  10zz yyyy  10yy yyxx  10xx xxxx => */
        /* 000z zzzz  yyyy yyyy  xxxx xxxx */
        if (decoder->remaining_bytes > 0)
        {
            /* error: expected follow-on byte instead of new 4-byte character. */
            /* The bytes before this are bad, but this 1 may be good. */
            decoder->error_bytes = decoder->used_bytes;
            memcpy(decoder->error_buffer, decoder->used_buffer,
                   decoder->error_bytes);
        }
        decoder->code_point = inbyte & 0x07;
        decoder->used_bytes = 1;
        decoder->remaining_bytes = 3;
    }
    else
    {
        /* error: not defined by UTF-8/Unicode */
        /* The byte is always bad. */
        decoder->error_bytes = decoder->used_bytes + 1;
        memcpy(decoder->error_buffer, decoder->used_buffer,
               decoder->error_bytes);
        decoder->code_point = decoder->replacement_code_point;
        decoder->used_bytes = 0;
        decoder->remaining_bytes = 0;
    }
    if (decoder->remaining_bytes == 0 && decoder->used_bytes > 0)
    {
        *code_point = decoder->code_point;
    }
    return decoder->remaining_bytes == 0;
}

GUTHTHILA_EXPORT guththila_utf8_encoder_t * GUTHTHILA_CALL
guththila_utf8_encoder_create(const axutil_env_t * env)
{
    guththila_utf8_encoder_t * encoder =
        (guththila_utf8_encoder_t *) AXIS2_MALLOC(env->allocator,
                                            sizeof(guththila_utf8_encoder_t));
    if (encoder)
    {
        encoder->remaining_bytes = 0;
        encoder->is_error = 0;
    }
    return encoder;
}

GUTHTHILA_EXPORT void GUTHTHILA_CALL guththila_utf8_encoder_free(
    guththila_utf8_encoder_t * encoder,
    const axutil_env_t * env)
{
    AXIS2_FREE(env->allocator, encoder);
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL  guththila_utf8_encoder_encode(
    guththila_utf8_encoder_t * encoder,
    int code_point,
    guththila_char_t * buffer,
    int *buffer_length,
    const axutil_env_t * env)
{
    encoder->is_error = 0;
    if (code_point <= 0x0000007f)
    {
        if (encoder->remaining_bytes == 0)
        {
            encoder->remaining_bytes = 1;
        }
        if (*buffer_length >= 1)
        {
            *buffer = (unsigned char) code_point;
            buffer++;
            (*buffer_length)--;
            encoder->remaining_bytes--;
        }
    }
    else if (code_point <= 0x000007ff)
    {
        if (encoder->remaining_bytes == 0)
        {
            encoder->remaining_bytes = 2;
        }
        if (*buffer_length >= 1)
        {
            if (encoder->remaining_bytes >= 2)
            {
               *buffer = 0xc0 | (code_point >> 6);
               buffer++;
               (*buffer_length)--;
               encoder->remaining_bytes--;
            }
            if (*buffer_length >= 1)
            {
                *buffer = 0x80 | (code_point & 0x003f);
                buffer++;
                (*buffer_length)--;
                encoder->remaining_bytes--;
            }
        }
    }
    else if (code_point <= 0x0000ffff)
    {
        if (encoder->remaining_bytes == 0)
        {
            encoder->remaining_bytes = 3;
        }
        if (*buffer_length >= 1)
        {
            if (encoder->remaining_bytes >= 3)
            {
                *buffer = 0xe0 | (code_point >> 12);
                buffer++;
                (*buffer_length)--;
                encoder->remaining_bytes--;
            }
            if (*buffer_length >= 1)
            {
                if (encoder->remaining_bytes >= 2)
                {
                    *buffer = 0x80 | ((code_point >> 6) & 0x003f);
                    buffer++;
                    (*buffer_length)--;
                    encoder->remaining_bytes--;
                }
                if (*buffer_length >= 1)
                {
                    *buffer = 0x80 | (code_point & 0x003f);
                    buffer++;
                    (*buffer_length)--;
                    encoder->remaining_bytes--;
                }
            }
        }
    }
    else if (code_point <= 0x0010ffff)
    {
        if (encoder->remaining_bytes == 0)
        {
            encoder->remaining_bytes = 4;
        }
        if (*buffer_length >= 1)
        {
            if (encoder->remaining_bytes >= 4)
            {
                *buffer = 0xf0 | (code_point >> 18);
                buffer++;
                (*buffer_length)--;
                encoder->remaining_bytes--;
            }
            if (*buffer_length >= 1)
            {
                if (encoder->remaining_bytes >= 3)
                {
                    *buffer = 0x80 | ((code_point >> 12) & 0x003f);
                    buffer++;
                    (*buffer_length)--;
                    encoder->remaining_bytes--;
                }
                if (*buffer_length >= 1)
                {
                    if (encoder->remaining_bytes >= 2)
                    {
                        *buffer = 0x80 | ((code_point >> 6) & 0x003f);
                        buffer++;
                        (*buffer_length)--;
                        encoder->remaining_bytes--;
                    }
                    if (*buffer_length >= 1)
                    {
                        *buffer = 0x80 | (code_point & 0x003f);
                        buffer++;
                        (*buffer_length)--;
                        encoder->remaining_bytes--;
                    }
                }
            }
        }
    }
    else
    {
        /* error: code point not defined by Unicode */
        encoder->is_error = 1;
        encoder->remaining_bytes = 0;
    }
    return encoder->remaining_bytes == 0;
}
