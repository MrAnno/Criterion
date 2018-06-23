/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdlib.h>
#include <string.h>

#include "criterion/internal/asprintf-compat.h"
#include "string.h"
#include "err.h"
#include "fmt.h"

struct cri_string {
    char *data;
    size_t length;
    size_t capacity;
};

static inline void cri_string_expand(cri_string *string, size_t n)
{
    if (string->capacity <= string->length + n) {
        string->capacity = string->length + n + 1;
        string->data = realloc(string->data, string->capacity);
        assert(string->data);
    }
}

cri_string *cri_string_new(void)
{
    return cri_string_new_reserve(8);
}

cri_string *cri_string_new_reserve(size_t initial_capacity)
{
    cri_string *string = malloc(sizeof(cri_string));
    assert(string);

    string->data = NULL;
    string->length = 0;
    string->capacity = 0;

    cri_string_expand(string, initial_capacity);
    *string->data = '\0';

    return string;
}

cri_string *cri_string_new_from_str(const char *str)
{
    size_t str_length = strlen(str);
    cri_string *string = cri_string_new_reserve(str_length);

    return cri_string_append_length(string, str, str_length);
}

void cri_string_clear(cri_string *string)
{
    string->length = 0;
    string->data[string->length] = '\0';
}

cri_string *cri_string_append(cri_string *string, const char *str)
{
    return cri_string_append_length(string, str, strlen(str));
}

cri_string *cri_string_append_length(cri_string *string, const char *str, size_t length)
{
    cri_string_expand(string, length);

    memcpy(string->data + string->length, str, length);
    string->length += length;
    string->data[string->length] = '\0';

    return string;
}

cri_string *cri_string_append_char(cri_string *string, char c)
{
    cri_string_expand(string, 1);

    string->data[string->length++] = c;
    string->data[string->length] = '\0';

    return string;
}

cri_string *cri_string_append_printf(cri_string *string, const char *format, ...)
{
    va_list vl;
    va_start(vl, format);

    char *formatted;
    int length = cr_vasprintf(&formatted, format, vl);

    if (length >= 0) {
        cri_string_append_length(string, formatted, length);
        cr_asprintf_free(formatted);
    }

    va_end(vl);

    return string;
}

const char *cri_string_data(const cri_string *string)
{
    return string->data;
}

size_t cri_string_capacity(const cri_string *string)
{
    return string->capacity;
}

size_t cri_string_length(const cri_string *string)
{
    return string->length;
}

bool cri_string_is_empty(const cri_string *string)
{
    return string->length == 0;
}

void cri_string_free(cri_string *string)
{
    free(cri_string_free_steal_data(string));
}

char *cri_string_free_steal_data(cri_string *string)
{
    char *data = string->data;

    string->data = NULL;
    string->length = 0;
    string->capacity = 0;
    free(string);

    return data;
}
