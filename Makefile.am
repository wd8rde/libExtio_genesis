
lib_LTLIBRARIES = libextio_genesis.la
libextio_genesis_la_CPPFLAGS = $(USB_CFLAGS) -std=gnu++0x
libextio_genesis_la_CDFLAGS = $(USB_LIBS)

libextio_genesis_la_SOURCES = \
	extio.cpp\
	extio.h\
	genesis.cpp\
	g59cmd.cpp\
	g59cmdpacket.cpp\
	usbutil.cpp\
	bsd_string.c\
	si570.c\
	simpleini-master/ConvertUTF.c

noinst_PROGRAMS = test
test_SOURCES = main.cpp
test_LDADD = .libs/libextio_genesis.la

