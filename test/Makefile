# Makefile for test cases of the wk2txt infrastructure classes
#
# $Id: $
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

# wk2txt environment
WK2TXT=../src

# Compile Env
CXX=g++
CFLAGS=-I$(WK2TXT)/

# Document Writer
SOURCES_DOCUMENTWRITER     := test_DocumentWriter.cpp
OBJS_DOCUMENTWRITER        := $(SOURCES_DOCUMENTWRITER:%.cpp=%.o)
EXTRA_OBJS_DOCUMENTWRITER  := $(WK2TXT)/DocumentWriter.o
TARGET_DOCUMENTWRITER      := test_DocumentWriter

# Option Parser
SOURCES_OPTIONPARSER       := test_OptionParser.cpp
OBJS_OPTIONPARSER          := $(SOURCES_OPTIONPARSER:%.cpp=%.o)
EXTRA_OBJS_OPTIONPARSER    := $(WK2TXT)/OptionParser.o 
TARGET_OPTIONPARSER        := test_OptionParser

# targets and objects
TARGET                     := $(TARGET_DOCUMENTWRITER) $(TARGET_OPTIONPARSER)
OBJS                       := $(OBJS_DOCUMENTWRITER) $(OBJS_OPTIONPARSER)

# all test targets
all:	                      $(TARGET_DOCUMENTWRITER) $(TARGET_OPTIONPARSER)

$(TARGET_DOCUMENTWRITER):     $(OBJS_DOCUMENTWRITER) $(EXTRA_OBJS_DOCUMENTWRITER)
	cd $(WK2TXT)
	make
	cd test
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) `xml2-config --cflags` -o test_DocumentWriter.o test_DocumentWriter.cpp
	$(CXX) `xml2-config --libs` -o test_DocumentWriter test_DocumentWriter.o $(EXTRA_OBJS_DOCUMENTWRITER)

$(TARGET_OPTIONSPARSER):      $(OBJS_OPTIONPARSER) $(EXTRA_OBJS_OPTIONPARSER)
	cd $(WK2TXT)
	make
	cd test
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) -o test_OptionParser.o test_OptionParser.cpp
	$(CXX) -o test_OptionParser test_OptionParser.o $(EXTRA_OBJS_OPTIONPARSER)

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

