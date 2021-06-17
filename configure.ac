AM_CPPFLAGS = \
    -I$(top_srcdir)/include

sbin_PROGRAMS = jnfo

EXTRA_DIST = readme.md license

jnfo_SOURCES = \
    src/main.cpp \
    include/utils.h
everdrone@nvjet jnfo % cat configure.ac
AC_INIT([jnfo], [1.0.0], [])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])

AX_CXX_COMPILE_STDCXX_17

prefix=/usr
sysconfdir=/etc
localstatedir=/var

AC_PROG_CXX
AC_PROG_CC

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT
