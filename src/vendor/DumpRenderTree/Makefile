WEBKIT=/usr/pack/webkit-svnr38860-rp

MACHINE=$(shell uname -m)
 
ifeq ($(MACHINE), i686)
	WEBKIT_PLATFORM=ia32-linux-ubuntu8.04
else ifeq ($(MACHINE), x86_64)
	WEBKIT_PLATFORM=amd64-linux-ubuntu8.04
else
	WEBKIT_PLATFORM=unknown-platform-$(MACHINE)
endif

PKG_CONFIG_PATH=$(WEBKIT)/$(WEBKIT_PLATFORM)/lib/pkgconfig:/usr/lib/pkgconfig
export PKG_CONFIG_PATH

WEBKIT_CFLAGS=`pkg-config --cflags webkit-1.0`
WEBKIT_LIBS=`pkg-config --libs webkit-1.0`
#WEBKIT_EXTRA_LIBS+=$(WEBKIT)/$(WEBKIT_PLATFORM)/lib/libwebkit-1.0.so
WEBKIT_EXTRA_LIBS+=-Wl,--rpath -Wl,$(WEBKIT)/$(WEBKIT_PLATFORM)/lib

CURRENT_DIR=`pwd` 

WEBKIT_EXTRA_CFLAGS=\
 -DBUILDING_GTK__=1 \
 -DNDEBUG \
 -DWTF_CHANGES  \
 -DWTF_USE_ICU_UNICODE=1 \
 -pthread \

GCC_EXTRA_CFLAGS+=\
 -fvisibility-inlines-hidden \
 -fno-rtti -fno-strict-aliasing \
 -fno-exceptions -fvisibility=hidden \
 -Wall -W -Wcast-align -Wchar-subscripts -Wreturn-type -Wformat \
 -Wformat-security -Wno-format-y2k -Wundef -Wmissing-format-attribute \
 -Wpointer-arith -Wwrite-strings -Wno-unused-parameter -Wno-parentheses \
 -O2 \
 -g

LDFLAGS=-pthread -lrt -lm -ldl
CPPFLAGS+=-I$(PWD)/include -I$(PWD)
CFLAGS+=$(WEBKIT_CFLAGS) $(WEBKIT_EXTRA_CFLAGS) $(GCC_EXTRA_CFLAGS)
CXX=g++

SOURCES := WorkQueue.cpp LayoutTestController.cpp 
SOURCES += gtk/DumpRenderTree.cpp gtk/WorkQueueItemGtk.cpp
SOURCES += gtk/LayoutTestControllerGtk.cpp

OBJS := $(SOURCES:%.cpp=%.o)

TARGET := DumpRenderTree

all:	$(TARGET)

$(TARGET): $(OBJS)
	g++ $(LDFLAGS) -o $@ $(OBJS) $(WEBKIT_EXTRA_LIBS) $(WEBKIT_LIBS)

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


env:
	@echo "=============================================================="
	@echo "MACHINE=$(MACHINE)"
	@echo "WEBKIT_PLATFORM=$(WEBKIT_PLATFORM)"
	@echo "PKG_CONFIG_PATH=$(PKG_CONFIG_PATH)"
	@echo "WEBKIT_CFLAGS=$(WEBKIT_CFLAGS) $(WEBKIT_EXTRA_CFLAGS)"
	@echo "WEBKIT_LIBS=$(WEBKIT_LIBS) $(WEBKIT_EXTRA_LIBS)"
	@echo "CFLAGS=$(CFLAGS)"
	@echo "LDFLAGS=$(LDFLAGS)"
	@echo "=============================================================="

clean:
	rm -rf $(OBJS) $(TARGET)

