
lib_LTLIBRARIES = libextio_genesis.la
libextio_genesis_la_CPPFLAGS = $(HID_CFLAGS) -std=gnu++0x
libextio_genesis_la_CDFLAGS = $(HID_LIBS)

libextio_genesis_la_SOURCES = \
	extio.cpp\
	extio.h\
	genesis.cpp\
	cmdbase.cpp\
	cmdpacket.cpp\
	genesis_observer.cpp\
	hid_util.cpp\
	bsd_string.c\
	si570.c\
	simpleini-master/ConvertUTF.c

noinst_PROGRAMS = test
test_SOURCES = main.cpp
test_LDADD = .libs/libextio_genesis.la

