#include <stddef.h>

#include "criterion/criterion.h"
#include "string/string.h"

Test(string, cri_string_constructors)
{
    const size_t capacity = 128;

    cri_string *str = cri_string_new();
    cr_expect_eq(cri_string_length(str), 0);
    cr_expect(cri_string_is_empty(str));
    cri_string_free(str);

    str = cri_string_new_reserve(capacity);
    cr_expect_eq(cri_string_length(str), 0);
    cr_expect_eq(cri_string_capacity(str), capacity);
    cr_expect(cri_string_is_empty(str));
    cri_string_free(str);

    str = cri_string_new_from_str("test");
    cr_expect_eq(cri_string_length(str), 4);
    cr_expect_str_eq(cri_string_data(str), "test");
    cri_string_free(str);
}

Test(string, cri_string_clear)
{
    cri_string *str = cri_string_new_from_str("0123456789");
    cr_expect_eq(cri_string_length(str), 10);

    cri_string_clear(str);

    cr_expect(cri_string_is_empty(str));
    cri_string_free(str);
}

Test(string, cri_string_append)
{
    cri_string *str = cri_string_new();
    cr_assert(cri_string_is_empty(str));

    cri_string_append(str, "test");
    cri_string_append_length(str, "123", 2);
    cri_string_append_char(str, '.');
    cri_string_append_printf(str, "%d...", 4);

    cr_assert_not(cri_string_is_empty(str));
    cr_assert_eq(cri_string_length(str), 11);
    cr_assert_str_eq(cri_string_data(str), "test12.4...");

    cri_string_free(str);
}

Test(string, cri_string_free_steal_data)
{
    const char *test_data = "0123456789";
    cri_string *str = cri_string_new_from_str(test_data);

    char *data = cri_string_free_steal_data(str);

    cr_assert_str_eq(data, test_data);

    free(data);
}
