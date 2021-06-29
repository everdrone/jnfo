AM_CPPFLAGS = \
    -I$(top_srcdir)/include

sbin_PROGRAMS = jnfo

EXTRA_DIST = readme.md license

# jnfo_CPPFLAGS = $(LIBNCURSES_CFLAGS)
# jnfo_LDFLAGS = $(LIBNCURSES_LIBS)

jnfo_SOURCES = \
    include/cpu.h \
    include/defines.h \
    include/filesystem.h \
    include/formatter.h \
    include/gpu.h \
    include/mem.h \
    include/parse_size.h \
    include/power.h \
    include/thermal.h \
    include/utils.h \
    src/main.cpp

# rules to package a .deb file
debfile_name=$(PACKAGE_NAME)-$(PACKAGE_VERSION)

deb: dist
    # extract dist into a new directory
	tar xzf $(top_builddir)/$(debfile_name).tar.gz
	cp -r $(top_srcdir)/debian $(builddir)/$(debfile_name)
    # build into the extracted directory
	cd $(debfile_name) && \
	debuild --no-tgz-check -us -uc -tc
    # clean uo
	rm -r $(debfile_name)
