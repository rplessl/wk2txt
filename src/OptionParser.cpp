/* $Id: OptionParser.cpp 56 2009-01-09 13:28:34Z roman $
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
 * 3.  Neither the name of the copyright holder(s) nor the names of any
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "Version.h"
#include "Debugging.h"

#include <getopt.h>
#include <stdlib.h>

#include <string>
#include <fstream>
#include <vector>

#include <iostream>
#include <iterator>
using namespace std;

#include "OptionParser.h"


// declared by getopt
extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

OptionParser::OptionParser()
{
  _debug = false;
  _xml = false;
  _timeout = 3600 * 60.0;
}

OptionParser::~OptionParser()
{
  // empty
}


bool OptionParser::parse(int argc, char* argv[])
{
  int opt_help, opt_input, opt_urlfile, opt_output, opt_timeout, opt_debug, opt_version;
  int opt_xml;
  int ch;
  int option_index;

  static struct option longopts[] = {
    { "input",                required_argument,  &opt_input,      1 },  // 0
    { "urlfile",              required_argument,  &opt_urlfile,    1 },  // 1
    { "output",               required_argument,  &opt_output,     1 },  // 2
    { "timeout",              required_argument,  &opt_timeout,    1 },  // 3
    { "debug",                no_argument,        &opt_debug,      1 },  // 4
    { "help",                 no_argument,        &opt_help,       1 },  // 5
    { "version",              no_argument,        &opt_version,    1 },  // 6
    { "xml",                  no_argument,        &opt_xml,        1 },  // 7
    { NULL,                   0,                  NULL,            0 }
  };


  while ((ch = getopt_long(argc, argv, "", longopts, &option_index)) != -1) {
    
    switch (ch) {

      case 0:
        LOG_DEBUG("option_index=%d ",option_index);

        switch (option_index) {

          case 0: // --input
            LOG_DEBUG("option: --input, value: %s\n",optarg);
            _urllist.push_back(parse_singleurl(optarg));
            break;

          case 1: // --urlfile
            LOG_DEBUG("option: --urlfile, value: %s\n",optarg);
            _urllist = parse_urllist(optarg);
            break;

          case 2: // --output
            LOG_DEBUG("option: --output, value: %s\n",optarg);
            _output_directory = string(optarg);
            break;
            
          case 3: // --timeout
            LOG_DEBUG("option --timeout, value: %s\n",optarg);
            _timeout = parse_float(optarg);
            break;

          case 4: // --debug
            LOG_DEBUG("option --debug\n");
            _debug = true;
            break;
            
          case 5: // --help
            LOG_DEBUG("option --help\n");
            fprintf(stderr,"%s",usage());
            exit(0);
            break;

          case 6: // --version
            fprintf(stdout,"%s",WK2TXT_VERSION);
            exit(0);
            break;
          case 7: // --xml
            LOG_DEBUG("option --xml\n");
            _xml = true;
            break;

        } // end switch(option_index)
      break;
        
      case '?':
        fprintf(stderr,"wk2txt: unknown or ambiguous command-line option\n\n");
        fprintf(stderr,"%s",usage());
        break;
      default:
        ;

      argc -= optind;
      argv += optind;
    
    } // switch(ch)


  } // while

  return true;

}

float OptionParser::parse_float(const char* arg) {
  return atof(arg);
}

string OptionParser::parse_singleurl(const char* arg) {
  return string(arg);
}

vector<string> OptionParser::parse_urllist(const char* filename) {
  vector<string> urllist;
  ifstream ifs(filename);
  string temp;

  while(getline( ifs, temp) ) {
    urllist.push_back(temp);
  }
  return urllist;
}

const bool OptionParser::debug() {
  return _debug;
};

const bool OptionParser::xml() {
  return _xml;
}

const vector<string> OptionParser::urllist() {
  return _urllist;
}

const string OptionParser::output_directory() {
  return _output_directory;
}

const double OptionParser::timeout() {
  return _timeout;
}

ostream& operator <<(ostream &os, OptionParser &p)
{
  os << "OptionParser:\n";
  os << "output directory: " << p.output_directory() << "\n";
  os << "xml: " << p.xml() << endl;
  os << "timeout: " << p.timeout() << "\n";
  os << "debug: " << p.debug() << "\n";
  os << "urllist:\n";
  for(vector<string>::iterator it = p._urllist.begin(); it != p._urllist.end(); ++it) {
    os << "  " << *it << "\n";
  }
  return os;
}


const char * OptionParser::usage(void)
{
return ""  
"usage: wk2txt <options>\n"
"  --input URL               convert <URL> to text\n" 
"  --urlfile file            convert URLs in <file> to text\n" 
"  --output directory        save output in directory <directory>\n"
"  --xml                     dump in XML format\n"
"  --timeout inverval        timeout after <interval> seconds (NOT IMPLEMENTED)\n"
"  --debug                   print diagnostic information\n"
"  --help                    print help on options\n"
"  --version                 print version number\n"
"\n"
;
}

