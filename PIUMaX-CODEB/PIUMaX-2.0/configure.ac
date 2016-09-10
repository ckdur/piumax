#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(PIUMaX2, [VERSION], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE([-Werror foreign])
AC_CONFIG_SRCDIR(src/piu.cpp)
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB(GL, main)
AC_CHECK_LIB(GLU, main)
AC_CHECK_LIB(SDL2, main)
AC_CHECK_LIB(png, main)
AC_CHECK_LIB(lua-5.1, main)
AC_CHECK_LIB(avcodec, main)
AC_CHECK_LIB(avformat, main)
AC_CHECK_LIB(avutil, main)

# Checks for header files.
AC_CHECK_HEADERS([malloc.h stdint.h stdlib.h string.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor memset pow regcomp strchr])

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(PIU2/Makefile)
AC_OUTPUT
