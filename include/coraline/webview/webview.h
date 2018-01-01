#ifndef WEBVIEW_H
#define WEBVIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#ifndef WEBVIEW_GTK
#define WEBVIEW_GTK
#endif

#if defined(WEBVIEW_GTK)
#include <JavaScriptCore/JavaScript.h>
#include <gtk/gtk.h>
#include <webkit/webkit.h>

struct webview_priv {
  GtkWidget *window;
  GtkWidget *scroller;
  GtkWidget *webview;
  GtkWidget *inspector_window;
  int should_exit;
};
#elif defined(WEBVIEW_WINAPI)
#define CINTERFACE
#include <windows.h>

#include <commctrl.h>
#include <exdisp.h>
#include <mshtmhst.h>
#include <mshtml.h>
#include <shobjidl.h>

#include <stdio.h>

struct webview_priv {
  HWND hwnd;
  IOleObject **browser;
};
#elif defined(WEBVIEW_COCOA)
#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import <objc/runtime.h>

struct webview_priv {
  NSAutoreleasePool *pool;
  NSWindow *window;
  WebView *webview;
  id windowDelegate;
  int should_exit;
};
#else
#error "Define one of: WEBVIEW_GTK, WEBVIEW_COCOA or WEBVIEW_WINAPI"
#endif

struct webview;

typedef void (*webview_external_invoke_cb_t)(struct webview *w,
                                             const char *arg);

struct webview {
  const char *url;
  const char *title;
  int width;
  int height;
  int resizable;
  int debug;
  webview_external_invoke_cb_t external_invoke_cb;
  struct webview_priv priv;
  void *userdata;
};

enum webview_dialog_type {
  WEBVIEW_DIALOG_TYPE_OPEN = 0,
  WEBVIEW_DIALOG_TYPE_SAVE = 1,
  WEBVIEW_DIALOG_TYPE_ALERT = 2
};

typedef void (*webview_dispatch_fn)(struct webview *w, void *arg);

struct webview_dispatch_arg {
  webview_dispatch_fn fn;
  struct webview *w;
  void *arg;
};

#define DEFAULT_URL                                                            \
  "data:text/"                                                                 \
  "html,%3C%21DOCTYPE%20html%3E%0A%3Chtml%20lang=%22en%22%3E%0A%3Chead%3E%"    \
  "3Cmeta%20charset=%22utf-8%22%3E%3Cmeta%20http-equiv=%22X-UA-Compatible%22%" \
  "20content=%22IE=edge%22%3E%3C%2Fhead%3E%0A%3Cbody%3E%3Cdiv%20id=%22app%22%" \
  "3E%3C%2Fdiv%3E%3Cscript%20type=%22text%2Fjavascript%22%3E%3C%2Fscript%3E%"  \
  "3C%2Fbody%3E%0A%3C%2Fhtml%3E"

#define CSS_INJECT_FUNCTION                                                    \
  "(function(e){var "                                                           \
  "t=document.createElement('style'),d=document.head||document."               \
  "getElementsByTagName('head')[0];t.setAttribute('type','text/"               \
  "css'),t.styleSheet?t.styleSheet.cssText=e:t.appendChild(document."          \
  "createTextNode(e)),d.appendChild(t)})"

static const char *webview_check_url(const char *url) {
  if (url == NULL || strlen(url) == 0) {
    return DEFAULT_URL;
  }
  return url;
}

static int webview_init(struct webview *w);
static int webview_loop(struct webview *w, int blocking);
static int webview_eval(struct webview *w, const char *js);
static int webview_inject_css(struct webview *w, const char *css);
static void webview_set_title(struct webview *w, const char *title);
static void webview_dialog(struct webview *w, enum webview_dialog_type dlgtype,
                           int flags, const char *title, const char *arg,
                           char *result, size_t resultsz);
static void webview_dispatch(struct webview *w, webview_dispatch_fn fn,
                             void *arg);
static void webview_terminate(struct webview *w);
static void webview_exit(struct webview *w);
static void webview_debug(const char *format, ...);
static void webview_print_log(const char *s);

static int webview(const char *title, const char *url, int w, int h,
                   int resizable) {
  struct webview webview = {0};
  webview.title = title;
  webview.url = url;
  webview.width = w;
  webview.height = h;
  webview.resizable = resizable;
  int r = webview_init(&webview);
  if (r != 0) {
    return r;
  }
  while (webview_loop(&webview, 1) == 0)
    ;
  webview_exit(&webview);
  return 0;
}

static void webview_debug(const char *format, ...) {
  char buf[4096];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, sizeof(buf), format, ap);
  webview_print_log(buf);
  va_end(ap);
}

static int webview_js_encode(const char *s, char *esc, size_t n) {
  int r = 1; /* At least one byte for trailing zero */
  for (; *s; s++) {
    if (*s >= 0x20 && *s < 0x80 && strchr("<>\\'\"", *s) == NULL) {
      if (n > 0) {
        *esc++ = *s;
        n--;
      }
      r++;
    } else {
      if (n > 0) {
        snprintf(esc, n, "\\x%02x", (int)*s);
        esc += 4;
        n -= 4;
      }
      r+= 4;
    }
  }
  return r;
}

static int webview_inject_css(struct webview *w, const char *css) {
  int n = webview_js_encode(css, NULL, 0);
  char *esc = (char *)calloc(1, sizeof(CSS_INJECT_FUNCTION) + n + 4);
  if (esc == NULL) {
    return -1;
  }
  strcpy(esc, CSS_INJECT_FUNCTION);
  strcat(esc, "(\"");
  webview_js_encode(css, esc + strlen(esc), n);
  strcat(esc, "\")");
  int r = webview_eval(w, esc);
  free(esc);
  return r;
}

#if defined(WEBVIEW_GTK)
static JSValueRef
webview_external_invoke_cb(JSContextRef context, JSObjectRef fn,
                           JSObjectRef thisObject, size_t argc,
                           const JSValueRef args[], JSValueRef *err) {
  (void)fn;
  (void)thisObject;
  (void)argc;
  (void)args;
  (void)err;
  struct webview *w = (struct webview *)JSObjectGetPrivate(thisObject);
  if (w->external_invoke_cb != NULL && argc == 1) {
    JSStringRef js = JSValueToStringCopy(context, args[0], NULL);
    size_t n = JSStringGetMaximumUTF8CStringSize(js);
    char *s = g_new(char, n);
    JSStringGetUTF8CString(js, s, n);
    w->external_invoke_cb(w, s);
    JSStringRelease(js);
    g_free(s);
  }
  return JSValueMakeUndefined(context);
}



static JSValueRef
webview_external_shutdown_cb(JSContextRef context, JSObjectRef fn,
                           JSObjectRef thisObject, size_t argc,
                           const JSValueRef args[], JSValueRef *err) {
  (void)fn;
  (void)thisObject;
  (void)argc;
  (void)args;
  (void)err;
  struct webview *w = (struct webview *)JSObjectGetPrivate(thisObject);
  webview_terminate(w);
  return JSValueMakeUndefined(context);
}


static const JSStaticFunction webview_external_static_funcs[] = {
	    {"invoke_", webview_external_invoke_cb, kJSPropertyAttributeReadOnly},
	    {"shutdown_", webview_external_shutdown_cb, kJSPropertyAttributeReadOnly},
    {NULL, NULL, 0},
};

static const JSClassDefinition webview_external_def = {
    0,          kJSClassAttributeNone,
    "external", NULL,
    NULL,       webview_external_static_funcs,
    NULL,       NULL,
    NULL,       NULL,
    NULL,       NULL,
    NULL,       NULL,
    NULL,       NULL,
    NULL,
};

static void webview_desroy_cb(GtkWidget *widget, gpointer arg) {
  (void)widget;
  struct webview *w = (struct webview *)arg;
  webview_terminate(w);
}

static gboolean webview_context_menu_cb(WebKitWebView *webview,
                                        GtkWidget *default_menu,
                                        WebKitHitTestResult *hit_test_result,
                                        gboolean triggered_with_keyboard,
                                        gpointer userdata) {
  (void)webview;
  (void)default_menu;
  (void)hit_test_result;
  (void)triggered_with_keyboard;
  (void)userdata;
  return TRUE;
}

static void webview_window_object_cleared_cb(WebKitWebView *webview,
                                             WebKitWebFrame *frame,
                                             gpointer context_ptr,
                                             gpointer window_object,
                                             gpointer arg) {
  (void)webview;
  (void)frame;
  (void)window_object;
  JSContextRef context = (JSContextRef)context_ptr;
  JSClassRef cls = JSClassCreate(&webview_external_def);
  JSObjectRef obj = JSObjectMake(context, cls, arg);
  JSObjectRef glob = JSContextGetGlobalObject(context);
  JSStringRef s = JSStringCreateWithUTF8CString("external");
  JSObjectSetProperty(context, glob, s, obj, kJSPropertyAttributeNone, NULL);
}

static WebKitWebView *webview_inspector_create_cb(WebKitWebInspector *inspector,
                                                  WebKitWebView *view,
                                                  gpointer arg) {
  struct webview *w = (struct webview *)arg;
  w->priv.inspector_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(w->priv.inspector_window), "WebInspector");
  gtk_window_set_default_size(GTK_WINDOW(w->priv.inspector_window), 640, 480);
  GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(w->priv.inspector_window), scroll);
  gtk_widget_show(scroll);
  GtkWidget *webview = webkit_web_view_new();
  gtk_container_add(GTK_CONTAINER(scroll), webview);
  return WEBKIT_WEB_VIEW(webview);
}

static gboolean webview_inspector_show_cb(WebKitWebInspector *inspector,
                                          gpointer arg) {
  gtk_widget_show(((struct webview *)arg)->priv.inspector_window);
  return TRUE;
}

static gboolean webview_inspector_hide_cb(WebKitWebInspector *inspector,
                                          gpointer arg) {
  gtk_widget_hide(((struct webview *)arg)->priv.inspector_window);
  return TRUE;
}

static int webview_init(struct webview *w) {
  if (gtk_init_check(0, NULL) == FALSE) {
    return -1;
  }

  w->priv.should_exit = 0;
  w->priv.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(w->priv.window), w->title);

  if (w->resizable) {
    gtk_window_set_default_size(GTK_WINDOW(w->priv.window), w->width,
                                w->height);
  } else {
    gtk_widget_set_size_request(w->priv.window, w->width, w->height);
  }
  gtk_window_set_resizable(GTK_WINDOW(w->priv.window), !!w->resizable);
  gtk_window_set_position(GTK_WINDOW(w->priv.window), GTK_WIN_POS_CENTER);

  w->priv.scroller = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(w->priv.window), w->priv.scroller);

  w->priv.webview = webkit_web_view_new();
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(w->priv.webview),
                           webview_check_url(w->url));
  gtk_container_add(GTK_CONTAINER(w->priv.scroller), w->priv.webview);

  if (w->debug) {
    WebKitWebSettings *settings =
        webkit_web_view_get_settings(WEBKIT_WEB_VIEW(w->priv.webview));
    g_object_set(G_OBJECT(settings), "enable-developer-extras", TRUE, NULL);
    WebKitWebInspector *inspector =
        webkit_web_view_get_inspector(WEBKIT_WEB_VIEW(w->priv.webview));
    g_signal_connect(G_OBJECT(inspector), "inspect-web-view",
                     G_CALLBACK(webview_inspector_create_cb), w);
    g_signal_connect(G_OBJECT(inspector), "show-window",
                     G_CALLBACK(webview_inspector_show_cb), w);
    g_signal_connect(G_OBJECT(inspector), "close-window",
                     G_CALLBACK(webview_inspector_hide_cb), w);
  } else {
    g_signal_connect(G_OBJECT(w->priv.webview), "context-menu",
                     G_CALLBACK(webview_context_menu_cb), w);
  }

  gtk_widget_show_all(w->priv.window);

  g_signal_connect(G_OBJECT(w->priv.window), "destroy",
                   G_CALLBACK(webview_desroy_cb), w);
  g_signal_connect(G_OBJECT(w->priv.webview), "window-object-cleared",
                   G_CALLBACK(webview_window_object_cleared_cb), w);
  return 0;
}

static int webview_loop(struct webview *w, int blocking) {
  gtk_main_iteration_do(blocking);
  return w->priv.should_exit;
}

static void webview_set_title(struct webview *w, const char *title) {
  gtk_window_set_title(GTK_WINDOW(w->priv.window), title);
}

static void webview_dialog(struct webview *w, enum webview_dialog_type dlgtype,
                           int flags, const char *title, const char *arg,
                           char *result, size_t resultsz) {
  GtkWidget *dlg;
  if (result != NULL) {
    result[0] = '\0';
  }
  if (dlgtype == WEBVIEW_DIALOG_TYPE_OPEN ||
      dlgtype == WEBVIEW_DIALOG_TYPE_SAVE) {
    dlg = gtk_file_chooser_dialog_new(
        title, GTK_WINDOW(w->priv.window),
        (dlgtype == WEBVIEW_DIALOG_TYPE_OPEN ? GTK_FILE_CHOOSER_ACTION_OPEN
                                             : GTK_FILE_CHOOSER_ACTION_SAVE),
        "_Cancel", GTK_RESPONSE_CANCEL,
        (dlgtype == WEBVIEW_DIALOG_TYPE_OPEN ? "_Open" : "_Save"),
        GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(dlg), FALSE);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dlg), FALSE);
    gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dlg), TRUE);
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dlg), TRUE);
    gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(dlg), TRUE);
    gint response = gtk_dialog_run(GTK_DIALOG(dlg));
    if (response == GTK_RESPONSE_ACCEPT) {
      gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
      g_strlcpy(result, filename, resultsz);
      g_free(filename);
    }
    gtk_widget_destroy(dlg);
  } else if (dlgtype == WEBVIEW_DIALOG_TYPE_ALERT) {
    dlg =
        gtk_message_dialog_new(GTK_WINDOW(w->priv.window), GTK_DIALOG_MODAL,
                               GTK_MESSAGE_OTHER, GTK_BUTTONS_OK, "%s", title);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dlg), "%s",
                                             arg);
    gtk_dialog_run(GTK_DIALOG(dlg));
    gtk_widget_destroy(dlg);
  }
}

static int webview_eval(struct webview *w, const char *js) {
  webkit_web_view_execute_script(WEBKIT_WEB_VIEW(w->priv.webview), js);
  return 0;
}

static gboolean webview_dispatch_wrapper(gpointer userdata) {
  struct webview_dispatch_arg *arg = (struct webview_dispatch_arg *)userdata;
  (arg->fn)(arg->w, arg->arg);
  g_free(arg);
  return FALSE;
}

static void webview_dispatch(struct webview *w, webview_dispatch_fn fn,
                             void *arg) {
  struct webview_dispatch_arg *context =
      (struct webview_dispatch_arg *)g_new(struct webview_dispatch_arg *, 1);
  context->w = w;
  context->arg = arg;
  context->fn = fn;
  gdk_threads_add_idle(webview_dispatch_wrapper, context);
}

static void webview_terminate(struct webview *w) { w->priv.should_exit = 1; }

static void webview_exit(struct webview *w) {}
static void webview_print_log(const char *s) { fprintf(stderr, "%s\n", s); }

#endif /* WEBVIEW_GTK */

#if defined(WEBVIEW_WINAPI)

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

#define WM_WEBVIEW_DISPATCH (WM_APP + 1)

typedef struct {
  IOleInPlaceFrame frame;
  HWND window;
} _IOleInPlaceFrameEx;

typedef struct {
  IOleInPlaceSite inplace;
  _IOleInPlaceFrameEx frame;
} _IOleInPlaceSiteEx;

typedef struct { IDocHostUIHandler ui; } _IDocHostUIHandlerEx;

typedef struct {
  IOleClientSite client;
  _IOleInPlaceSiteEx inplace;
  _IDocHostUIHandlerEx ui;
  IDispatch external;
} _IOleClientSiteEx;

#ifdef __cplusplus
#define iid_ref(x) &(x)
#define iid_unref(x) *(x)
#else
#define iid_ref(x) (x)
#define iid_unref(x) (x)
#endif

static inline WCHAR *webview_to_utf16(const char *s) {
  DWORD size = MultiByteToWideChar(CP_UTF8, 0, s, -1, 0, 0);
  WCHAR *ws = (WCHAR *)GlobalAlloc(GMEM_FIXED, sizeof(WCHAR) * size);
  if (ws == NULL) {
    return NULL;
  }
  MultiByteToWideChar(CP_UTF8, 0, s, -1, ws, size);
  return ws;
}

static inline char *webview_from_utf16(WCHAR *ws) {
  int n = WideCharToMultiByte(CP_UTF8, 0, ws, -1, NULL, 0, NULL, NULL);
  char *s = (char *)GlobalAlloc(GMEM_FIXED, n);
  if (s == NULL) {
    return NULL;
  }
  WideCharToMultiByte(CP_UTF8, 0, ws, -1, s, n, NULL, NULL);
  return s;
}

static int iid_eq(REFIID a, const IID *b) {
  return memcmp((const void *)iid_ref(a), (const void *)b, sizeof(GUID)) == 0;
}

static HRESULT STDMETHODCALLTYPE JS_QueryInterface(IDispatch FAR *This,
                                                   REFIID riid,
                                                   LPVOID FAR *ppvObj) {
  if (iid_eq(riid, &IID_IDispatch)) {
    *ppvObj = This;
    return S_OK;
  }
  *ppvObj = 0;
  return E_NOINTERFACE;
}
static ULONG STDMETHODCALLTYPE JS_AddRef(IDispatch FAR *This) { return 1; }
static ULONG STDMETHODCALLTYPE JS_Release(IDispatch FAR *This) { return 1; }
static HRESULT STDMETHODCALLTYPE JS_GetTypeInfoCount(IDispatch FAR *This,
                                                     UINT *pctinfo) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE JS_GetTypeInfo(IDispatch FAR *This,
                                                UINT iTInfo, LCID lcid,
                                                ITypeInfo **ppTInfo) {
  return S_OK;
}
#define WEBVIEW_JS_INVOKE_ID 0x1000
static HRESULT STDMETHODCALLTYPE JS_GetIDsOfNames(IDispatch FAR *This,
                                                  REFIID riid,
                                                  LPOLESTR *rgszNames,
                                                  UINT cNames, LCID lcid,
                                                  DISPID *rgDispId) {
  if (cNames != 1) {
    return S_FALSE;
  }
  if (wcscmp(rgszNames[0], L"invoke_") == 0) {
    rgDispId[0] = WEBVIEW_JS_INVOKE_ID;
    return S_OK;
  }
  return S_FALSE;
}

static HRESULT STDMETHODCALLTYPE
JS_Invoke(IDispatch FAR *This, DISPID dispIdMember, REFIID riid, LCID lcid,
          WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult,
          EXCEPINFO *pExcepInfo, UINT *puArgErr) {
  size_t offset = (size_t) & ((_IOleClientSiteEx *)NULL)->external;
  _IOleClientSiteEx *ex = (_IOleClientSiteEx *)((char *)(This)-offset);
  struct webview *w = (struct webview *)GetWindowLongPtr(
      ex->inplace.frame.window, GWLP_USERDATA);
  if (pDispParams->cArgs == 1 && pDispParams->rgvarg[0].vt == VT_BSTR) {
    BSTR bstr = pDispParams->rgvarg[0].bstrVal;
    char *s = webview_from_utf16(bstr);
    if (s != NULL) {
      if (dispIdMember == WEBVIEW_JS_INVOKE_ID) {
        if (w->external_invoke_cb != NULL) {
          w->external_invoke_cb(w, s);
        }
      } else {
        return S_FALSE;
      }
      GlobalFree(s);
    }
  }
  return S_OK;
}

static IDispatchVtbl ExternalDispatchTable = {
    JS_QueryInterface, JS_AddRef,        JS_Release, JS_GetTypeInfoCount,
    JS_GetTypeInfo,    JS_GetIDsOfNames, JS_Invoke};

static ULONG STDMETHODCALLTYPE Site_AddRef(IOleClientSite FAR *This) {
  return 1;
}
static ULONG STDMETHODCALLTYPE Site_Release(IOleClientSite FAR *This) {
  return 1;
}
static HRESULT STDMETHODCALLTYPE Site_SaveObject(IOleClientSite FAR *This) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE Site_GetMoniker(IOleClientSite FAR *This,
                                                 DWORD dwAssign,
                                                 DWORD dwWhichMoniker,
                                                 IMoniker **ppmk) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE
Site_GetContainer(IOleClientSite FAR *This, LPOLECONTAINER FAR *ppContainer) {
  *ppContainer = 0;
  return E_NOINTERFACE;
}
static HRESULT STDMETHODCALLTYPE Site_ShowObject(IOleClientSite FAR *This) {
  return NOERROR;
}
static HRESULT STDMETHODCALLTYPE Site_OnShowWindow(IOleClientSite FAR *This,
                                                   BOOL fShow) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE
Site_RequestNewObjectLayout(IOleClientSite FAR *This) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE Site_QueryInterface(IOleClientSite FAR *This,
                                                     REFIID riid,
                                                     void **ppvObject) {
  if (iid_eq(riid, &IID_IUnknown) || iid_eq(riid, &IID_IOleClientSite))
    *ppvObject = &((_IOleClientSiteEx *)This)->client;
  else if (iid_eq(riid, &IID_IOleInPlaceSite))
    *ppvObject = &((_IOleClientSiteEx *)This)->inplace;
  else if (iid_eq(riid, &IID_IDocHostUIHandler))
    *ppvObject = &((_IOleClientSiteEx *)This)->ui;
  else {
    *ppvObject = 0;
    return (E_NOINTERFACE);
  }
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE InPlace_QueryInterface(
    IOleInPlaceSite FAR *This, REFIID riid, LPVOID FAR *ppvObj) {
  return (Site_QueryInterface(
      (IOleClientSite *)((char *)This - sizeof(IOleClientSite)), riid, ppvObj));
}
static ULONG STDMETHODCALLTYPE InPlace_AddRef(IOleInPlaceSite FAR *This) {
  return 1;
}
static ULONG STDMETHODCALLTYPE InPlace_Release(IOleInPlaceSite FAR *This) {
  return 1;
}
static HRESULT STDMETHODCALLTYPE InPlace_GetWindow(IOleInPlaceSite FAR *This,
                                                   HWND FAR *lphwnd) {
  *lphwnd = ((_IOleInPlaceSiteEx FAR *)This)->frame.window;
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
InPlace_ContextSensitiveHelp(IOleInPlaceSite FAR *This, BOOL fEnterMode) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE
InPlace_CanInPlaceActivate(IOleInPlaceSite FAR *This) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
InPlace_OnInPlaceActivate(IOleInPlaceSite FAR *This) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
InPlace_OnUIActivate(IOleInPlaceSite FAR *This) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE InPlace_GetWindowContext(
    IOleInPlaceSite FAR *This, LPOLEINPLACEFRAME FAR *lplpFrame,
    LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect,
    LPOLEINPLACEFRAMEINFO lpFrameInfo) {
  *lplpFrame = (LPOLEINPLACEFRAME) & ((_IOleInPlaceSiteEx *)This)->frame;
  *lplpDoc = 0;
  lpFrameInfo->fMDIApp = FALSE;
  lpFrameInfo->hwndFrame = ((_IOleInPlaceFrameEx *)*lplpFrame)->window;
  lpFrameInfo->haccel = 0;
  lpFrameInfo->cAccelEntries = 0;
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE InPlace_Scroll(IOleInPlaceSite FAR *This,
                                                SIZE scrollExtent) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE
InPlace_OnUIDeactivate(IOleInPlaceSite FAR *This, BOOL fUndoable) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
InPlace_OnInPlaceDeactivate(IOleInPlaceSite FAR *This) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
InPlace_DiscardUndoState(IOleInPlaceSite FAR *This) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE
InPlace_DeactivateAndUndo(IOleInPlaceSite FAR *This) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE
InPlace_OnPosRectChange(IOleInPlaceSite FAR *This, LPCRECT lprcPosRect) {
  IOleObject *browserObject;
  IOleInPlaceObject *inplace;
  browserObject = *((IOleObject **)((char *)This - sizeof(IOleObject *) -
                                    sizeof(IOleClientSite)));
  if (!browserObject->lpVtbl->QueryInterface(browserObject,
                                             iid_unref(&IID_IOleInPlaceObject),
                                             (void **)&inplace)) {
    inplace->lpVtbl->SetObjectRects(inplace, lprcPosRect, lprcPosRect);
    inplace->lpVtbl->Release(inplace);
  }
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE Frame_QueryInterface(
    IOleInPlaceFrame FAR *This, REFIID riid, LPVOID FAR *ppvObj) {
  return E_NOTIMPL;
}
static ULONG STDMETHODCALLTYPE Frame_AddRef(IOleInPlaceFrame FAR *This) {
  return 1;
}
static ULONG STDMETHODCALLTYPE Frame_Release(IOleInPlaceFrame FAR *This) {
  return 1;
}
static HRESULT STDMETHODCALLTYPE Frame_GetWindow(IOleInPlaceFrame FAR *This,
                                                 HWND FAR *lphwnd) {
  *lphwnd = ((_IOleInPlaceFrameEx *)This)->window;
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
Frame_ContextSensitiveHelp(IOleInPlaceFrame FAR *This, BOOL fEnterMode) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE Frame_GetBorder(IOleInPlaceFrame FAR *This,
                                                 LPRECT lprectBorder) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE Frame_RequestBorderSpace(
    IOleInPlaceFrame FAR *This, LPCBORDERWIDTHS pborderwidths) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE Frame_SetBorderSpace(
    IOleInPlaceFrame FAR *This, LPCBORDERWIDTHS pborderwidths) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE Frame_SetActiveObject(
    IOleInPlaceFrame FAR *This, IOleInPlaceActiveObject *pActiveObject,
    LPCOLESTR pszObjName) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
Frame_InsertMenus(IOleInPlaceFrame FAR *This, HMENU hmenuShared,
                  LPOLEMENUGROUPWIDTHS lpMenuWidths) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE Frame_SetMenu(IOleInPlaceFrame FAR *This,
                                               HMENU hmenuShared,
                                               HOLEMENU holemenu,
                                               HWND hwndActiveObject) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE Frame_RemoveMenus(IOleInPlaceFrame FAR *This,
                                                   HMENU hmenuShared) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE Frame_SetStatusText(IOleInPlaceFrame FAR *This,
                                                     LPCOLESTR pszStatusText) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
Frame_EnableModeless(IOleInPlaceFrame FAR *This, BOOL fEnable) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
Frame_TranslateAccelerator(IOleInPlaceFrame FAR *This, LPMSG lpmsg, WORD wID) {
  return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE UI_QueryInterface(IDocHostUIHandler FAR *This,
                                                   REFIID riid,
                                                   LPVOID FAR *ppvObj) {
  return (Site_QueryInterface((IOleClientSite *)((char *)This -
                                                 sizeof(IOleClientSite) -
                                                 sizeof(_IOleInPlaceSiteEx)),
                              riid, ppvObj));
}
static ULONG STDMETHODCALLTYPE UI_AddRef(IDocHostUIHandler FAR *This) {
  return 1;
}
static ULONG STDMETHODCALLTYPE UI_Release(IDocHostUIHandler FAR *This) {
  return 1;
}
static HRESULT STDMETHODCALLTYPE UI_ShowContextMenu(
    IDocHostUIHandler FAR *This, DWORD dwID, POINT __RPC_FAR *ppt,
    IUnknown __RPC_FAR *pcmdtReserved, IDispatch __RPC_FAR *pdispReserved) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
UI_GetHostInfo(IDocHostUIHandler FAR *This, DOCHOSTUIINFO __RPC_FAR *pInfo) {
  pInfo->cbSize = sizeof(DOCHOSTUIINFO);
  pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;
  pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE UI_ShowUI(
    IDocHostUIHandler FAR *This, DWORD dwID,
    IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
    IOleCommandTarget __RPC_FAR *pCommandTarget,
    IOleInPlaceFrame __RPC_FAR *pFrame, IOleInPlaceUIWindow __RPC_FAR *pDoc) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE UI_HideUI(IDocHostUIHandler FAR *This) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE UI_UpdateUI(IDocHostUIHandler FAR *This) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE UI_EnableModeless(IDocHostUIHandler FAR *This,
                                                   BOOL fEnable) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
UI_OnDocWindowActivate(IDocHostUIHandler FAR *This, BOOL fActivate) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
UI_OnFrameWindowActivate(IDocHostUIHandler FAR *This, BOOL fActivate) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
UI_ResizeBorder(IDocHostUIHandler FAR *This, LPCRECT prcBorder,
                IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow) {
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE
UI_TranslateAccelerator(IDocHostUIHandler FAR *This, LPMSG lpMsg,
                        const GUID __RPC_FAR *pguidCmdGroup, DWORD nCmdID) {
  return S_FALSE;
}
static HRESULT STDMETHODCALLTYPE UI_GetOptionKeyPath(
    IDocHostUIHandler FAR *This, LPOLESTR __RPC_FAR *pchKey, DWORD dw) {
  return S_FALSE;
}
static HRESULT STDMETHODCALLTYPE UI_GetDropTarget(
    IDocHostUIHandler FAR *This, IDropTarget __RPC_FAR *pDropTarget,
    IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget) {
  return S_FALSE;
}
static HRESULT STDMETHODCALLTYPE UI_GetExternal(
    IDocHostUIHandler FAR *This, IDispatch __RPC_FAR *__RPC_FAR *ppDispatch) {
  *ppDispatch = (IDispatch *)(This + 1);
  return S_OK;
}
static HRESULT STDMETHODCALLTYPE UI_TranslateUrl(
    IDocHostUIHandler FAR *This, DWORD dwTranslate, OLECHAR __RPC_FAR *pchURLIn,
    OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut) {
  *ppchURLOut = 0;
  return S_FALSE;
}
static HRESULT STDMETHODCALLTYPE
UI_FilterDataObject(IDocHostUIHandler FAR *This, IDataObject __RPC_FAR *pDO,
                    IDataObject __RPC_FAR *__RPC_FAR *ppDORet) {
  *ppDORet = 0;
  return S_FALSE;
}

static const TCHAR *classname = "WebView";
static const SAFEARRAYBOUND ArrayBound = {1, 0};

static IOleClientSiteVtbl MyIOleClientSiteTable = {
    Site_QueryInterface, Site_AddRef,       Site_Release,
    Site_SaveObject,     Site_GetMoniker,   Site_GetContainer,
    Site_ShowObject,     Site_OnShowWindow, Site_RequestNewObjectLayout};
static IOleInPlaceSiteVtbl MyIOleInPlaceSiteTable = {
    InPlace_QueryInterface,
    InPlace_AddRef,
    InPlace_Release,
    InPlace_GetWindow,
    InPlace_ContextSensitiveHelp,
    InPlace_CanInPlaceActivate,
    InPlace_OnInPlaceActivate,
    InPlace_OnUIActivate,
    InPlace_GetWindowContext,
    InPlace_Scroll,
    InPlace_OnUIDeactivate,
    InPlace_OnInPlaceDeactivate,
    InPlace_DiscardUndoState,
    InPlace_DeactivateAndUndo,
    InPlace_OnPosRectChange};

static IOleInPlaceFrameVtbl MyIOleInPlaceFrameTable = {
    Frame_QueryInterface,
    Frame_AddRef,
    Frame_Release,
    Frame_GetWindow,
    Frame_ContextSensitiveHelp,
    Frame_GetBorder,
    Frame_RequestBorderSpace,
    Frame_SetBorderSpace,
    Frame_SetActiveObject,
    Frame_InsertMenus,
    Frame_SetMenu,
    Frame_RemoveMenus,
    Frame_SetStatusText,
    Frame_EnableModeless,
    Frame_TranslateAccelerator};

static IDocHostUIHandlerVtbl MyIDocHostUIHandlerTable = {
    UI_QueryInterface,
    UI_AddRef,
    UI_Release,
    UI_ShowContextMenu,
    UI_GetHostInfo,
    UI_ShowUI,
    UI_HideUI,
    UI_UpdateUI,
    UI_EnableModeless,
    UI_OnDocWindowActivate,
    UI_OnFrameWindowActivate,
    UI_ResizeBorder,
    UI_TranslateAccelerator,
    UI_GetOptionKeyPath,
    UI_GetDropTarget,
    UI_GetExternal,
    UI_TranslateUrl,
    UI_FilterDataObject};

static void UnEmbedBrowserObject(struct webview *w) {
  if (w->priv.browser != NULL) {
    (*w->priv.browser)->lpVtbl->Close(*w->priv.browser, OLECLOSE_NOSAVE);
    (*w->priv.browser)->lpVtbl->Release(*w->priv.browser);
    GlobalFree(w->priv.browser);
    w->priv.browser = NULL;
  }
}

static int EmbedBrowserObject(struct webview *w) {
  RECT rect;
  IWebBrowser2 *webBrowser2 = NULL;
  LPCLASSFACTORY pClassFactory = NULL;
  _IOleClientSiteEx *_iOleClientSiteEx = NULL;
  IOleObject **browser = (IOleObject **)GlobalAlloc(
      GMEM_FIXED, sizeof(IOleObject *) + sizeof(_IOleClientSiteEx));
  if (browser == NULL) {
    goto error;
  }
  w->priv.browser = browser;

  _iOleClientSiteEx = (_IOleClientSiteEx *)(browser + 1);
  _iOleClientSiteEx->client.lpVtbl = &MyIOleClientSiteTable;
  _iOleClientSiteEx->inplace.inplace.lpVtbl = &MyIOleInPlaceSiteTable;
  _iOleClientSiteEx->inplace.frame.frame.lpVtbl = &MyIOleInPlaceFrameTable;
  _iOleClientSiteEx->inplace.frame.window = w->priv.hwnd;
  _iOleClientSiteEx->ui.ui.lpVtbl = &MyIDocHostUIHandlerTable;
  _iOleClientSiteEx->external.lpVtbl = &ExternalDispatchTable;

  if (CoGetClassObject(iid_unref(&CLSID_WebBrowser),
                       CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, NULL,
                       iid_unref(&IID_IClassFactory),
                       (void **)&pClassFactory) != S_OK) {
    goto error;
  }

  if (pClassFactory == NULL) {
    goto error;
  }

  if (pClassFactory->lpVtbl->CreateInstance(pClassFactory, 0,
                                            iid_unref(&IID_IOleObject),
                                            (void **)browser) != S_OK) {
    goto error;
  }
  pClassFactory->lpVtbl->Release(pClassFactory);
  if ((*browser)->lpVtbl->SetClientSite(
          *browser, (IOleClientSite *)_iOleClientSiteEx) != S_OK) {
    goto error;
  }
  (*browser)->lpVtbl->SetHostNames(*browser, L"My Host Name", 0);

  if (OleSetContainedObject((struct IUnknown *)(*browser), TRUE) != S_OK) {
    goto error;
  }
  GetClientRect(w->priv.hwnd, &rect);
  if ((*browser)->lpVtbl->DoVerb((*browser), OLEIVERB_SHOW, NULL,
                                 (IOleClientSite *)_iOleClientSiteEx, -1,
                                 w->priv.hwnd, &rect) != S_OK) {
    goto error;
  }
  if ((*browser)->lpVtbl->QueryInterface((*browser),
                                         iid_unref(&IID_IWebBrowser2),
                                         (void **)&webBrowser2) != S_OK) {
    goto error;
  }

  webBrowser2->lpVtbl->put_Left(webBrowser2, 0);
  webBrowser2->lpVtbl->put_Top(webBrowser2, 0);
  webBrowser2->lpVtbl->put_Width(webBrowser2, rect.right);
  webBrowser2->lpVtbl->put_Height(webBrowser2, rect.bottom);
  webBrowser2->lpVtbl->Release(webBrowser2);

  return 0;
error:
  UnEmbedBrowserObject(w);
  if (pClassFactory != NULL) {
    pClassFactory->lpVtbl->Release(pClassFactory);
  }
  if (browser != NULL) {
    GlobalFree(browser);
  }
  return -1;
}

#define WEBVIEW_DATA_URL_PREFIX "data:text/html,"
static long DisplayHTMLPage(struct webview *w) {
  IWebBrowser2 *webBrowser2;
  VARIANT myURL;
  LPDISPATCH lpDispatch;
  IHTMLDocument2 *htmlDoc2;
  BSTR bstr;
  IOleObject *browserObject;
  SAFEARRAY *sfArray;
  VARIANT *pVar;
  browserObject = *w->priv.browser;
  int isDataURL = 0;
  const char *webview_url = webview_check_url(w->url);
  if (!browserObject->lpVtbl->QueryInterface(
          browserObject, iid_unref(&IID_IWebBrowser2), (void **)&webBrowser2)) {
    LPCSTR webPageName;
    isDataURL = (strncmp(webview_url, WEBVIEW_DATA_URL_PREFIX,
                         strlen(WEBVIEW_DATA_URL_PREFIX)) == 0);
    if (isDataURL) {
      webPageName = "about:blank";
    } else {
      webPageName = (LPCSTR)webview_url;
    }
    VariantInit(&myURL);
    myURL.vt = VT_BSTR;
#ifndef UNICODE
    {
      wchar_t *buffer = webview_to_utf16(webPageName);
      if (buffer == NULL) {
        goto badalloc;
      }
      myURL.bstrVal = SysAllocString(buffer);
      GlobalFree(buffer);
    }
#else
    myURL.bstrVal = SysAllocString(webPageName);
#endif
    if (!myURL.bstrVal) {
    badalloc:
      webBrowser2->lpVtbl->Release(webBrowser2);
      return (-6);
    }
    webBrowser2->lpVtbl->Navigate2(webBrowser2, &myURL, 0, 0, 0, 0);
    VariantClear(&myURL);
    if (!isDataURL) {
      return 0;
    }

    char *url = (char *)calloc(1, strlen(webview_url) + 1);
    char *q = url;
    for (const char *p = webview_url + strlen(WEBVIEW_DATA_URL_PREFIX); *q = *p;
         p++, q++) {
      if (*q == '%' && *(p + 1) && *(p + 2)) {
        sscanf(p + 1, "%02x", q);
        p = p + 2;
      }
    }

    if (webBrowser2->lpVtbl->get_Document(webBrowser2, &lpDispatch) == S_OK) {
      if (lpDispatch->lpVtbl->QueryInterface(lpDispatch,
                                             iid_unref(&IID_IHTMLDocument2),
                                             (void **)&htmlDoc2) == S_OK) {
        if ((sfArray = SafeArrayCreate(VT_VARIANT, 1,
                                       (SAFEARRAYBOUND *)&ArrayBound))) {
          if (!SafeArrayAccessData(sfArray, (void **)&pVar)) {
            pVar->vt = VT_BSTR;
#ifndef UNICODE
            {
              wchar_t *buffer = webview_to_utf16(url);
              if (buffer == NULL) {
                goto release;
              }
              bstr = SysAllocString(buffer);
              GlobalFree(buffer);
            }
#else
            bstr = SysAllocString(string);
#endif
            if ((pVar->bstrVal = bstr)) {
              htmlDoc2->lpVtbl->write(htmlDoc2, sfArray);
              htmlDoc2->lpVtbl->close(htmlDoc2);
            }
          }
          SafeArrayDestroy(sfArray);
        }
      release:
        free(url);
        htmlDoc2->lpVtbl->Release(htmlDoc2);
      }
      lpDispatch->lpVtbl->Release(lpDispatch);
    }
    webBrowser2->lpVtbl->Release(webBrowser2);
    return (0);
  }
  return (-5);
}

static LRESULT CALLBACK wndproc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                LPARAM lParam) {
  struct webview *w = (struct webview *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  switch (uMsg) {
  case WM_CREATE:
    w = (struct webview *)((CREATESTRUCT *)lParam)->lpCreateParams;
    w->priv.hwnd = hwnd;
    return EmbedBrowserObject(w);
  case WM_DESTROY:
    UnEmbedBrowserObject(w);
    PostQuitMessage(0);
    return TRUE;
  case WM_SIZE: {
    IWebBrowser2 *webBrowser2;
    IOleObject *browser = *w->priv.browser;
    if (browser->lpVtbl->QueryInterface(browser, iid_unref(&IID_IWebBrowser2),
                                        (void **)&webBrowser2) == S_OK) {
      RECT rect;
      GetClientRect(hwnd, &rect);
      webBrowser2->lpVtbl->put_Width(webBrowser2, rect.right);
      webBrowser2->lpVtbl->put_Height(webBrowser2, rect.bottom);
    }
    return TRUE;
  }
  case WM_WEBVIEW_DISPATCH: {
    webview_dispatch_fn f = (webview_dispatch_fn)wParam;
    void *arg = (void *)lParam;
    (*f)(w, arg);
    return TRUE;
  }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#define WEBVIEW_KEY_FEATURE_BROWSER_EMULATION                                  \
  "Software\\Microsoft\\Internet "                                             \
  "Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION"

static int webview_fix_ie_compat_mode() {
  HKEY hKey;
  DWORD ie_version = 11000;
  TCHAR appname[MAX_PATH + 1];
  TCHAR *p;
  if (GetModuleFileName(NULL, appname, MAX_PATH + 1) == 0) {
    return -1;
  }
  for (p = &appname[strlen(appname) - 1]; p != appname && *p != '\\'; p--)
    ;
  p++;
  if (RegCreateKey(HKEY_CURRENT_USER, WEBVIEW_KEY_FEATURE_BROWSER_EMULATION,
                   &hKey) != ERROR_SUCCESS) {
    return -1;
  }
  if (RegSetValueEx(hKey, p, 0, REG_DWORD, (BYTE *)&ie_version,
                    sizeof(ie_version)) != ERROR_SUCCESS) {
    RegCloseKey(hKey);
    return -1;
  }
  RegCloseKey(hKey);
  return 0;
}

static int webview_init(struct webview *w) {
  WNDCLASSEX wc;
  HINSTANCE hInstance;
  STARTUPINFO info;
  DWORD style;
  RECT clientRect;
  RECT rect;

  if (webview_fix_ie_compat_mode() < 0) {
    return -1;
  }

  hInstance = GetModuleHandle(NULL);
  if (hInstance == NULL) {
    return -1;
  }
  GetStartupInfo(&info);
  if (OleInitialize(NULL) != S_OK) {
    return -1;
  }
  ZeroMemory(&wc, sizeof(WNDCLASSEX));
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.hInstance = hInstance;
  wc.lpfnWndProc = wndproc;
  wc.lpszClassName = classname;
  RegisterClassEx(&wc);

  style = WS_OVERLAPPEDWINDOW;
  if (!w->resizable) {
    style = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
  }

  rect.left = 0;
  rect.top = 0;
  rect.right = w->width;
  rect.bottom = w->height;
  AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);

  GetClientRect(GetDesktopWindow(), &clientRect);
  int left = (clientRect.right / 2) - ((rect.right - rect.left) / 2);
  int top = (clientRect.bottom / 2) - ((rect.bottom - rect.top) / 2);
  rect.right = rect.right - rect.left + left;
  rect.left = left;
  rect.bottom = rect.bottom - rect.top + top;
  rect.top = top;

  w->priv.hwnd =
      CreateWindowEx(0, classname, w->title, style, rect.left, rect.top,
                     rect.right - rect.left, rect.bottom - rect.top,
                     HWND_DESKTOP, NULL, hInstance, (void *)w);
  if (w->priv.hwnd == 0) {
    OleUninitialize();
    return -1;
  }

  SetWindowLongPtr(w->priv.hwnd, GWLP_USERDATA, (LONG_PTR)w);

  DisplayHTMLPage(w);

  SetWindowText(w->priv.hwnd, w->title);
  ShowWindow(w->priv.hwnd, info.wShowWindow);
  UpdateWindow(w->priv.hwnd);
  SetFocus(w->priv.hwnd);

  return 0;
}

static int webview_loop(struct webview *w, int blocking) {
  MSG msg;
  if (blocking) {
    GetMessage(&msg, 0, 0, 0);
  } else {
    PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
  }
  switch (msg.message) {
  case WM_QUIT:
    return -1;
  case WM_COMMAND:
  case WM_KEYDOWN:
  case WM_KEYUP: {
    HRESULT r = S_OK;
    IWebBrowser2 *webBrowser2;
    IOleObject *browser = *w->priv.browser;
    if (browser->lpVtbl->QueryInterface(browser, iid_unref(&IID_IWebBrowser2),
                                        (void **)&webBrowser2) == S_OK) {
      IOleInPlaceActiveObject *pIOIPAO;
      if (browser->lpVtbl->QueryInterface(
              browser, iid_unref(&IID_IOleInPlaceActiveObject),
              (void **)&pIOIPAO) == S_OK) {
        r = pIOIPAO->lpVtbl->TranslateAccelerator(pIOIPAO, &msg);
        pIOIPAO->lpVtbl->Release(pIOIPAO);
      }
      webBrowser2->lpVtbl->Release(webBrowser2);
    }
    if (r != S_FALSE) {
      break;
    }
  }
  default:
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

static int webview_eval(struct webview *w, const char *js) {
  IWebBrowser2 *webBrowser2;
  IHTMLDocument2 *htmlDoc2;
  IDispatch *docDispatch;
  IDispatch *scriptDispatch;
  if ((*w->priv.browser)
          ->lpVtbl->QueryInterface((*w->priv.browser),
                                   iid_unref(&IID_IWebBrowser2),
                                   (void **)&webBrowser2) != S_OK) {
    return -1;
  }

  if (webBrowser2->lpVtbl->get_Document(webBrowser2, &docDispatch) != S_OK) {
    return -1;
  }
  if (docDispatch->lpVtbl->QueryInterface(docDispatch,
                                          iid_unref(&IID_IHTMLDocument2),
                                          (void **)&htmlDoc2) != S_OK) {
    return -1;
  }
  if (htmlDoc2->lpVtbl->get_Script(htmlDoc2, &scriptDispatch) != S_OK) {
    return -1;
  }
  DISPID dispid;
  BSTR evalStr = SysAllocString(L"eval");
  if (scriptDispatch->lpVtbl->GetIDsOfNames(
          scriptDispatch, iid_unref(&IID_NULL), &evalStr, 1,
          LOCALE_SYSTEM_DEFAULT, &dispid) != S_OK) {
    SysFreeString(evalStr);
    return -1;
  }
  SysFreeString(evalStr);

  DISPPARAMS params;
  VARIANT arg;
  VARIANT result;
  EXCEPINFO excepInfo;
  UINT nArgErr = (UINT)-1;
  params.cArgs = 1;
  params.cNamedArgs = 0;
  params.rgvarg = &arg;
  arg.vt = VT_BSTR;
  static const char *prologue = "(function(){";
  static const char *epilogue = ";})();";
  int n = strlen(prologue) + strlen(epilogue) + strlen(js) + 1;
  char *eval = (char *)malloc(n);
  strcpy(eval, prologue);
  strcat(eval, js);
  strcat(eval, epilogue);
  wchar_t *buf = webview_to_utf16(eval);
  if (buf == NULL) {
    return -1;
  }
  arg.bstrVal = SysAllocString(buf);
  if (scriptDispatch->lpVtbl->Invoke(
          scriptDispatch, dispid, iid_unref(&IID_NULL), 0, DISPATCH_METHOD,
          &params, &result, &excepInfo, &nArgErr) != S_OK) {
    return -1;
  }
  SysFreeString(arg.bstrVal);
  free(eval);
  scriptDispatch->lpVtbl->Release(scriptDispatch);
  htmlDoc2->lpVtbl->Release(htmlDoc2);
  docDispatch->lpVtbl->Release(docDispatch);
  return 0;
}

static void webview_dispatch(struct webview *w, webview_dispatch_fn fn,
                             void *arg) {
  PostMessageW(w->priv.hwnd, WM_WEBVIEW_DISPATCH, (WPARAM)fn, (LPARAM)arg);
}

static void webview_set_title(struct webview *w, const char *title) {
  SetWindowText(w->priv.hwnd, title);
}

/* These are missing parts from MinGW */
#ifndef __IFileDialog_INTERFACE_DEFINED__
#define __IFileDialog_INTERFACE_DEFINED__
enum _FILEOPENDIALOGOPTIONS {
  FOS_OVERWRITEPROMPT = 0x2,
  FOS_STRICTFILETYPES = 0x4,
  FOS_NOCHANGEDIR = 0x8,
  FOS_PICKFOLDERS = 0x20,
  FOS_FORCEFILESYSTEM = 0x40,
  FOS_ALLNONSTORAGEITEMS = 0x80,
  FOS_NOVALIDATE = 0x100,
  FOS_ALLOWMULTISELECT = 0x200,
  FOS_PATHMUSTEXIST = 0x800,
  FOS_FILEMUSTEXIST = 0x1000,
  FOS_CREATEPROMPT = 0x2000,
  FOS_SHAREAWARE = 0x4000,
  FOS_NOREADONLYRETURN = 0x8000,
  FOS_NOTESTFILECREATE = 0x10000,
  FOS_HIDEMRUPLACES = 0x20000,
  FOS_HIDEPINNEDPLACES = 0x40000,
  FOS_NODEREFERENCELINKS = 0x100000,
  FOS_DONTADDTORECENT = 0x2000000,
  FOS_FORCESHOWHIDDEN = 0x10000000,
  FOS_DEFAULTNOMINIMODE = 0x20000000,
  FOS_FORCEPREVIEWPANEON = 0x40000000
};
typedef DWORD FILEOPENDIALOGOPTIONS;
typedef enum FDAP { FDAP_BOTTOM = 0, FDAP_TOP = 1 } FDAP;
DEFINE_GUID(IID_IFileDialog, 0x42f85136, 0xdb7e, 0x439c, 0x85, 0xf1, 0xe4, 0x07,
            0x5d, 0x13, 0x5f, 0xc8);
typedef struct IFileDialogVtbl {
  BEGIN_INTERFACE
  HRESULT(STDMETHODCALLTYPE *QueryInterface)
  (IFileDialog *This, REFIID riid, void **ppvObject);
  ULONG(STDMETHODCALLTYPE *AddRef)(IFileDialog *This);
  ULONG(STDMETHODCALLTYPE *Release)(IFileDialog *This);
  HRESULT(STDMETHODCALLTYPE *Show)(IFileDialog *This, HWND hwndOwner);
  HRESULT(STDMETHODCALLTYPE *SetFileTypes)
  (IFileDialog *This, UINT cFileTypes, const COMDLG_FILTERSPEC *rgFilterSpec);
  HRESULT(STDMETHODCALLTYPE *SetFileTypeIndex)
  (IFileDialog *This, UINT iFileType);
  HRESULT(STDMETHODCALLTYPE *GetFileTypeIndex)
  (IFileDialog *This, UINT *piFileType);
  HRESULT(STDMETHODCALLTYPE *Advise)
  (IFileDialog *This, IFileDialogEvents *pfde, DWORD *pdwCookie);
  HRESULT(STDMETHODCALLTYPE *Unadvise)(IFileDialog *This, DWORD dwCookie);
  HRESULT(STDMETHODCALLTYPE *SetOptions)
  (IFileDialog *This, FILEOPENDIALOGOPTIONS fos);
  HRESULT(STDMETHODCALLTYPE *GetOptions)
  (IFileDialog *This, FILEOPENDIALOGOPTIONS *pfos);
  HRESULT(STDMETHODCALLTYPE *SetDefaultFolder)
  (IFileDialog *This, IShellItem *psi);
  HRESULT(STDMETHODCALLTYPE *SetFolder)(IFileDialog *This, IShellItem *psi);
  HRESULT(STDMETHODCALLTYPE *GetFolder)(IFileDialog *This, IShellItem **ppsi);
  HRESULT(STDMETHODCALLTYPE *GetCurrentSelection)
  (IFileDialog *This, IShellItem **ppsi);
  HRESULT(STDMETHODCALLTYPE *SetFileName)(IFileDialog *This, LPCWSTR pszName);
  HRESULT(STDMETHODCALLTYPE *GetFileName)(IFileDialog *This, LPWSTR *pszName);
  HRESULT(STDMETHODCALLTYPE *SetTitle)(IFileDialog *This, LPCWSTR pszTitle);
  HRESULT(STDMETHODCALLTYPE *SetOkButtonLabel)
  (IFileDialog *This, LPCWSTR pszText);
  HRESULT(STDMETHODCALLTYPE *SetFileNameLabel)
  (IFileDialog *This, LPCWSTR pszLabel);
  HRESULT(STDMETHODCALLTYPE *GetResult)(IFileDialog *This, IShellItem **ppsi);
  HRESULT(STDMETHODCALLTYPE *AddPlace)
  (IFileDialog *This, IShellItem *psi, FDAP fdap);
  HRESULT(STDMETHODCALLTYPE *SetDefaultExtension)
  (IFileDialog *This, LPCWSTR pszDefaultExtension);
  HRESULT(STDMETHODCALLTYPE *Close)(IFileDialog *This, HRESULT hr);
  HRESULT(STDMETHODCALLTYPE *SetClientGuid)(IFileDialog *This, REFGUID guid);
  HRESULT(STDMETHODCALLTYPE *ClearClientData)(IFileDialog *This);
  HRESULT(STDMETHODCALLTYPE *SetFilter)
  (IFileDialog *This, IShellItemFilter *pFilter);
  END_INTERFACE
} IFileDialogVtbl;
interface IFileDialog {
  CONST_VTBL IFileDialogVtbl *lpVtbl;
};
DEFINE_GUID(IID_IFileOpenDialog, 0xd57c7288, 0xd4ad, 0x4768, 0xbe, 0x02, 0x9d,
            0x96, 0x95, 0x32, 0xd9, 0x60);
DEFINE_GUID(IID_IFileSaveDialog, 0x84bccd23, 0x5fde, 0x4cdb, 0xae, 0xa4, 0xaf,
            0x64, 0xb8, 0x3d, 0x78, 0xab);
#endif

static void webview_dialog(struct webview *w, enum webview_dialog_type dlgtype,
                           int flags, const char *title, const char *arg,
                           char *result, size_t resultsz) {
  if (dlgtype == WEBVIEW_DIALOG_TYPE_OPEN ||
      dlgtype == WEBVIEW_DIALOG_TYPE_SAVE) {
    IFileDialog *dlg = NULL;
    IShellItem *res = NULL;
    WCHAR *ws = NULL;
    char *s = NULL;
    FILEOPENDIALOGOPTIONS opts, add_opts;
    if (dlgtype == WEBVIEW_DIALOG_TYPE_OPEN) {
      if (CoCreateInstance(
              iid_unref(&CLSID_FileOpenDialog), NULL, CLSCTX_INPROC_SERVER,
              iid_unref(&IID_IFileOpenDialog), (void **)&dlg) != S_OK) {
        goto error_dlg;
      }
      add_opts |= FOS_NOCHANGEDIR | FOS_ALLNONSTORAGEITEMS | FOS_NOVALIDATE |
                  FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_SHAREAWARE |
                  FOS_NOTESTFILECREATE | FOS_NODEREFERENCELINKS |
                  FOS_FORCESHOWHIDDEN | FOS_DEFAULTNOMINIMODE;
    } else {
      if (CoCreateInstance(
              iid_unref(&CLSID_FileSaveDialog), NULL, CLSCTX_INPROC_SERVER,
              iid_unref(&IID_IFileSaveDialog), (void **)&dlg) != S_OK) {
        goto error_dlg;
      }
      add_opts |= FOS_OVERWRITEPROMPT | FOS_NOCHANGEDIR |
                  FOS_ALLNONSTORAGEITEMS | FOS_NOVALIDATE | FOS_SHAREAWARE |
                  FOS_NOTESTFILECREATE | FOS_NODEREFERENCELINKS |
                  FOS_FORCESHOWHIDDEN | FOS_DEFAULTNOMINIMODE;
    }
    if (dlg->lpVtbl->GetOptions(dlg, &opts) != S_OK) {
      goto error_dlg;
    }
    opts &= ~FOS_NOREADONLYRETURN;
    opts |= add_opts;
    if (dlg->lpVtbl->SetOptions(dlg, opts) != S_OK) {
      goto error_dlg;
    }
    if (dlg->lpVtbl->Show(dlg, w->priv.hwnd) != S_OK) {
      goto error_dlg;
    }
    if (dlg->lpVtbl->GetResult(dlg, &res) != S_OK) {
      goto error_dlg;
    }
    if (res->lpVtbl->GetDisplayName(res, SIGDN_FILESYSPATH, &ws) != S_OK) {
      goto error_result;
    }
    s = webview_from_utf16(ws);
    strncpy(result, s, resultsz);
    result[resultsz - 1] = '\0';
    CoTaskMemFree(ws);
  error_result:
    res->lpVtbl->Release(res);
  error_dlg:
    dlg->lpVtbl->Release(dlg);
    return;
  } else if (dlgtype == WEBVIEW_DIALOG_TYPE_ALERT) {
#if 0
    /* MinGW often doesn't contain TaskDialog, so we'll use MessageBox for now */
    WCHAR *wtitle = webview_to_utf16(title);
    WCHAR *warg = webview_to_utf16(arg);
    TaskDialog(w->priv.hwnd, NULL, NULL, wtitle, warg, 0, NULL, NULL);
    GlobalFree(warg);
    GlobalFree(wtitle);
#else
    MessageBox(w->priv.hwnd, arg, title, MB_OK);
#endif
  }
}

static void webview_terminate(struct webview *w) { PostQuitMessage(0); }
static void webview_exit(struct webview *w) { OleUninitialize(); }
static void webview_print_log(const char *s) { OutputDebugString(s); }

#endif /* WEBVIEW_WINAPI */

#if defined(WEBVIEW_COCOA)
#if (!defined MAC_OS_X_VERSION_10_12) ||                                       \
    MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_12
#define NSWindowStyleMaskResizable NSResizableWindowMask
#define NSWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
#define NSWindowStyleMaskTitled NSTitledWindowMask
#define NSWindowStyleMaskClosable NSClosableWindowMask
#define NSEventMaskAny NSAnyEventMask
#define NSEventModifierFlagCommand NSCommandKeyMask
#define NSEventModifierFlagOption NSAlternateKeyMask
#define NSAlertStyleInformational NSInformationalAlertStyle
#endif /* MAC_OS_X_VERSION_10_12 */
#if (!defined MAC_OS_X_VERSION_10_13) ||                                       \
    MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_13
#define NSModalResponseOK NSFileHandlingPanelOKButton
#endif /* MAC_OS_X_VERSION_10_12, MAC_OS_X_VERSION_10_13 */
static void webview_window_will_close(id self, SEL cmd, id notification) {
  struct webview *w =
      (struct webview *)objc_getAssociatedObject(self, "webview");
  webview_terminate(w);
}

static BOOL webview_is_selector_excluded_from_web_script(id self, SEL cmd,
                                                         SEL selector) {
  return selector != @selector(invoke:);
}

static void webview_did_clear_window_object(id self, SEL cmd, id webview,
                                            id script, id frame) {
  [script setValue:self forKey:@"external"];
}

static void webview_external_invoke(id self, SEL cmd, id arg) {
  struct webview *w =
      (struct webview *)objc_getAssociatedObject(self, "webview");
  if (w == NULL || w->external_invoke_cb == NULL) {
    return;
  }
  if ([arg isKindOfClass:[NSString class]] == NO) {
    return;
  }
  w->external_invoke_cb(w, [(NSString *)(arg)UTF8String]);
}

static int webview_init(struct webview *w) {
  w->priv.pool = [[NSAutoreleasePool alloc] init];
  [NSApplication sharedApplication];

  Class webViewDelegateClass =
      objc_allocateClassPair([NSObject class], "WebViewDelegate", 0);
  class_addMethod(webViewDelegateClass, sel_registerName("windowWillClose:"),
                  (IMP)webview_window_will_close, "v@:@");
  class_addMethod(object_getClass(webViewDelegateClass),
                  sel_registerName("isSelectorExcludedFromWebScript:"),
                  (IMP)webview_is_selector_excluded_from_web_script, "c@::");
  class_addMethod(webViewDelegateClass,
                  sel_registerName("webView:didClearWindowObject:forFrame:"),
                  (IMP)webview_did_clear_window_object, "v@:@@@");
  class_addMethod(webViewDelegateClass, sel_registerName("invoke:"),
                  (IMP)webview_external_invoke, "v@:@");
  objc_registerClassPair(webViewDelegateClass);

  w->priv.windowDelegate = [[webViewDelegateClass alloc] init];
  objc_setAssociatedObject(w->priv.windowDelegate, "webview", (id)(w),
                           OBJC_ASSOCIATION_ASSIGN);

  NSString *nsTitle = [NSString stringWithUTF8String:w->title];
  NSRect r = NSMakeRect(0, 0, w->width, w->height);
  NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
                     NSWindowStyleMaskMiniaturizable;
  if (w->resizable) {
    style = style | NSWindowStyleMaskResizable;
  }
  w->priv.window = [[NSWindow alloc] initWithContentRect:r
                                               styleMask:style
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];
  [w->priv.window autorelease];
  [w->priv.window setTitle:nsTitle];
  [w->priv.window setDelegate:w->priv.windowDelegate];
  [w->priv.window center];

  [[NSUserDefaults standardUserDefaults] setBool:!!w->debug
                                          forKey:@"WebKitDeveloperExtras"];
  [[NSUserDefaults standardUserDefaults] synchronize];
  w->priv.webview =
      [[WebView alloc] initWithFrame:r frameName:@"WebView" groupName:nil];
  NSURL *nsURL = [NSURL
      URLWithString:[NSString stringWithUTF8String:webview_check_url(w->url)]];
  [[w->priv.webview mainFrame] loadRequest:[NSURLRequest requestWithURL:nsURL]];

  [w->priv.webview setAutoresizesSubviews:YES];
  [w->priv.webview
      setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
  w->priv.webview.frameLoadDelegate = w->priv.windowDelegate;
  [[w->priv.window contentView] addSubview:w->priv.webview];
  [w->priv.window orderFrontRegardless];

  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
  [NSApp finishLaunching];
  [NSApp activateIgnoringOtherApps:YES];

  NSMenu *menubar = [[[NSMenu alloc] initWithTitle:@""] autorelease];

  NSString *appName = [[NSProcessInfo processInfo] processName];
  NSMenuItem *appMenuItem =
      [[[NSMenuItem alloc] initWithTitle:appName action:NULL keyEquivalent:@""]
          autorelease];
  NSMenu *appMenu = [[[NSMenu alloc] initWithTitle:appName] autorelease];
  [appMenuItem setSubmenu:appMenu];
  [menubar addItem:appMenuItem];

  NSString *title = [@"Hide " stringByAppendingString:appName];
  NSMenuItem *item = [[[NSMenuItem alloc] initWithTitle:title
                                                 action:@selector(hide:)
                                          keyEquivalent:@"h"] autorelease];
  [appMenu addItem:item];
  item = [[[NSMenuItem alloc] initWithTitle:@"Hide Others"
                                     action:@selector(hideOtherApplications:)
                              keyEquivalent:@"h"] autorelease];
  [item setKeyEquivalentModifierMask:(NSEventModifierFlagOption |
                                      NSEventModifierFlagCommand)];
  [appMenu addItem:item];
  item = [[[NSMenuItem alloc] initWithTitle:@"Show All"
                                     action:@selector(unhideAllApplications:)
                              keyEquivalent:@""] autorelease];
  [appMenu addItem:item];
  [appMenu addItem:[NSMenuItem separatorItem]];

  title = [@"Quit " stringByAppendingString:appName];
  item = [[[NSMenuItem alloc] initWithTitle:title
                                     action:@selector(terminate:)
                              keyEquivalent:@"q"] autorelease];
  [appMenu addItem:item];

  [NSApp setMainMenu:menubar];

  w->priv.should_exit = 0;
  return 0;
}

static int webview_loop(struct webview *w, int blocking) {
  NSDate *until = (blocking ? [NSDate distantFuture] : [NSDate distantPast]);
  NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                      untilDate:until
                                         inMode:NSDefaultRunLoopMode
                                        dequeue:YES];
  if (event) {
    [NSApp sendEvent:event];
  }
  return w->priv.should_exit;
}

static int webview_eval(struct webview *w, const char *js) {
  NSString *nsJS = [NSString stringWithUTF8String:js];
  [[w->priv.webview windowScriptObject] evaluateWebScript:nsJS];
  return 0;
}

static void webview_set_title(struct webview *w, const char *title) {
  NSString *nsTitle = [NSString stringWithUTF8String:title];
  [w->priv.window setTitle:nsTitle];
}

static void webview_dialog(struct webview *w, enum webview_dialog_type dlgtype,
                           int flags, const char *title, const char *arg,
                           char *result, size_t resultsz) {
  if (result != NULL) {
    result[0] = '\0';
  }
  if (dlgtype == WEBVIEW_DIALOG_TYPE_OPEN ||
      dlgtype == WEBVIEW_DIALOG_TYPE_SAVE) {
    NSSavePanel *panel;
    if (dlgtype == WEBVIEW_DIALOG_TYPE_OPEN) {
      NSOpenPanel *openPanel = [NSOpenPanel openPanel];
      [openPanel setCanChooseFiles:YES];
      [openPanel setCanChooseDirectories:NO];
      [openPanel setResolvesAliases:NO];
      [openPanel setAllowsMultipleSelection:NO];
      panel = openPanel;
    } else {
      panel = [NSSavePanel savePanel];
    }
    [panel setCanCreateDirectories:YES];
    [panel setShowsHiddenFiles:YES];
    [panel setExtensionHidden:NO];
    [panel setCanSelectHiddenExtension:NO];
    [panel setTreatsFilePackagesAsDirectories:YES];
    [panel beginSheetModalForWindow:w->priv.window
                  completionHandler:^(NSInteger result) {
                    [NSApp stopModalWithCode:result];
                  }];
    if ([NSApp runModalForWindow:panel] == NSModalResponseOK) {
      const char *filename = [[[panel URL] path] UTF8String];
      strlcpy(result, filename, resultsz);
    }
  } else if (dlgtype == WEBVIEW_DIALOG_TYPE_ALERT) {
    NSAlert *a = [NSAlert new];
    [a setAlertStyle:NSAlertStyleInformational];
    [a setShowsHelp:NO];
    [a setShowsSuppressionButton:NO];
    [a setMessageText:[NSString stringWithUTF8String:title]];
    [a setInformativeText:[NSString stringWithUTF8String:arg]];
    [a addButtonWithTitle:@"OK"];
    [a runModal];
    [a release];
  }
}

static void webview_dispatch_cb(void *arg) {
  struct webview_dispatch_arg *context = (struct webview_dispatch_arg *)arg;
  (context->fn)(context->w, context->arg);
  free(context);
}

static void webview_dispatch(struct webview *w, webview_dispatch_fn fn,
                             void *arg) {
  struct webview_dispatch_arg *context = (struct webview_dispatch_arg *)malloc(
      sizeof(struct webview_dispatch_arg));
  context->w = w;
  context->arg = arg;
  context->fn = fn;
  dispatch_async_f(dispatch_get_main_queue(), context, webview_dispatch_cb);
}

static void webview_terminate(struct webview *w) { w->priv.should_exit = 1; }
static void webview_exit(struct webview *w) { [NSApp terminate:NSApp]; }
static void webview_print_log(const char *s) { NSLog(@"%s", s); }

#endif /* WEBVIEW_COCOA */

#ifdef __cplusplus
}
#endif

#endif /* WEBVIEW_H */
