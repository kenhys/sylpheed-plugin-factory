
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include "alertpanel.h"
#include "mainwindow.h"

#include "sylplugin_factory.h"

static FILE *log_file;
static gchar log_date_buf[64];

#if !GLIB_CHECK_VERSION(2, 32, 0)
static GStaticMutex log_mutex;
#else
static GMutex log_mutex;
#endif

void sylpf_log_handler(const gchar *log_domain,
                       GLogLevelFlags log_level,
                       const gchar *message,
                       gpointer user_data)
{
  time_t now;
  struct tm *local;

#if !GLIB_CHECK_VERSION(2, 32, 0)
#else
  g_mutex_lock(&log_mutex);
#endif
  if (log_file) {
    now = time(NULL);
    local = localtime(&now);
    strftime(log_date_buf, 64, "%Y-%m-%d %H:%M:%S", local);

    switch (log_level) {
    case G_LOG_LEVEL_WARNING:
      g_fprintf(log_file, "%s [%s][] %s\n",
                log_date_buf, log_domain, message);
      break;
    case G_LOG_LEVEL_DEBUG:
      g_fprintf(log_file, "%s [%s][DEBUG] %s\n",
                log_date_buf, log_domain, message);
      break;
    case G_LOG_LEVEL_ERROR:
      g_fprintf(log_file, "%s [%s][ERROR] %s\n",
                log_date_buf, log_domain, message);
      break;
    case G_LOG_LEVEL_INFO:
      g_fprintf(log_file, "%s [%s][INFO] %s\n",
                log_date_buf, log_domain, message);
      break;
    case G_LOG_LEVEL_CRITICAL:
      g_fprintf(log_file, "%s [%s][CRITICAL] %s\n",
                log_date_buf, log_domain, message);
      break;
    default:
      g_fprintf(log_file, "%s [%s][NORMAL] %s\n",
                log_date_buf, log_domain, message);
      break;
    }
    fflush(log_file);
  }
#if !GLIB_CHECK_VERSION(2, 32, 0)
#else
  g_mutex_unlock(&log_mutex);
#endif
}


guint sylpf_init_logger(const gchar *log_domain,
                        GLogLevelFlags log_levels,
                        GLogFunc log_func,
                        gpointer user_data)
{
  guint handler_id;
  gchar *log_path;

  log_path = g_strconcat(get_rc_dir(),
                         G_DIR_SEPARATOR_S,
                         log_domain,
                         ".log",
                         NULL);

#if !GLIB_CHECK_VERSION(2, 32, 0)
  log_mutex = G_STATIC_MUTEX_INIT;
#else
  g_mutex_init(&log_mutex);
#endif
  log_file = g_fopen(log_path, "a+");
  g_free(log_path);

  if (log_func) {
    handler_id = g_log_set_handler(log_domain,
                                   log_levels,
                                   log_func,
                                   user_data);
  } else {
    handler_id = g_log_set_handler(log_domain,
                                   log_levels,
                                   sylpf_log_handler,
                                   user_data);
  }
  return handler_id;
}



gint sylpf_get_rc_integer(const gchar *rcname,
                          const gchar *group,
                          const gchar *key)
{
  gint ivalue = 0;
  gchar *rcpath = NULL;
  GKeyFile *rcfile;

  rcpath = g_strconcat(get_rc_dir(),
                       G_DIR_SEPARATOR_S,
                       rcname,
                       NULL);
  rcfile = g_key_file_new();
  g_key_file_load_from_file(rcfile, rcpath, G_KEY_FILE_KEEP_COMMENTS, NULL);

  ivalue = SYLPF_GET_RC_INTEGER(rcfile, group, key);

  g_key_file_free(rcfile);
  g_free(rcpath);
  return ivalue;
}

gchar *sylpf_get_rc_string(const gchar *rcname,
                          const gchar *group,
                          const gchar *key)
{
  gchar *rcpath = NULL;
  gchar *value = NULL;
  GKeyFile *rcfile = NULL;

  rcpath = g_strconcat(get_rc_dir(),
                       G_DIR_SEPARATOR_S,
                       rcname,
                       NULL);
  rcfile = g_key_file_new();
  g_key_file_load_from_file(rcfile, rcpath, G_KEY_FILE_KEEP_COMMENTS, NULL);

  value = SYLPF_GET_RC_STRING(rcfile, group, key);

  g_key_file_free(rcfile);

  return value;
}

void sylpf_load_option_rcfile(SylPluginFactoryOption *option,
                              const gchar *rcname)
{
  option->rcpath = g_strconcat(get_rc_dir(), G_DIR_SEPARATOR_S, rcname, NULL);
  option->rcfile = g_key_file_new();

  g_key_file_load_from_file(option->rcfile, option->rcpath, G_KEY_FILE_KEEP_COMMENTS, NULL);
}

void sylpf_update_plugin_onoff_status(SylPluginFactoryOption *option,
                                      gboolean onoff,
                                      const char *title,
                                      const char *message,
                                      const char *tooltip)
{
  option->plugin_enabled = onoff;

  if (title && message) {
    syl_plugin_alertpanel_message(title, message, ALERT_NOTICE);
  }

  if (onoff != FALSE) {
    gtk_widget_hide(option->plugin_off);
    gtk_widget_show(option->plugin_on);
    gtk_tooltips_set_tip(option->plugin_tooltip,
                         option->plugin_switch,
                         tooltip,
                         NULL);
  } else {
    gtk_widget_hide(option->plugin_on);
    gtk_widget_show(option->plugin_off);
    gtk_tooltips_set_tip(option->plugin_tooltip,
                         option->plugin_switch,
                         tooltip,
                         NULL);
  }
}

void sypf_setup_plugin_onoff_switch(SylPluginFactoryOption *option,
                                    GCallback callback_func,
                                    const char **on_xpm,
                                    const char **off_xpm)
{
  GtkWidget *main_window = syl_plugin_main_window_get();
  GtkWidget *statusbar = syl_plugin_main_window_get_statusbar();
  GtkWidget *plugin_box = gtk_hbox_new(FALSE, 0);

  GdkPixbuf* on_pixbuf = gdk_pixbuf_new_from_xpm_data(on_xpm);
  option->plugin_on = gtk_image_new_from_pixbuf(on_pixbuf);

  GdkPixbuf* off_pixbuf = gdk_pixbuf_new_from_xpm_data(off_xpm);
  option->plugin_off = gtk_image_new_from_pixbuf(off_pixbuf);

  gtk_box_pack_start(GTK_BOX(plugin_box), option->plugin_on, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(plugin_box), option->plugin_off, FALSE, FALSE, 0);

  option->plugin_tooltip = gtk_tooltips_new();

  option->plugin_switch = gtk_button_new();
  gtk_button_set_relief(GTK_BUTTON(option->plugin_switch), GTK_RELIEF_NONE);
  GTK_WIDGET_UNSET_FLAGS(option->plugin_switch, GTK_CAN_FOCUS);
  gtk_widget_set_size_request(option->plugin_switch, 20, 20);

  gtk_container_add(GTK_CONTAINER(option->plugin_switch), plugin_box);
  g_signal_connect(G_OBJECT(option->plugin_switch), "clicked",
                   G_CALLBACK(callback_func), main_window);
  gtk_box_pack_start(GTK_BOX(statusbar), option->plugin_switch, FALSE, FALSE, 0);

  gtk_widget_show_all(option->plugin_switch);
  gtk_widget_hide(option->plugin_off);
}


void sylpf_init_gettext(const gchar *package,
                        const gchar *dir_name)
{
  GSList *modules = NULL;

#ifdef G_OS_WIN32
#else
  debug_print("[DEBUG] get_startup_dir:%s\n", get_startup_dir());
  modules = syl_plugin_get_module_list();
  if (g_path_is_absolute(dir_name)) {
  } else {
    syl_init_gettext(package, dir_name);
  }
#endif
}

GtkWidget *sylpf_pack_widget_with_aligned_frame(GtkWidget *widget,
                                                const gchar *frame_title)
{
  GtkWidget *align, *frame, *frame_align;
  align = gtk_alignment_new(0, 0, 1, 1);
  gtk_alignment_set_padding(GTK_ALIGNMENT(align),
                            SYLPF_ALIGN_TOP,
                            SYLPF_ALIGN_BOTTOM,
                            SYLPF_ALIGN_LEFT,
                            SYLPF_ALIGN_RIGHT);

  frame = gtk_frame_new(frame_title);
  frame_align = gtk_alignment_new(0, 0, 1, 1);
  gtk_alignment_set_padding(GTK_ALIGNMENT(frame_align),
                            SYLPF_ALIGN_TOP,
                            SYLPF_ALIGN_BOTTOM,
                            SYLPF_ALIGN_LEFT,
                            SYLPF_ALIGN_RIGHT);

  gtk_container_add(GTK_CONTAINER(frame_align), widget);
  gtk_container_add(GTK_CONTAINER(frame), frame_align);
  gtk_container_add(GTK_CONTAINER(align), frame);

  gtk_container_add(GTK_CONTAINER(frame_align), widget);
  gtk_widget_show(widget);

  return align;
}

void sylpf_pack_confirm_area(GtkWidget *parent,
                                   GCallback ok_cb,
                                   GCallback cancel_cb,
                                   gpointer data)
{
  GtkWidget *confirm_area;
  GtkWidget *ok_btn;
  GtkWidget *cancel_btn;

  confirm_area = gtk_hbutton_box_new();
  gtk_button_box_set_layout(GTK_BUTTON_BOX(confirm_area), GTK_BUTTONBOX_END);
  gtk_box_set_spacing(GTK_BOX(confirm_area), 6);

  ok_btn = gtk_button_new_from_stock(GTK_STOCK_OK);
  GTK_WIDGET_SET_FLAGS(ok_btn, GTK_CAN_DEFAULT);
  gtk_box_pack_start(GTK_BOX(confirm_area), ok_btn, FALSE, FALSE, 0);
  gtk_widget_show(ok_btn);

  cancel_btn = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
  GTK_WIDGET_SET_FLAGS(cancel_btn, GTK_CAN_DEFAULT);
  gtk_box_pack_start(GTK_BOX(confirm_area), cancel_btn, FALSE, FALSE, 0);
  gtk_widget_show(cancel_btn);

  gtk_widget_show(confirm_area);

  gtk_box_pack_end(GTK_BOX(parent), confirm_area, FALSE, FALSE, 0);
  gtk_widget_grab_default(ok_btn);

  g_signal_connect(G_OBJECT(ok_btn), "clicked",
                   G_CALLBACK(ok_cb), data);
  g_signal_connect(G_OBJECT(cancel_btn), "clicked",
                   G_CALLBACK(cancel_cb), data);
}

void sylpf_init_preference_dialog_size(GtkWidget *dialog)
{
  gpointer mainwin;
  GtkWidget *window;
  gint width, height;

  mainwin = syl_plugin_main_window_get();
  window = ((MainWindow*)mainwin)->window;
  
  gtk_window_get_size(GTK_WINDOW(window),
                      &width,
                      &height);

  height *= 0.8;
  if (width * 0.8 > 400) {
    width = 400;
  }

  gtk_widget_set_size_request(dialog,
                              width, height * 0.8);
}

gchar *sylpf_get_text_from_message_partial(MsgInfo *msginfo, ContentType content)
{
  gchar *buf;
  MimeInfo *mimeinfo, *partial;
  FILE *msgfile, *input;
  
  SYLPF_START_FUNC;

  mimeinfo = procmime_scan_message(msginfo);
  msgfile = procmsg_open_message(msginfo);

  buf = NULL;
  partial = mimeinfo;
  while (partial && partial->mime_type != MIME_TEXT) {
    partial = procmime_mimeinfo_next(partial);
  }
  if (partial && partial->mime_type == MIME_TEXT) {
    input = procmime_get_text_content(partial, msgfile, NULL);

    buf = calloc(partial->size+1, 1);

    fread(buf, partial->size, 1, input);
  }
  SYLPF_RETURN_VALUE(buf);
}

gchar *sylpf_format_diff2html_text(gchar const *text)
{
  gchar *buf;
  gchar **lines;
  gchar *line;
  
  SYLPF_START_FUNC;

  buf = "<html>\n"
    "<style type=\"text/css\">\n"
    "<!--\n"
    "div.yellow { background: #ffe; font-family: monospace; }\n"
    "div.red { background: #fed; font-family: monospace; }\n"
    "div.green { background: #efe; font-family: monospace; }\n"
    "div.snow { background: #f5f5fa; font-family: monospace; }\n"
    "pre {padding: 0; margin: 0;}\n"
    "-->\n"
    "</style>\n"
    "<body>\n"
    "<pre>\n";
  
  lines = g_strsplit(text, "\n", 1024);
  if (!lines) {
    return g_strconcat(buf, text, "</pre></body></html>", NULL);
  }

  while (line) {
    line = *lines;

    if (line) {
      SYLPF_DEBUG_STR("line", line);

      if (g_strstr_len(line, -1, "===")) {
        buf = sylpf_append_code_markup(buf, line, "yellow");
      } else if (g_strstr_len(line, -1, "---")) {
        buf = sylpf_append_code_markup(buf, line, "red");
      } else if (g_strstr_len(line, -1, "+++")) {
        buf = sylpf_append_code_markup(buf, line, "green");
      } else if (g_strstr_len(line, -1, "@@")) {
        buf = sylpf_append_code_markup(buf, line, "snow");
      } else if (g_strstr_len(line, 1, "-")) {
        buf = sylpf_append_code_markup(buf, line, "red");
      } else if (g_strstr_len(line, 1, "+")) {
        buf = sylpf_append_code_markup(buf, line, "green");
      } else {
        buf = sylpf_append_code_markup(buf, line, NULL);
      }
    }
    lines++;
  }
  buf = g_strconcat(buf, "</pre></body></html>", NULL);

  SYLPF_RETURN_VALUE(buf);
}

gchar *sylpf_append_code_markup(gchar *text,
                                gchar *piece,
                                const gchar *klass)
{
  gchar *markup;

  SYLPF_START_FUNC;

  if (klass) {
    markup = g_strdup_printf("%s<div class='%s'><pre>%s</pre></div>",
                             text,
                             klass,
                             piece);
  } else {
    markup = g_strdup_printf("%s<div><pre>%s</pre></div>",
                             text,
                             piece);
  }

  SYLPF_RETURN_VALUE(markup);
}


