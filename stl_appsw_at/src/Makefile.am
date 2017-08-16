ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -Wundef \
	    -Wstrict-prototypes \
	    -Wno-trigraphs

AM_CPPFLAGS = -D__packed__= \
        -DIMAGE_APPS_PROC \
        -DFEATURE_Q_SINGLE_LINK \
        -DFEATURE_Q_NO_SELF_QPTR \
        -DFEATURE_LINUX \
        -DFEATURE_NATIVELINUX \
		-I../inc \
		-I$(WORKSPACE)/qticom/application/libs/libstl_socket/inc \
		-I$(WORKSPACE)/qticom/application/libs/libcfg \
		-I$(WORKSPACE)/qticom/application/common
	
	
h_sources = 

requiredlibs = -lpthread

library_includedir = $(pkgincludedir)
library_include_HEADERS = $(h_sources)

c_sources = stl_appsw_at.c 

bin_PROGRAMS = stl_appsw_at
stl_appsw_at_CC = @CC@
stl_appsw_at_SOURCES = $(c_sources) $(h_sources)
stl_appsw_at_CFLAGS = $(AM_CFLAGS) $(DIAG_CFLAGS) -fPIC
stl_appsw_at_CPPFLAGS =$(AM_CPPFLAGS)
stl_appsw_at_LDFLAGS =  -lpthread  -lcfg -lipc  $(stlCFG_LIBS) $(DIAG_LIBS) -shared -version-info 1:0:0
stl_appsw_at_LDADD = $(requiredlibs)

