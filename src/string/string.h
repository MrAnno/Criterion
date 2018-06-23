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
#ifndef CRI_STRING_H_
#define CRI_STRING_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct cri_string cri_string;

cri_string *cri_string_new(void);
cri_string *cri_string_new_reserve(size_t initial_capacity);
cri_string *cri_string_new_from_str(const char *str);

void cri_string_clear(cri_string *string);

cri_string *cri_string_append(cri_string *string, const char *str);
cri_string *cri_string_append_length(cri_string *string, const char *str, size_t length);
cri_string *cri_string_append_char(cri_string *string, char c);
cri_string *cri_string_append_printf(cri_string *string, const char *format, ...);

const char *cri_string_data(const cri_string *string);
size_t cri_string_capacity(const cri_string *string);
size_t cri_string_length(const cri_string *string);
bool cri_string_is_empty(const cri_string *string);

void cri_string_free(cri_string *string);
char *cri_string_free_steal_data(cri_string *string);

#endif /* !CRI_STRING_H_ */
