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
#include <stdarg.h>

#include "path.h"
#include "config.h"
#include "string/fmt.h"

#if defined (HAVE_GETCWD)
# include <unistd.h>
# include <limits.h>
#elif defined (HAVE_GETCURRENTDIRECTORY)
# include <windows.h>
#endif

#if defined (_WIN32) && !defined (__CYGWIN__)
# include <windows.h>
#else
# include <unistd.h>

# include <sys/types.h>
# include <sys/stat.h>
#endif

#if defined (_WIN32) || defined (__CYGWIN__)
# define CRI_PATH_SEPARATOR '\\'
#else
# define CRI_PATH_SEPARATOR '/'
#endif

const char *basename_compat(const char *str)
{
    const char *start = str;

    for (const char *c = str; *c; ++c)
        if ((*c == '/' || *c == '\\') && c[1])
            start = c + 1;
    return start;
}

char *cri_path_cwd(void)
{
#if defined (HAVE_GETCWD)
    char *cwd = malloc(PATH_MAX);
    if (!cwd || !getcwd(cwd, PATH_MAX))
        return NULL;
#elif defined (HAVE_GETCURRENTDIRECTORY)
    DWORD buflen = GetCurrentDirectoryA(0, NULL);
    if (!buflen)
        return NULL;

    char *cwd = malloc(buflen + 1);
    if (!cwd || !GetCurrentDirectoryA(buflen + 1, cwd))
        return NULL;
#endif
    return cwd;
}

bool cri_path_isrelative(const char *path)
{
#ifdef _WIN32
    bool absolute = *path == '\\'
            || (strlen(path) > 3 && path[1] == ':' && path[2] == '\\');
    return !absolute;
#else
    return *path != '/';
#endif
}

char *cri_path_relativeof(const char *cstpath)
{
    char *path = strdup(cstpath);
    if (!path)
        return NULL;
    char *cwd = cri_path_cwd();
    if (!cwd)
        return NULL;

    if (!strcmp(cwd, "/"))
        return path;

    char *rel = NULL;
    size_t off = 0;
    size_t size = 0;

    char *lastdir = cwd + 1;
    char *firstdiff = NULL;
    char *ccwd = cwd;
    char *cpath = path;

    for (; *ccwd && *cpath; ++ccwd, ++cpath) {
        if (*ccwd != *cpath) {
            cri_fmt_bprintf(&rel, &off, &size, "../");

            while (*ccwd && *ccwd != '/' && *ccwd != '\\')
                ++ccwd;
            while (*cpath && *cpath != '/' && *cpath != '\\')
                ++cpath;

            if (!firstdiff)
                firstdiff = lastdir;
            if (!*cpath || !*ccwd)
                break;
        }
        if (*cpath == '/' || *cpath == '\\') {
            lastdir = cpath + 1;
        }
    }
    if (!firstdiff) {
        if (*cpath == '/' || *cpath == '\\') {
            cri_fmt_bprintf(&rel, &off, &size, "./");
        } else {
            while (*cpath != '/' && *cpath != '\\')
                --cpath;
            cri_fmt_bprintf(&rel, &off, &size, "../");
        }
        firstdiff = cpath;
    }

    char *saveptr;
    char *component = cri_strtok_r(firstdiff, "/\\", &saveptr);
    if (component) {
        cri_fmt_bprintf(&rel, &off, &size, "%s", component);
        while ((component = cri_strtok_r(NULL, "/\\", &saveptr))) {
            cri_fmt_bprintf(&rel, &off, &size, "/%s", component);
        }
    }

    free(cwd);
    free(path);

    return rel;
}

bool cri_path_isdirectory(const char *path)
{
#if defined (_WIN32) && !defined (__CYGWIN__)
    DWORD attr = GetFileAttributesA(path);
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat sb;
    return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
#endif
}


static inline size_t first_non_separator_index(const char *str)
{
    size_t idx = 0;
    while (str[idx] && (str[idx] == '/' || str[idx] == '\\'))
        ++idx;

    return idx;
}

static inline size_t first_trailing_separator_index(const char *str)
{
    size_t idx = strlen(str);
    while (idx > 0 && (str[idx - 1] == '/' || str[idx - 1] == '\\'))
        --idx;

    return idx;
}

/*
 * cri_path_build_va():
 * - skips empty elements
 * - the boundary between two elements is cleaned (separators are removed), one separator is inserted
 * - separators at the beginning and the end of the path are not removed
 */
static cri_string *cri_path_build_va(char separator, cri_string *output, const char *first_path_part, va_list *vl)
{
    if (!first_path_part)
        return NULL;

    if (!output)
        output = cri_string_new_reserve(256);
    else
        cri_string_clear(output);

    size_t last_end_idx = 0;
    bool is_first_non_empty = true, leading = false;
    const char *last_path_part = first_path_part;

    while (true) {
        const char *path_part = first_path_part ? first_path_part : va_arg(*vl, const char *);
        first_path_part = NULL;

        if (!path_part)
            break;

        if (!*path_part)
            continue;

        last_path_part = path_part;

        size_t begin_idx = first_non_separator_index(path_part);
        size_t end_idx = first_trailing_separator_index(path_part);
        last_end_idx = end_idx;

        if (!leading) {
            cri_string_append_length(output, path_part, begin_idx);
            leading = true;
        }

        if (begin_idx >= end_idx) {
            last_end_idx = is_first_non_empty ? begin_idx : end_idx;
            continue;
        }

        if (!is_first_non_empty)
            cri_string_append_char(output, separator);

        is_first_non_empty = false;

        cri_string_append_length(output, path_part + begin_idx, end_idx - begin_idx);
    }

    cri_string_append(output, last_path_part + last_end_idx);

    return output;
}

cri_string *cri_path_combine(cri_string *output, const char *first_path_part, ...)
{
    va_list vl;

    va_start(vl, first_path_part);
    output = cri_path_build_va(CRI_PATH_SEPARATOR, output, first_path_part, &vl);
    va_end(vl);

    return output;
}

cri_string *cri_path_build(char separator, cri_string *output, const char *first_path_part, ...)
{
    va_list vl;

    va_start(vl, first_path_part);
    output = cri_path_build_va(separator, output, first_path_part, &vl);
    va_end(vl);

    return output;
}
