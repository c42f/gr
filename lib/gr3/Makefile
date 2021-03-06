       GRDIR = /usr/local/gr
      LIBDIR = $(DESTDIR)$(GRDIR)/lib
      INCDIR = $(DESTDIR)$(GRDIR)/include

UNAME := $(shell uname)

    INCLUDES = -I../../3rdparty -I../../3rdparty/jpeg -I../gr
      GRLIBS = -L../gr -lGR
     PNGLIBS = ../../3rdparty/libpng16/libpng.a
    JPEGLIBS = ../../3rdparty/jpeg/libjpeg.a
       ZLIBS = ../../3rdparty/zlib/libz.a
      CFLAGS = -O3 -Wall -Wextra -pedantic -g -fPIC -DGRDIR=\"$(GRDIR)\" $(EXTRA_CFLAGS)
        SRCS = gr3.c gr3_convenience.c gr3_html.c gr3_povray.c gr3_png.c \
               gr3_jpeg.c gr3_gr.c gr3_mc.c gr3_slices.c
ifeq ($(UNAME), Darwin)
      GLLIBS = -framework OpenGL -framework Cocoa
        SRCS+= gr3_cgl.c
INSTALL_NAME = -Wl,-install_name,@rpath/libGR3.so
     LDFLAGS = $(EXTRA_LDFLAGS) -Wl,-rpath,@loader_path/.
     SOFLAGS = -dynamiclib
else
      GLLIBS =
PLATFORMLIBS = -lX11 -lGL
        SRCS+= gr3_glx.c
INSTALL_NAME =
     LDFLAGS = $(EXTRA_LDFLAGS) -Wl,-rpath,'$$ORIGIN/.'
     SOFLAGS = -shared
endif

default: targets

targets: prerequisites libGR3.so

prerequisites:
	@make -C ../gr

ifneq ($(UNAME), Darwin)
targets: libGR3platform.so
install: install_platform
install_platform:
	@if [ ! -d $(DESTDIR)$(GRDIR) ]; then mkdir -m 755 $(DESTDIR)$(GRDIR); fi
	@if [ ! -d $(LIBDIR) ]; then mkdir -m 755 $(LIBDIR); fi
	cp -r libGR3platform.so $(LIBDIR)
libGR3platform.so: gr3_platform_glx.c
	${CC} -o $@ $^ $(CFLAGS) $(INCLUDES) $(PLATFORMLIBS) $(SOFLAGS) $(LDFLAGS)
endif

libGR3.so: $(SRCS)
	${CC} -o $@ $^ $(CFLAGS) $(INCLUDES) ${ZLIBS} $(JPEGLIBS) $(GLLIBS) $(GRLIBS) $(PNGLIBS) -ldl \
	$(SOFLAGS) $(LDFLAGS) $(INSTALL_NAME)

gr3.c: gr3.h gr3_internals.h
gr3_internals.h: gr3.h gr3_glx.h gr3_cgl.h
gr3_glx.c: gr3_glx.h
gr3_cgl.c: gr3_cgl.h
gr3_convenience.c: gr3_internals.h
gr3_html.c: gr3_internals.h
gr3_povray.c: gr3_internals.h
gr3_png.c: gr3_internals.h
gr3_jpeg.c: gr3_internals.h
gr3_gr.c: gr3_internals.h
gr3_mc.c: gr3.h gr3_mc_data.h
gr3_slices.c: gr3.h

install: targets
	@if [ ! -d $(DESTDIR)$(GRDIR) ]; then mkdir -m 755 $(DESTDIR)$(GRDIR); fi
	@if [ ! -d $(LIBDIR) ]; then mkdir -m 755 $(LIBDIR); fi
	cp -p libGR3.so $(LIBDIR)
	@if [ ! -d $(INCDIR) ]; then mkdir -m 755 $(INCDIR); fi
	cp -p gr3.h $(INCDIR)

clean:
	@make -C ../gr clean
	rm -rf libGR3.so*
	rm -rf libGR3platform.so*
	rm -f *.o
