AC_INIT([greenthumb], [0.1])

AC_PROG_CXX
AC_LANG(C++)

m4_include([/usr/share/aclocal/wxwin.m4])

WX_CONFIG_CHECK([3.0.0], [wxWin=1], [wxWin=0], [adv,core,base,net,html])

if test "$wxWin" != 1; then
    AC_MSG_ERROR([
        wxWidgets must be installed on your system.

        Please check that wx-config is in path, the directory
        where wxWidgets libraries are installed (returned by
        'wx-config --libs' or 'wx-config --static --libs' command)
        is in LD_LIBRARY_PATH or equivalent variable and
        wxWidgets version is 3.0.0 or above.
        ])
fi

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

AC_ARG_WITH([soci-include-path],
  [AS_HELP_STRING([--with-soci-include-path],
  [location of the soci headers, defaults to /usr/include/soci])],
  [CPPFLAGS="$CPPFLAGS -I$withval"],
  [CPPFLAGS="$CPPFLAGS -I/usr/include/soci"])

AC_CHECK_HEADER(
    [soci.h],
    [],
    [AC_MSG_ERROR([soci.h not found])]
)

AC_CHECK_HEADER(
    [sqlite3/soci-sqlite3.h],
    [],
    [AC_MSG_ERROR([sqlite3/soci-sqlite3.h not found])]
)

AC_CHECK_HEADER([json/json.h], [], [AC_MSG_ERROR([json/json.h not found])])

CXXFLAGS="$CXXFLAGS -std=c++11"
AC_CHECK_HEADER(
    [greentop/ExchangeApi.h],
    [],
    [AC_MSG_ERROR([greentop/ExchangeApi.h not found])]
)


AC_OUTPUT(Makefile)


