
#ifndef AXIS2C_TEST_MACROS_H_
#define AXIS2C_TEST_MACROS_H_

#include <stdio.h>

#define TEST_SEPARATOR_STR      "[----------------]"
#define TEST_START_STR          "[TEST       START]"
#define TEST_PASSED_STR         "[TEST      PASSED]"
#define TEST_FAILED_STR         "[TEST      FAILED]"
#define TEST_CASE_START_STR     "[TEST CASE    RUN]"
#define TEST_CASE_OK_STR        "[              OK]"
#define TEST_CASE_FAIL_STR      "[          FAILED]"

#define TEST_FAILURE_PRINT  printf("In %s:%i: failure\n",__FILE__, __LINE__);

int tests_ok;
int tests_failures;


#define START_TEST() \
printf("\x1B[32m"TEST_SEPARATOR_STR"\n"TEST_START_STR"\n"TEST_SEPARATOR_STR"\x1B[0m\n");\
tests_ok=0;\
tests_failures=0;

#define END_TEST() \
if(tests_failures==0)\
{\
printf("\x1B[32m"TEST_SEPARATOR_STR"\n"TEST_PASSED_STR"\x1B[0m Tests passed %i\n",tests_ok);\
}\
else \
{\
    printf("\x1B[31m"TEST_SEPARATOR_STR"\n"TEST_FAILED_STR"\x1B[0m\n");\
}

#define START_TEST_CASE(TEST_NAME) \
const char test_name[]=TEST_NAME;\
int expresions_ok=0;\
int expresions_failed=0;\
printf("\x1B[32m"TEST_CASE_START_STR"\x1B[0m %s\n",test_name);

#define END_TEST_CASE() \
if(expresions_failed==0)\
{\
printf("\x1B[32m"TEST_CASE_OK_STR"\x1B[0m %s, tests ok %i\n",test_name,expresions_ok);\
tests_ok++;\
}\
else \
{\
    printf("\x1B[31m"TEST_CASE_FAIL_STR"\x1B[0m %s, tests ok %i, tests failed %i\n",\
                                        test_name,expresions_ok,expresions_failed);\
    tests_failures++;\
}

#define TEST_ASSERT(EXPR) \
{\
if (!EXPR)\
{\
    TEST_FAILURE_PRINT\
    printf("Assertion failed\n");\
    return -1;\
}\
}

#define TEST_ASSERT_VOID(EXPR) \
{\
if (!EXPR)\
{\
    expresions_failed++;\
    TEST_FAILURE_PRINT\
    printf("Assertion failed\n");\
    END_TEST_CASE()\
    return;\
}\
}

#define TEST_ERROR(TEXT)\
{\
	TEST_FAILURE_PRINT\
	printf("%s\n",TEXT);\
	expresions_failed++;\
}

#define TEST_ABORT(TEXT)\
{\
	TEST_FAILURE_PRINT\
	printf("%s\n",TEXT);\
	expresions_failed++;\
    END_TEST_CASE()\
    return;\
}

#define EXPECT_NEQ(VAL1, VAL2) \
{\
__typeof(VAL1) var_val1=VAL1;\
__typeof(VAL2) var_val2=VAL2;\
if (var_val1 != var_val2)\
{\
    expresions_ok++;\
}else\
{\
    TEST_FAILURE_PRINT\
    printf("Expected not %i, received %i\n",var_val1,var_val2);\
    expresions_failed++;\
}\
}

#define EXPECT_EQ(VAL1, VAL2) \
{\
__typeof(VAL1) var_val1=VAL1;\
__typeof(VAL2) var_val2=VAL2;\
if (var_val1 == var_val2)\
{\
    expresions_ok++;\
}else\
{\
    TEST_FAILURE_PRINT\
    printf("Expected %i, received %i\n",var_val1,var_val2);\
    expresions_failed++;\
}\
}

#define EXPECT_GT(VAL1, VAL2) \
{\
__typeof(VAL1) var_val1=VAL1;\
__typeof(VAL2) var_val2=VAL2;\
if (var_val1 > var_val2)\
{\
    expresions_ok++;\
}else\
{\
    TEST_FAILURE_PRINT\
    printf("Expected %i to be greater than %i\n",var_val1,var_val2);\
    expresions_failed++;\
}\
}

#define EXPECT_NOT_NULL(VAL1) \
{\
if (VAL1)\
{\
    expresions_ok++;\
}else\
{\
    TEST_FAILURE_PRINT\
    printf("Expected something got NULL\n");\
    expresions_failed++;\
}\
}

#define EXPECT_NULL(VAL1) \
{\
if (!VAL1)\
{\
    expresions_ok++;\
}else\
{\
    TEST_FAILURE_PRINT\
    printf("Expected NULL got something\n");\
    expresions_failed++;\
}\
}

#define EXPECT_STREQ(STR1, STR2) \
{\
const char* var_str1 = STR1;\
const char* var_str2 = STR2;\
if (0 == axutil_strcmp(var_str1,var_str2))\
{\
    expresions_ok++;\
}\
else\
{\
    TEST_FAILURE_PRINT\
    printf("Expected %s, received %s\n",var_str2,var_str1);\
    expresions_failed++;\
}\
}


#define EXPECT_STRNEQ(STR1, STR2) \
{\
const char* var_str1 = STR1;\
const char* var_str2 = STR2;\
if (0 != axutil_strcmp(var_str1,var_str2))\
{\
    expresions_ok++;\
}\
else\
{\
    TEST_FAILURE_PRINT\
    printf("Expected %s, received %s\n",var_str2,var_str1);\
    expresions_failed++;\
}\
}

#endif /* AXIS2C_TEST_MACROS_H_ */
