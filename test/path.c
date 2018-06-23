#include <stddef.h>

#include "criterion/criterion.h"
#include "compat/path.h"
#include "string/string.h"

#define expect_path_eq(actual, expected) cr_expect_str_eq(cri_string_data(actual), expected)

Test(path, basename_compat)
{
    cr_expect_str_eq(basename_compat("/a/b/c/dir/basename.ext"), "basename.ext");
    cr_expect_str_eq(basename_compat("C:\\a\\b\\c\\dir\\basename.ext"), "basename.ext");
    cr_expect_str_eq(basename_compat("C:\\a\\b\\c/dir/basename.ext"), "basename.ext");

    cr_expect_str_eq(basename_compat("/a/basename/"), "basename/");
    cr_expect_str_eq(basename_compat("./"), "./");
    cr_expect_str_eq(basename_compat("../../"), "../");

    cr_expect_str_eq(basename_compat("a//b\\\\base"), "base");

    cr_expect_str_eq(basename_compat("basename"), "basename");
    cr_expect_str_eq(basename_compat(""), "");
}

Test(path, cri_path_build)
{
    const char s = '/';
    cri_string *o = cri_string_new();

    cr_expect_eq(cri_path_build(s, o, NULL), NULL);

    expect_path_eq(cri_path_build(s, o, "", NULL), "");
    expect_path_eq(cri_path_build(s, o, " ", NULL), " ");
    expect_path_eq(cri_path_build(s, o, "/", NULL), "/");
    expect_path_eq(cri_path_build(s, o, "//", NULL), "//");
    expect_path_eq(cri_path_build(s, o, "///", NULL), "///");

    expect_path_eq(cri_path_build(s, o, "a", NULL), "a");
    expect_path_eq(cri_path_build(s, o, "//a//", NULL), "//a//");

    expect_path_eq(cri_path_build(s, o, "/a", "b", "c", NULL), "/a/b/c");
    expect_path_eq(cri_path_build(s, o, "a", "b", "c/", NULL), "a/b/c/");
    expect_path_eq(cri_path_build(s, o, "/a", "b", "c/", NULL), "/a/b/c/");

    expect_path_eq(cri_path_build(s, o, "a/b", "c//d", "e", NULL), "a/b/c//d/e");
    expect_path_eq(cri_path_build(s, o, "/a/b", "//c//d//", "e//", NULL), "/a/b/c//d/e//");

    expect_path_eq(cri_path_build(s, o, "/", "a", "/", NULL), "/a/");
    expect_path_eq(cri_path_build(s, o, "//", "a", "//", NULL), "//a//");
    expect_path_eq(cri_path_build(s, o, "///", "a", "///", NULL), "///a///");

    expect_path_eq(cri_path_build(s, o, "a", "///", NULL), "a///");
    expect_path_eq(cri_path_build(s, o, "///", "a", NULL), "///a");

    expect_path_eq(cri_path_build(s, o, "a", "///", "b", NULL), "a/b");
    expect_path_eq(cri_path_build(s, o, "a", "/", "b", NULL), "a/b");

    expect_path_eq(cri_path_build(s, o, "", "a", "", "b//", "/", "", NULL), "a/b/");
    expect_path_eq(cri_path_build(s, o, "a", "b/", "", "/", "///", "c", NULL), "a/b/c");

    expect_path_eq(cri_path_build(s, o, "a", "b", "c", NULL), "a/b/c");
    expect_path_eq(cri_path_build(s, o, "/a/", "/b/", "/c", "d/", "e", NULL), "/a/b/c/d/e");
    expect_path_eq(cri_path_build(s, o, "//a//", "//b//", "//c//", NULL), "//a/b/c//");

    cri_string_free(o);
}

Test(path, cri_path_build_win)
{
    const char s = '\\';
    cri_string *o = cri_string_new();

    expect_path_eq(cri_path_build(s, o, "C:\\", "\\Users\\", "Snaipe", NULL), "C:\\Users\\Snaipe");
    expect_path_eq(cri_path_build(s, o, "C:/", "\\Users//", "Snaipe\\", NULL), "C:\\Users\\Snaipe\\");
    expect_path_eq(cri_path_build(s, o, "\\", "//test\\", "\\", NULL), "\\test\\");

    cri_string_free(o);
}
