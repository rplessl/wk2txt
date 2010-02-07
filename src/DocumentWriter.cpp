/* $Id: DocumentWriter.cpp 61 2009-01-09 14:56:01Z roman $
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

#include "DocumentWriter.h"

#include <cassert>

#include <libxml/parser.h>
#include <libxml/tree.h>

DocumentWriter::DocumentWriter() {
  // emtpy
}

DocumentWriter::~DocumentWriter() {
  // empty
}

ostream& operator <<(ostream &os, DocumentWriter &w) {
  assert(false && "not implemented");
  os << "not implemented" << endl;
  return os;
}

void DocumentWriter::set_content(string s) {
  _content = s;
}

void DocumentWriter::set_meta(string name, string value) {
  _metadata[name] = value;
}

void DocumentWriter::write_as_xml(ostream &os) {
  
  xmlDocPtr  doc = NULL;
  xmlNodePtr root_node = NULL;
  xmlNodePtr node = NULL;
  xmlNodePtr node1 = NULL;
  xmlNodePtr dn = NULL;

  // check that libxml version in use is compatible with the version the software
  // has been compiled agains 
  LIBXML_TEST_VERSION;

  // Creates a new document, a node and set it as a root node
  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "documents");
  xmlDocSetRootElement(doc, root_node);

  // creates DTD declaration
  // -- not used at the momemtn
  // xmlDtdPtr dtd = NULL;
  // dtd = xmlCreateIntSubset(doc, BAD_CAST "root", NULL, BAD_CAST "wk2txt.dtd");

  // create document node
  dn = xmlNewChild(root_node, NULL, BAD_CAST "document", NULL);

  // create meta nodes
  map<string, string>::const_iterator it;
  for (it=_metadata.begin(); it != _metadata.end(); ++it) {
    node = xmlNewChild(dn, NULL, BAD_CAST "meta", BAD_CAST it->second.c_str());
    xmlNewProp(node, BAD_CAST "name", BAD_CAST it->first.c_str());
  }
  
  node = xmlNewNode(NULL, BAD_CAST "content");
  node1 = xmlNewText(BAD_CAST _content.c_str());
  xmlAddChild(node, node1);
  xmlAddChild(dn, node);

  // Dump result to memory
  // see http://xmlsoft.org/html/libxml-tree.html#xmlDocDumpFormatMemoryEnc
  xmlChar *addr;
  int len;
  xmlDocDumpFormatMemoryEnc(doc, &addr, &len, "UTF-8", 1);
  os.write((const char*)addr,len);

  // free buffer allocated by xmlDocDumpMemoryEnc
  xmlFree(addr);

  xmlFreeDoc(doc);
  xmlCleanupParser();
  
}

void DocumentWriter::write_as_plaintext(ostream &os) {
  map<string, string>::const_iterator i;
  for (i=_metadata.begin(); i != _metadata.end(); ++i) {
    os << "META " << i->first << "=" << i->second << endl;
  }
  os << _content;
}
