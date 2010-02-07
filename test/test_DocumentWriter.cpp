#include "DocumentWriter.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Initializing DocumentWriter\n";
  DocumentWriter w;

  w.set_content(string("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));
  
  w.set_meta(string("downloaded-from"), string("http://roman.plessl.info"));
  w.set_meta(string("downloaded-on"), string("05-01-2009 15:00:00Z"));
  
  cout << "dumping as plain text\n";
  w.write_as_plaintext(cout);
  cout << "\n\n";
  cout << "dumping as XML\n";
  w.write_as_xml(cout);

  // writing to a file
  ofstream dumpfile("dump.xml");
  w.write_as_xml(dumpfile);
  dumpfile.close();

  cout << endl << endl;
  cout << "document has been dumped to file dump.xml in XML format" << endl;
  
}

/*
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
*/