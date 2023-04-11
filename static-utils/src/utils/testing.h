// Biblioteca con funcionalidades utilitarias para testing con cspec
#ifndef TESTING_H_
#define TESTING_H_

#define TEST_FUNC(func) \
    { "\e[1;92m" #func "\e[0m", func }

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof(*(array)))

#define ADD_TEST_CASES_TO_SUITE(suite, tests)               \
    for (unsigned long i = 0; i < ARRAY_LENGTH(tests); i++) \
        CU_add_test(suite, tests[i].name, tests[i].func);

// Estructuras
struct test_case
{
    const char *name;
    void (*func)(void);
};
typedef struct test_case t_test_case;

#endif