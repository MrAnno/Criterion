# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

include(CheckPrototypeDefinition)
include(CheckLibraryExists)
include(PackageUtils)

# Check for packages

cr_find_package (Gettext)
cr_find_package (Libintl)

if (I18N AND GETTEXT_FOUND AND LIBINTL_LIB_FOUND)
  set (GettextTranslate_ALL 1)
  set (GettextTranslate_GMO_BINARY 1)
  set (ENABLE_NLS 1)
endif ()

cr_find_package (PCRE PKGCONFIG libpcre)

# Check for functions

check_prototype_definition(
  strtok_s
  "char *strtok_s(char *strToken, const char *strDelimit, char **context)"
  NULL
  "string.h"
  HAVE_STRTOK_S)

check_library_exists (rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)

# Check for C++11

if (LANG_CXX)
  enable_language(CXX)
endif ()

if (NOT MSVC AND CMAKE_CXX_COMPILER_WORKS)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

  if(COMPILER_SUPPORTS_CXX11)
    set(CXX11_FLAG "-std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CXX11_FLAG "-std=c++0x")
  else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
  endif()
endif()

# Valgrind support

if (DEV_BUILD)
  set(ENABLE_VALGRIND_ERRORS 1)
endif ()
