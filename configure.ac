#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT(extio_genesis, new, [wd8rde@gmail.com>])
#AM_INIT_AUTOMAKE([-Wall -Werror foreign])
#AM_INIT_AUTOMAKE([-Werror foreign])
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([genesis.cpp])
#AC_CONFIG_HEADER([config.h])

AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
LT_INIT([dlopen])

dnl XXX we need to cover Intel as well as PowerPC
if test $host_cpu = "powerpc" && test $host_vendor = "apple"; then

	dnl set specific macosx paths (fink)
	CFLAGS="$CFLAGS -I/sw/include"
	LDFLAGS="$LDFLAGS -L/sw/lib"
	CPPFLAGS="$CPPFLAGS -I/sw/include"
fi

# Checks for libraries.
AC_CHECK_LIB([hidapi-libusb], [hid_get_serial_number_string], ,
	[AC_MSG_ERROR("Could not find library hidapi-libusb.")])
AC_CHECK_LIB([pthread], [pthread_create])

#check for wxwidgets
AM_OPTIONS_WXCONFIG
reqwx=2.4.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi
 
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_HEADER_STDBOOL
AC_TYPE_UID_T

# Check for hidapi library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_STAT
AC_CHECK_FUNCS([memset])
PKG_CHECK_MODULES(HID, hidapi-libusb >= 0.8)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
