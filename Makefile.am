ACLOCAL_AMFLAGS = -I m4
#SUBDIRS = simpleini-master wx_settings

lib_LTLIBRARIES = libextio_genesis.la
libextio_genesis_la_CPPFLAGS = $(HID_CFLAGS) -std=gnu++0x
libextio_genesis_la_CDFLAGS = $(HID_LIBS)

libextio_genesis_la_SOURCES = \
	extio.cpp\
	extio.h\
	genesis.cpp\
	g11cmd.h\
	g59cmd.h\
	cmdbase.cpp\
	cmdpacket.cpp\
	genesis_observer.cpp\
	hid_util.cpp\
	bsd_string.c\
	si570.c\
	simpleini-master/ConvertUTF.c\
	wx_settings/genesisgui.cpp\
	wx_settings/genesisgui.h\
	wx_settings/wx_genesis.cpp\
	wx_settings/wx_genesis.h \
	wx_settings/wx_genesis_gui.cpp

bin_PROGRAMS = genesis
genesis_SOURCES = main.cpp
genesis_LDADD = .libs/libextio_genesis.a

