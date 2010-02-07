# $Id: Makefile 68 2009-01-09 15:39:58Z roman $
#
# Copyright (C) 2008 Christian Plessl <christian.plessl@oetiker.ch>
# Copyright (C) 2008 Roman Plessl <roman.plessl@oetiker.ch>
# Copyright (C) 2008 OETIKER+PARTNER AG, Switzerland
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
# 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
#     its contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

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

LibXML_CFLAGS=`xml2-config --cflags`   
LibXML_LIBS=`xml2-config --libs`

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

CPPFLAGS+=-I$(PWD)/vendor/DumpRenderTree/include 
CPPFLAGS+=-I$(PWD)/vendor/DumpRenderTree 
CPPFLAGS+=-I$(PWD)/include
CPPFLAGS+=-I$(PWD)

CFLAGS+=$(WEBKIT_CFLAGS)
CFLAGS+=$(WEBKIT_EXTRA_CFLAGS)
CFLAGS+=$(LibXML_CFLAGS)
CFLAGS+=$(GCC_EXTRA_CFLAGS)
# our own debug routines
# CFLAGS+=-DDEBUG

CXX=g++

SOURCES := vendor/DumpRenderTree/WorkQueue.cpp 
SOURCES += vendor/DumpRenderTree/LayoutTestController.cpp 
SOURCES += vendor/DumpRenderTree/gtk/WorkQueueItemGtk.cpp
SOURCES += vendor/DumpRenderTree/gtk/LayoutTestControllerGtk.cpp
SOURCES += vendor/sha1/sha1.cpp
SOURCES += OptionParser.cpp
SOURCES += DocumentWriter.cpp
SOURCES += wk2txt.cpp

# SOURCES += test_wk2txt.cpp
# SOURCES += vendor/DumpRenderTree/gtk/DumpRenderTree.cpp

OBJS := $(SOURCES:%.cpp=%.o)

TARGET := wk2txt

all:	$(TARGET)

$(TARGET): $(OBJS)
	g++ $(LDFLAGS) -o $@ $(OBJS) $(WEBKIT_EXTRA_LIBS) $(WEBKIT_LIBS) $(LibXML_LIBS)

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


install: 
	@echo "ToBe added"

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

