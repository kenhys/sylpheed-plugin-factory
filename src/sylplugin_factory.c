
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include "alertpanel.h"
#include "mainwindow.h"
#include "plugin.h"
#include "sylmain.h"

#define PLUGIN_NAME "plugin_factory"
#define PLUGIN_DESC "plugin factory"

#include "sylplugin_factory.h"

static FILE *log_file;
static gchar log_date_buf[64];

#if !GLIB_CHECK_VERSION(2, 32, 0)
static GStaticMutex log_mutex;
#else
static GMutex log_mutex;
#endif

static gchar *get_gitcommitmailer_summary(gchar *text);
static gchar *extract_pair_text(gchar *text, gchar *key, gchar *marker);
static gchar *get_revision_from_commit_mail(gchar *text);
static gchar *get_author_from_commit_mail(gchar *text);
static gchar *get_date_from_commit_mail(gchar *text);
static gchar *get_gitcommitmailer_diff(gchar *text);

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
  GtkWidget *main_window;
  GtkWidget *statusbar;
  GtkWidget *plugin_box;
  GdkPixbuf* on_pixbuf;
  GdkPixbuf* off_pixbuf;

  main_window = syl_plugin_main_window_get();
  statusbar = syl_plugin_main_window_get_statusbar();
  plugin_box = gtk_hbox_new(FALSE, 0);

  on_pixbuf = gdk_pixbuf_new_from_xpm_data(on_xpm);
  option->plugin_on = gtk_image_new_from_pixbuf(on_pixbuf);

  off_pixbuf = gdk_pixbuf_new_from_xpm_data(off_xpm);
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
  size_t n_reads;

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

    n_reads = fread(buf, partial->size, 1, input);
  }
  SYLPF_RETURN_VALUE(buf);
}

gchar *sylpf_format_diff2html_text(gchar const *text)
{
  gchar *buf;
  gchar **lines;
  gchar *line;
  
  SYLPF_START_FUNC;

  buf = "<!DOCTYPE html>\n"
    "<head>"
    "<style type=\"text/css\">\n"
    "<!--\n"
    "div.yellow { background: #ffe; font-family: monospace; }\n"
    "div.red { background: #ffaaaa; font-family: monospace; }\n"
    "div.green { background: #aaffaa; font-family: monospace; }\n"
    "div.snow { background: #f5f5fa; font-family: monospace; }\n"
    "pre {padding: 0; margin: 0;}\n"
    "-->\n"
    "</style>\n"
    "</head>"
    "<body>\n"
    "<pre>\n";
  
  lines = g_strsplit(text, "\n", 1024);
  if (!lines) {
    return g_strconcat(buf, text, "</pre></body></html>", NULL);
  }

  line = *lines;

  while (line) {

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

    line = *lines;
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

static gchar *extract_pair_text(gchar *text, gchar *key, gchar *marker)
{
  gchar *item;

  SYLPF_START_FUNC;

  SYLPF_DEBUG_STR("key", key);
  SYLPF_DEBUG_STR("marker", marker);

  item = g_strstr_len(text, -1, key);
  g_return_val_if_fail(item != NULL, "N/A");

  item += strlen(key);

  marker = g_strstr_len(item, -1, marker);
  
  SYLPF_RETURN_VALUE(g_strndup(item, marker-item));
}

gchar *sylpf_format_gitcommitmailer_text(gchar *text)
{
  gchar *summary;
  gchar *body;
  gchar *html;
  
  SYLPF_START_FUNC;

  summary = get_gitcommitmailer_summary(text);
  body = get_gitcommitmailer_diff(text);
  html = g_strconcat(summary, body, NULL);
  g_free(summary);
  g_free(body);

  SYLPF_RETURN_VALUE(html);
}

static gchar *get_gitcommitmailer_summary(gchar *text)
{
  gchar *html;
  gchar *revision;
  gchar *author;
  gchar *date;
  gchar *message;
  gchar *files;
  gchar *dt_style;
  gchar *pre_style;
  
  SYLPF_START_FUNC;

  html = NULL;
  revision = get_revision_from_commit_mail(text);
  author = get_author_from_commit_mail(text);
  date = get_date_from_commit_mail(text);
  message = extract_pair_text(text, "Message:", "Modified files:");
  files = extract_pair_text(text, "Modified files:", "Modified: ");

  dt_style = "clear: both; float: left; font-weight: bold; width: 8em";

  pre_style = "border: 1px solid #aaa; "
    "font-family: Consolas, Menlo, &quot;Liberation Mono&quot;, Courier, monospace;"
    "line-height: 1.2; padding: 0.5em; width: auto";
  
  html = g_strdup_printf("<dl style=\"line-height: 1.5; margin-left: 2em\">"
                         "<dt style=\"%s\">Author</dt>"
                         "<dd style=\"margin-left: 8.5em\">%s &lt;%s&gt;</dd>"
                         "<dt style=\"%s\">Date</dt>"
                         "<dd style=\"margin-left: 8.5em\">%s</dd>"
                         "<dt style=\"%s\">New Revision</dt>"
                         "<dd style=\"margin-left: 8.5em\">%s</dd>"
                         "<dt style=\"%s\">Messages</dt>"
                         "<dd style=\"margin-left: 8.5em\">"
                         "<pre style=\"%s\">%s</pre></dd>"
                         "<dt style=\"%s\">Modified files</dt>"
                         "<dd style=\"margin-left: 8.5em\">%s</dd>",
                         dt_style, author, "<@example.com>",
                         dt_style, date,
                         dt_style, revision,
                         dt_style, pre_style, message,
                         dt_style, files);
  SYLPF_RETURN_VALUE(html);
}

static GList *get_modified_files_list(gchar **lines, gint *n_lines)
{
  GList *modified;

  modified = g_list_alloc();
  n_lines = 0;
  while (lines[n_lines]) {
    if (g_str_has_prefix(lines[n_lines], "  Modified:")) {
      if (lines[n_lines] != NULL &&
          lines[n_lines + 1] != NULL &&
          lines[n_lines + 2] != NULL &&
          lines[n_lines + 3] != NULL &&
          g_str_has_prefix(lines[n_lines], "===") &&
          g_str_has_prefix(lines[n_lines], "---") &&
          g_str_has_prefix(lines[n_lines], "+++") &&
          g_str_has_prefix(lines[n_lines], "@@ ")) {
        g_list_append(modified_list, GINT_TO_POINTER(n_lines));
      }
    }
    n_lines++;
  }
  return modified;
}

#define N_COLUMNS 3
static gchar *get_gitcommitmailer_diff(gchar *text)
{
  gchar *html = "<div class=\"diff-section\" style=\"clear: both\">";
  gchar *pattern_modified = "  Modified: (.+)\\s\\(\\+(\\d+)\\s-(\\d+)\\)";
  gchar *modified;
  gchar *match;
  gchar *added;
  gchar *deleted;
  gchar *thead;
  gchar *tbody;
  gchar **lines;
  gchar *line;
  gint index_src;
  gint index_dest;
  gint index;
  gint n_lines;
  gint n_modified;
  GList *modified_list;
  gpointer value;
#if GTK_CHECK_VERSION(2, 14, 0)
  GRegex *regex;
  GMatchInfo *match_info;
#else
#endif

  SYLPF_START_FUNC;

  match = NULL;
  modified = NULL;
  
  
  lines = g_strsplit(text, "\n", -1);
  
  n_modified = 0;
  n_lines = 0;
  index = 0;
  modified_list = get_modified_files_list(lines, &n_lines);
  
#if GTK_CHECK_VERSION(2, 14, 0)
  regex = g_regex_new(pattern_modified, 0, 0, NULL);
  g_regex_match(regex, text, 0, &match_info);
  while (g_match_info_matches(match_info)) {

    match = g_match_info_fetch(match_info, 1);
    added = g_match_info_fetch(match_info, 2);
    deleted = g_match_info_fetch(match_info, 3);
    g_print ("Found: %s\n", match);
    g_print ("Found: %s\n", added);
    g_print ("Found: %s\n", deleted);

    thead = g_strdup_printf("<thead>"
                           "<tr class=\"diff-header\" style=\"border: 1px solid #aaa\">"
                           "<td colspan=\"3\">"
                           "<pre style=\"border: 0; font-family: Consolas, Menlo, &quot;"
                           "Liberation Mono&quot;"
                           ", Courier, monospace; line-height: 1.2; margin: 0;"
                           " padding: 0.5em; white-space: normal; width: auto\">"
                           "<span class=\"diff-header\" style=\"background-color: #eaf2f5;"
                           "color: #999999; display: block; white-space: pre\">"
                           "Modified: %s (+%s -%s)</span>"
                           "<span class=\"diff-header-mark\" style=\"background-color: #eaf2f5;"
                           "color: #999999; display: block; white-space: pre\">"
                           "==================================================================="
                           "</span>"
                           "</pre>"
                           "</td>"
                           "</tr>"
                           "</thead>",
                           match, added, deleted);

    value = g_list_nth_data(modified_list, n_modified);
    index = GPOINTER_TO_INT(value);
    line = lines[index + 4];
    index_src = g_strtod(&line[4], NULL);
    index_dest = index_src;
    g_print ("Found: %d\n", index_src);

    while (index < n_lines) {
      if (n_modified < g_list_length(modified_list) - 1) {
        value = g_list_nth_data(modified_list, n_modified + 1);
        if (index < GPOINTER_TO_INT(value)) {
          line = lines[index];
          if (line[0] == '-') {
            index_src++;
          } else if (line[0] == '+') {
            index_dest++;
          } else {
          }
        }
      }
      index++;
    }
    tbody = g_strdup_printf("<tbody></tbody></table>");

    html = g_strdup_printf("%s<table style=\"border-collapse: collapse;"
                           "border: 1px solid #aaa\">"
                           "%s%s",
                           html, thead, tbody);

    n_modified++;
    g_match_info_next(match_info, NULL);
  }
  html = g_strdup_printf("%s</div>", html);

  g_match_info_free(match_info);
  g_regex_unref(regex);
  if (g_utf8_strlen(match, -1) == 0) {
    match = "N/A";
  }

  g_strfreev(lines);
#endif
  SYLPF_RETURN_VALUE(html);
}   

gchar *sylpf_search_matched_string(gchar *text, const gchar *pattern, gint ref, gchar *marker)
{
  gchar *match;
#if GTK_CHECK_VERSION(2, 14, 0)
  GRegex *regex;
  GMatchInfo *match_info;
#else
#endif

  match = NULL;

#if GTK_CHECK_VERSION(2, 14, 0)
  regex = g_regex_new(pattern, 0, 0, NULL);
  g_regex_match(regex, text, 0, &match_info);
  while (g_match_info_matches(match_info)) {
    match = g_match_info_fetch(match_info, ref);
    g_print ("Found: %s\n", match);
    g_match_info_next(match_info, NULL);
  }
  g_match_info_free(match_info);
  g_regex_unref(regex);
  if (g_utf8_strlen(match, -1) == 0) {
    match = "N/A";
  }
#endif
  SYLPF_RETURN_VALUE(match);
}

static gchar *get_revision_from_commit_mail(gchar *text)
{
  gchar *revision;
  const gchar *pattern = "New Revision: (.+)";
#if GTK_CHECK_VERSION(2, 14, 0)
#else
#endif

  SYLPF_START_FUNC;
  
#if GTK_CHECK_VERSION(2, 14, 0)
  revision = sylpf_search_matched_string(text, pattern, 1, NULL);
#else
  revision = extract_pair_text(text, "New Revision: ", "\n");
#endif
  
  SYLPF_RETURN_VALUE(revision);
}


static gchar *get_date_from_commit_mail(gchar *text)
{
  gchar *date;
  const gchar *pattern = "\\s(\\d+-\\d+-\\d+\\s\\d+:\\d+:\\d+\\s\\+\\d+\\s\\(\\w+,\\s\\d+\\s\\w+\\s\\d+\\))";

  SYLPF_START_FUNC;

#if GTK_CHECK_VERSION(2, 14, 0)
  date = sylpf_search_matched_string(text, pattern, 1, NULL);
#else
#endif
                                     
  SYLPF_RETURN_VALUE(date);
}

static gchar *get_author_from_commit_mail(gchar *text)
{
  const gchar *pattern = "(.+)\\s\\d+-\\d+-\\d+\\s\\d+:\\d+:\\d+\\s\\+\\d+\\s\\(\\w+,\\s\\d+\\s\\w+\\s\\d+\\)";
  gchar *author;
  
  SYLPF_START_FUNC;

#if GTK_CHECK_VERSION(2, 14, 0)
  author = sylpf_search_matched_string(text, pattern, 1, NULL);
#else
#endif
                                     
  SYLPF_RETURN_VALUE(author);
}


void sylpf_update_folderview_visibility(gboolean visible)
{
  gpointer mainwin;
  MainWindow *window;
  
  SYLPF_START_FUNC;

  mainwin = syl_plugin_main_window_get();
  g_return_if_fail(mainwin != NULL);

  window = (MainWindow*)mainwin;
  g_return_if_fail(window != NULL);

  if (visible) {
    gtk_widget_show(window->folderview->vbox);
  } else {
    gtk_widget_hide(window->folderview->vbox);
  }
  SYLPF_END_FUNC;
}

gboolean sylpf_append_config_about_page(GtkWidget *notebook,
                                        GKeyFile *pkey,
                                        const gchar *widget_tab,
                                        const gchar *plugin_label,
                                        const gchar *plugin_desc,
                                        const gchar *copyright)
{
  GtkWidget *hbox;
  GtkWidget *vbox;
  GtkWidget *label;
  GtkWidget *desc;
  GtkWidget *scrolled;
  GtkTextBuffer *tbuffer;
  GtkWidget *tview;

  SYLPF_START_FUNC;

  g_return_val_if_fail(notebook != NULL, FALSE);

  hbox = gtk_hbox_new(TRUE, SYLPF_BOX_SPACE);
  vbox = gtk_vbox_new(FALSE, SYLPF_BOX_SPACE);

  label = gtk_label_new(plugin_label);
  desc = gtk_label_new(plugin_desc);

  scrolled = gtk_scrolled_window_new(NULL, NULL);

  gtk_box_pack_start(GTK_BOX(vbox), label,
                     FALSE, TRUE, SYLPF_BOX_SPACE);
  gtk_box_pack_start(GTK_BOX(vbox), desc,
                     FALSE, TRUE, SYLPF_BOX_SPACE);
  gtk_box_pack_start(GTK_BOX(vbox), scrolled,
                     TRUE, TRUE, SYLPF_BOX_SPACE);
  gtk_box_pack_start(GTK_BOX(hbox), vbox,
                     TRUE, TRUE, SYLPF_BOX_SPACE);

  tbuffer = gtk_text_buffer_new(NULL);
  gtk_text_buffer_set_text(tbuffer, copyright, strlen(copyright));
  tview = gtk_text_view_new_with_buffer(tbuffer);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(tview), FALSE);
  gtk_container_add(GTK_CONTAINER(scrolled), tview);

  gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 6);

  label = gtk_label_new(widget_tab);
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
  gtk_widget_show_all(notebook);

  SYLPF_RETURN_VALUE(TRUE);
}

gchar *sylpf_get_repo_name_from_msginfo(MsgInfo *msginfo)
{
  gchar *text = sylpf_get_text_from_message_partial(msginfo, MIME_TEXT);
  return text;
}

gchar *sylpf_get_commit_hash_from_msginfo(MsgInfo *msginfo)
{
  gchar *text;
  gchar *revision, *marker;

  text = sylpf_get_text_from_message_partial(msginfo, MIME_TEXT);

  revision = g_strstr_len(text, -1, "New Revision: ");
  g_return_val_if_fail(revision != NULL, "N/A");

  revision += strlen("New Revision: ");

  marker = g_strstr_len(revision, -1, "\n");

  return g_strndup(revision, 7);
}

