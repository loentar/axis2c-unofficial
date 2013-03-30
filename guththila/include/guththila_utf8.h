
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
#ifndef GUTHTHILA_UTF8_H
#define GUTHTHILA_UTF8_H

#include <stdio.h>
#include <guththila_defines.h>
#include <axutil_utils.h>

EXTERN_C_START()  

typedef struct guththila_utf8_decoder_s
{
    int remaining_bytes;      /* Number of bytes needed to finish character */
    int used_bytes;           /* Number of bytes used so far for character */
    int error_bytes;          /* Number of bytes read that are in error */
    guththila_char_t used_buffer[4];  /* Buffer of bytes used so far */
    guththila_char_t error_buffer[4]; /* Buffer of bytes that are in error */
    int code_point;           /* Decoded code point */
    int replacement_code_point;       /* Code point used to replace errors */
} guththila_utf8_decoder_t;

#ifndef GUTHTHILA_UTF8_DECODER_IS_ERROR
#define GUTHTHILA_UTF8_DECODER_IS_ERROR(_decoder) ((_decoder)->error_bytes > 0)
#endif  

#ifndef GUTHTHILA_UTF8_DECODER_IS_CODE_POINT
#define GUTHTHILA_UTF8_DECODER_IS_CODE_POINT(_decoder) ((_decoder)->remaining_bytes == 0 && (_decoder)->used_bytes > 0)
#endif  

#ifndef GUTHTHILA_UTF8_DECODER_GET_CODE_POINT
#define GUTHTHILA_UTF8_DECODER_GET_CODE_POINT(_decoder) ((_decoder)->code_point)
#endif  

/* 
 * Create a UTF-8 decoder.  The decoder transcodes a UTF-8 byte stream
 * into Unicode code points.
 * @param env environment
 */
GUTHTHILA_EXPORT guththila_utf8_decoder_t * GUTHTHILA_CALL
guththila_utf8_decoder_create(const axutil_env_t * env);

/* 
 * Decode the next UTF-8 byte into a Unicode code point.  Since
 * more than 1 byte may be needed to decode a complete code point,
 * this function should be called repeatedly with successive bytes
 * from the UTF-8 byte stream as long as the return value is false.
 * This returns true when a complete code point is decoded or when
 * there is a complete error.  It is possible for there to be an
 * error without a completed code point as well as with a completed
 * code point.  The caller should check for error each time this
 * function is called.
 * @param decoder UTF-8 decoder
 * @param inchar the next UTF-8 byte to be decoded
 * @param code_point the returned Unicode code point
 * @param env environment
 * @return 0 if more bytes are needed, non-zero if code_point is complete.
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL  guththila_utf8_decoder_decode(
    guththila_utf8_decoder_t * decoder,
    guththila_char_t inchar,
    int *code_point,
    const axutil_env_t * env);

/* 
 * Free the UTF-8 decoder.
 * @param decoder UTF-8 decoder
 * @param env environment
 */
GUTHTHILA_EXPORT void GUTHTHILA_CALL  guththila_utf8_decoder_free(
    guththila_utf8_decoder_t * decoder,
    const axutil_env_t * env);

/* 
 * Clear the UTF-8 decoder after getting error and code point.
 * @param decoder UTF-8 decoder
 * @param env environment
 */
GUTHTHILA_EXPORT void GUTHTHILA_CALL  guththila_utf8_decoder_clear(
    guththila_utf8_decoder_t * decoder,
    const axutil_env_t * env);


typedef struct guththila_utf8_encoder_s
{
    int remaining_bytes;      /* Number of bytes needed to finish character */
    int is_error;             /* Whether there was an error in code point */
} guththila_utf8_encoder_t;

#ifndef GUTHTHILA_UTF8_ENCODER_IS_ERROR
#define GUTHTHILA_UTF8_ENCODER_IS_ERROR(_encoder) ((_encoder)->is_error)
#endif  

#ifndef GUTHTHILA_UTF8_ENCODER_IS_BUFFERED
#define GUTHTHILA_UTF8_ENCODER_IS_BUFFERED(_encoder) ((_encoder)->remaining_bytes == 0)
#endif  

/* 
 * Create a UTF-8 encoder.  The encoder transcodes a Unicode code point
 * into a UTF-8 byte stream.
 * @param env environment
 */
GUTHTHILA_EXPORT guththila_utf8_encoder_t * GUTHTHILA_CALL
guththila_utf8_encoder_create(const axutil_env_t * env);

/* 
 * Encode a Unicode code point into a UTF-8 byte stream buffer.  Since
 * more than 1 byte may be needed to encode a complete code point,
 * this function may need more buffer space than supplied so it should
 * be called repeatedly with more buffer space as long as the return
 * value is false.  This returns true when a complete code point is
 * encoded into the buffer.  It is possible for there to be an
 * error with an invalid Unicode code point.  The caller should check
 * for error each time this function is called.
 * @param encoder UTF-8 encoder
 * @param code_point the Unicode code point to be encoded
 * @param inchar the next UTF-8 byte to be decoded
 * @param env environment
 * @return 0 if more buffer is needed, non-zero if encoding is complete.
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL  guththila_utf8_encoder_encode(
    guththila_utf8_encoder_t * encoder,
    int code_point,
    guththila_char_t * buffer,
    int *buffer_length,
    const axutil_env_t * env);
/* 
 * Free the UTF-8 encoder.
 * @param encoder UTF-8 encoder
 * @param env environment
 */
GUTHTHILA_EXPORT void GUTHTHILA_CALL  guththila_utf8_encoder_free(
    guththila_utf8_encoder_t * encoder,
    const axutil_env_t * env);

EXTERN_C_END() 
#endif  /*  */

