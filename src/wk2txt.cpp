/* $Id: wk2txt.cpp 54 2009-01-09 12:51:25Z roman $
 * 
 * Copyright (C) 2007 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008 Alp Toker <alp@nuanti.com>
 *
 * Copyright (C) 2008 Roman Plessl <roman.plessl@oetiker.ch>
 * Copyright (C) 2008 Christian Plessl <christian.plessl@oetiker.ch>
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
 */

#include "wk2txt.h"
#include "OptionParser.h"
#include "DocumentWriter.h"

#include <vendor/DumpRenderTree/LayoutTestController.h>
#include <vendor/DumpRenderTree/WorkQueue.h>
#include <vendor/DumpRenderTree/WorkQueueItem.h>
#include <vendor/sha1/sha1.h>

#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <JavaScriptCore/JavaScript.h>

#include <cassert>
#include <stdlib.h>
#include <string.h>

#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

extern "C" {
  // This API is not yet public.
  extern GSList* webkit_web_frame_get_children(WebKitWebFrame* frame);
  extern gchar* webkit_web_frame_get_inner_text(WebKitWebFrame* frame);
  extern gchar* webkit_web_frame_dump_render_tree(WebKitWebFrame* frame);
}

volatile bool done;

// global variables
OptionParser gOptions;  // store command line options

LayoutTestController* gLayoutTestController = 0;
static WebKitWebView* webView;
WebKitWebFrame* mainFrame = 0;
WebKitWebFrame* topLoadingFrame = 0;
guint waitToDumpWatchdog = 0;

const unsigned maxViewHeight = 600;
const unsigned maxViewWidth = 800;

static gchar* autocorrectURL(const gchar* url)
{
    if (strncmp("http://", url, 7) != 0 && strncmp("https://", url, 8) != 0) {
        GString* string = g_string_new("file://");
        g_string_append(string, url);
        return g_string_free(string, FALSE);
    }

    return g_strdup(url);
}

static bool shouldLogFrameLoadDelegates(const char* pathOrURL)
{
    return strstr(pathOrURL, "loading/");
}

void dumpFrameScrollPosition(WebKitWebFrame* frame)
{

}

void displayWebView()
{

}

static void appendString(gchar*& target, gchar* string)
{
    gchar* oldString = target;
    target = g_strconcat(target, string, NULL);
    g_free(oldString);
}

static gchar* dumpFramesAsText(WebKitWebFrame* frame)
{
    gchar* result = 0;

    // Add header for all but the main frame.
    bool isMainFrame = (webkit_web_view_get_main_frame(webView) == frame);

    gchar* innerText = webkit_web_frame_get_inner_text(frame);
    if (isMainFrame)
        result = g_strdup_printf("%s\n", innerText);
    else {
        const gchar* frameName = webkit_web_frame_get_name(frame);
        result = g_strdup_printf("\n--------\nFrame: '%s'\n--------\n%s\n", frameName, innerText);
    }
    g_free(innerText);

    if (gLayoutTestController->dumpChildFramesAsText()) {
        GSList* children = webkit_web_frame_get_children(frame);
        for (GSList* child = children; child; child = g_slist_next(child))
           appendString(result, dumpFramesAsText((WebKitWebFrame*)children->data));
        g_slist_free(children);
    }

    return result;
}

static void invalidateAnyPreviousWaitToDumpWatchdog()
{
    if (waitToDumpWatchdog) {
        g_source_remove(waitToDumpWatchdog);
        waitToDumpWatchdog = 0;
    }
}

void dump()
{
    invalidateAnyPreviousWaitToDumpWatchdog();

    char* result = 0;
    bool dumpAsText = true;
    gLayoutTestController->setDumpAsText(dumpAsText);
    result = dumpFramesAsText(mainFrame);
    
    if (!result) {
        fprintf(stderr,"ERROR: could not dump main frame as text");
    }   

    // Catch URL (again for LayoutTestController object). The return type
    // of that function of the LayoutTestControll object is
    // const std::string& testPathOrURL() const { return m_testPathOrURL; }
    const std::string url = gLayoutTestController->testPathOrURL();

    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo = gmtime(&rawtime); 
    char *timestamp = asctime(timeinfo);
    timestamp[24] = '\0';

    DocumentWriter w;
    w.set_meta(string("downloaded-from"),url);
    w.set_meta(string("downloaded-on"),timestamp);
    w.set_content(string(result));
     
    // compute filename as SHA1 hash of URL
    char url_sha1_hash[40];
    char urlstring[2048];
    strncpy(urlstring, url.c_str(), 2048);
    sha1((unsigned char*)urlstring, strlen(urlstring), url_sha1_hash, 40);

    string dumpfilename;
    if (gOptions.output_directory() == ""){
      dumpfilename = url_sha1_hash;
    } else {
      dumpfilename = gOptions.output_directory() + "/" + url_sha1_hash;
    }
    if (gOptions.xml()) {
      dumpfilename += ".xml";
    } else {
      dumpfilename += ".txt";
    }

    ofstream dumpfile(dumpfilename.c_str());

		if(gOptions.xml()) {
      w.write_as_xml(dumpfile);
    } else {
      w.write_as_plaintext(dumpfile);
    }
    dumpfile.close();
    done = true;
}

static void setDefaultsToConsistentStateValuesForTesting()
{
    WebKitWebSettings *settings = webkit_web_view_get_settings(webView);

    g_object_set(G_OBJECT(settings),
                 "default-font-family", "Times",
                 "monospace-font-family", "Courier",
                 "serif-font-family", "Times",
                 "sans-serif-font-family", "Helvetica",
                 "default-font-size", 16,
                 "default-monospace-font-size", 13,
                 "minimum-font-size", 1,
                 NULL);
}

static void renderText(const string& testPathOrURL)
{
    assert(!testPathOrURL.empty());

    string pathOrURL(testPathOrURL);
    string expectedPixelHash;

    gchar* url = autocorrectURL(pathOrURL.c_str());
    const string testURL(url);

    gLayoutTestController = new LayoutTestController(testURL, expectedPixelHash);
    topLoadingFrame = 0;
    done = false;

    if (shouldLogFrameLoadDelegates(pathOrURL.c_str()))
        gLayoutTestController->setDumpFrameLoadCallbacks(true);

    WorkQueue::shared()->clear();
    WorkQueue::shared()->setFrozen(false);

    webkit_web_view_open(webView, url);

    g_free(url);
    url = NULL;

    while (!done)
        g_main_context_iteration(NULL, TRUE);

    // A blank load seems to be necessary to reset state after certain tests.
    webkit_web_view_open(webView, "about:blank");

    gLayoutTestController->deref();
    gLayoutTestController = 0;
}

void webViewLoadStarted(WebKitWebView* view, WebKitWebFrame* frame, void*)
{
    // Make sure we only set this once per test.  If it gets cleared, and then set again, we might
    // end up doing two dumps for one test.
    if (!topLoadingFrame && !done)
        topLoadingFrame = frame;
}

static gboolean processWork(void* data)
{
    // quit doing work once a load is in progress
    while (WorkQueue::shared()->count() > 0 && !topLoadingFrame) {
        WorkQueueItem* item = WorkQueue::shared()->dequeue();
        assert(item);
        item->invoke();
        delete item;
    }

    // if we didn't start a new load, then we finished all the commands, so we're ready to dump state
    if (!topLoadingFrame && !gLayoutTestController->waitToDump())
        dump();

    return FALSE;
}

static void webViewLoadFinished(WebKitWebView* view, WebKitWebFrame* frame, void*)
{
    if (frame != topLoadingFrame)
        return;

    topLoadingFrame = 0;
    WorkQueue::shared()->setFrozen(true); // first complete load freezes the queue for the rest of this test
    if (gLayoutTestController->waitToDump())
        return;

    if (WorkQueue::shared()->count())
        g_timeout_add(0, processWork, 0);
     else
        dump();
}

static void webViewWindowObjectCleared(WebKitWebView* view, WebKitWebFrame* frame, JSGlobalContextRef context, JSObjectRef windowObject, gpointer data)
{
    JSValueRef exception = 0;
    assert(gLayoutTestController);

    gLayoutTestController->makeWindowObject(context, windowObject, &exception);
    assert(!exception);
}

int main(int argc, char* argv[])
{
    g_thread_init(NULL);
    gtk_init(&argc, &argv);

    gOptions.parse(argc, argv);
    cerr << gOptions;  

    GtkWidget* window = gtk_window_new(GTK_WINDOW_POPUP);
    GtkContainer* container = GTK_CONTAINER(gtk_fixed_new());
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(container));
    gtk_widget_realize(window);

    webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_container_add(container, GTK_WIDGET(webView));
    gtk_widget_realize(GTK_WIDGET(webView));
    mainFrame = webkit_web_view_get_main_frame(webView);

    g_signal_connect(G_OBJECT(webView), "load-started", G_CALLBACK(webViewLoadStarted), 0);
    g_signal_connect(G_OBJECT(webView), "load-finished", G_CALLBACK(webViewLoadFinished), 0);
    g_signal_connect(G_OBJECT(webView), "window-object-cleared", G_CALLBACK(webViewWindowObjectCleared), 0);
    setDefaultsToConsistentStateValuesForTesting();

    vector<string>::const_iterator i;
    vector<string> urls = gOptions.urllist();

    for (i=urls.begin(); i != urls.end(); ++i) {
			renderText(*i);
    }
    return 0;
}
