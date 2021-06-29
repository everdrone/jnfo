AC_INIT([jnfo], [1.1], [])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])

AX_CXX_COMPILE_STDCXX_17

prefix=/usr
sysconfdir=/etc
localstatedir=/var

AC_PROG_CXX
AC_PROG_CC

# PKG_CHECK_MODULES(LIBNCURSES, [ libncurses5 >= 6.1 ])
# AC_SUBST(LIBNCURSES_CFLAGS)
# AC_SUBST(LIBNCURSES_LIBS)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT
