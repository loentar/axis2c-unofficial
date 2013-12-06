
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

#include <test_md5.h>
#include <stdio.h>
#include <axutil_string.h>
#include <axutil_md5.h>
#include "../test_common/axis2c_test_macros.h"

/* Digests a string and prints the result.
 */
static void MDString (char * string, const axutil_env_t * env, char *out)
{
  axutil_md5_ctx_t * context;
  unsigned char digest[16];
  unsigned int i;
  unsigned int len = axutil_strlen(string);
  char *out_ptr;

  context = axutil_md5_ctx_create(env);
  axutil_md5_update(context, env, string, len);
  axutil_md5_final(context, env, digest);
  axutil_md5_ctx_free(context, env);

  for (i = 0, out_ptr=out; i < 16; i++,out_ptr+=2)
     sprintf (out_ptr,"%02x", digest[i]);

  out[32]= '\0';
}

void test_md5(const axutil_env_t *env)
{
    START_TEST_CASE("test_md5");
    /*printf ("test confirmation: Rivest, R., \"The MD5 Message-Digest Algorithm\", RFC 1321, April 1992.\n");*/
    char output[33];

    MDString ("", env, output);
    EXPECT_STREQ(output, "d41d8cd98f00b204e9800998ecf8427e");
    MDString ("a", env,output);
    EXPECT_STREQ(output, "0cc175b9c0f1b6a831c399e269772661");
    MDString ("abc", env,output);
    EXPECT_STREQ(output, "900150983cd24fb0d6963f7d28e17f72");
    MDString ("message digest", env,output);
    EXPECT_STREQ(output, "f96b697d7cb7938d525a2f31aaf161d0");
    MDString ("abcdefghijklmnopqrstuvwxyz", env,output);
    EXPECT_STREQ(output, "c3fcd3d76192e4007dfb496cca67e13b");
    MDString ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", env,output);
    EXPECT_STREQ(output, "d174ab98d277d9f5a5611c2c9f419d9f");
    MDString ("12345678901234567890123456789012345678901234567890123456789012345678901234567890", env,output);
    EXPECT_STREQ(output, "57edf4a22be3c955ac49da2e2107b67a");

    END_TEST_CASE();
}
