/* $Id: OptionParser.h 54 2009-01-09 12:51:25Z roman $
 * 
 * Copyright (C) 2008 Christian Plessl <christian.plessl@oetiker.ch>
 * Copyright (C) 2008 Roman Plessl <roman.plessl@oetiker.ch>           
 * Copyright (C) 2008 OETIKER+PARTNER AG, Switzerland
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class OptionParser
{
public:
  OptionParser();
  virtual ~OptionParser();

  friend ostream& operator <<(ostream &os, OptionParser &p);

  bool parse(int argc, char* argv[]);

  // accessors
  const bool debug();
  const bool xml();
  const vector<string> urllist();
  const string output_directory();
  const double timeout();

private:

  // instance variables
  bool _debug;
  bool _xml;
  vector<string> _urllist;
  string _output_directory;
  double _timeout;

  // parsing command line arguments
  float parse_float(const char* arg);
  string parse_singleurl(const char* arg);
  vector<string> parse_urllist(const char* filename);

  // helper functions
  const char * usage(void);
  
};
