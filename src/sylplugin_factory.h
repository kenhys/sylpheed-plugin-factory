#ifndef __SYLPF_UTILITY_H_INCLUDED__
#define __SYLPF_UTILITY_H_INCLUDED__

#define SYLPF_SYLPHEED_RC "sylpheedrc"
#define SYLPF_PLUGIN_RC "mypluginrc"

typedef struct  _SylPluginFactoryOption SylPluginFactoryOption;
struct _SylPluginFactoryOption {
  gchar *rcpath;
  GKeyFile *rcfile;

  gboolean plugin_enabled;

  GtkWidget *plugin_on;
  GtkWidget *plugin_off;
  GtkWidget *plugin_switch;
  GtkTooltips *plugin_tooltip;
};

typedef struct _SylpfGitCommitMailerInfo SylpfGitCommitMailerInfo;
struct _SylpfGitCommitMailerInfo {
  GList *modified;
  gint modified_index;
  gchar **lines;
  gint n_lines;
  gint line_no;
};

#define SYLPF_ALIGN_TOP 3
#define SYLPF_ALIGN_BOTTOM 3
#define SYLPF_ALIGN_LEFT 6
#define SYLPF_ALIGN_RIGHT 6
#define SYLPF_BOX_SPACE 6

#define SYLPF_OPTION_P (SylPluginFactoryOption*)&SYLPF_OPTION

#define SYLPF_DEBUG_FUNC(func) \
  debug_print("[DEBUG][%s] %s() called.\n", SYLPF_ID, __func__)

#define SYLPF_DEBUG_MSG(msg) \
  debug_print("[DEBUG][%s] %s\n", SYLPF_ID, msg)

#define SYLPF_WARN_MSG(msg) \
  g_print("\e[37;43m[WARN]\e[m[%s] %s\n", SYLPF_ID, msg)

#define SYLPF_INFO_MSG(msg) \
  g_print("\e[37;42m[INFO]\e[m[%s] %s\n", SYLPF_ID, msg)

#define SYLPF_ERR_MSG(msg) \
  g_print("\e[37;41m[ERROR]\e[m[%s] %s\n", SYLPF_ID, msg)

#define SYLPF_DEBUG_VAL(msg, val) \
  g_log(SYLPF_ID, G_LOG_LEVEL_DEBUG, "[%s] %s:%ld (%08x)", __func__, msg, (long int)val, (unsigned int)val)

#define SYLPF_DEBUG_STR(msg, val) \
  g_log(SYLPF_ID, G_LOG_LEVEL_DEBUG, "[%s] %s:%s", __func__, msg, val)

#define SYLPF_DEBUG_DSTR(msg, val) \
  g_log(SYLPF_ID, G_LOG_LEVEL_DEBUG, "[%s] %s:|%s|", __func__, msg, val)

#define SYLPF_DEBUG_PTR(msg, val) \
  g_log(SYLPF_ID, G_LOG_LEVEL_DEBUG, "[%s] %s:%p", __func__, msg, (void*)val)

#define SYLPF_START_FUNC \
  g_log(SYLPF_ID, G_LOG_LEVEL_DEBUG, "[START] %s called.", __func__);

#define SYLPF_END_FUNC \
  g_log(SYLPF_ID, G_LOG_LEVEL_DEBUG, "[END] %s called.", __func__);
  
#define SYLPF_RETURN \
  SYLPF_END_FUNC;

#define SYLPF_RETURN_VALUE(arg) \
  SYLPF_END_FUNC; return (arg);

#define SYLPF_GET_RC_INTEGER(keyfile, group, key)              \
  g_key_file_get_integer((keyfile), (group), (key), NULL)

#define SYLPF_GET_RC_BOOLEAN(keyarg) \
  g_key_file_get_boolean(SYLPF_OPTION.rcfile, SYLPF_ID, keyarg, NULL)

#define SYLPF_SET_RC_BOOLEAN(key, value) \
  g_key_file_set_boolean(SYLPF_OPTION.rcfile, SYLPF_ID, key, value)

#define SYLPF_GET_RC_STRING(keyfile, group, key) \
  g_key_file_get_string((keyfile), (group), (key), NULL)

#define SYLPF_GET_RC_STRING_LIST(keyfile, group, key, length, error) \
  g_key_file_get_string_list((keyfile), (group), (key), (length), (error))

#define SYLPF_SET_RC_STRING(keyfile, group, key, value) \
  g_key_file_set_string((keyfile), (group), (key), (value))

#define SYLPF_GET_RC_SHOW_ATTACH_TAB \
  sylpf_get_rc_integer(SYLPF_SYLPHEED_RC, "Common", "show_attach_tab")

#define SYLPF_GET_RC_MESSAGE_FONT_NAME \
  sylpf_get_rc_string(SYLPF_SYLPHEED_RC, "Common", "message_font_name")

#define SYLPF_BOX_PACKS(widget, child, expand, fill, space) \
  gtk_box_pack_start(GTK_BOX(widget), child, expand, fill, space)

#define SYLPF_BOX_PACKE(widget, child, expand, fill, space) \
  gtk_box_pack_end(GTK_BOX(widget), child, expand, fill, space)

guint sylpf_init_logger(const gchar *log_domain,
                        GLogLevelFlags log_levels,
                        GLogFunc log_func,
                        gpointer user_data);

gint sylpf_get_rc_integer(const gchar *rcname,
                          const gchar *group,
                          const gchar *key);

gchar *sylpf_get_rc_string(const gchar *rcname,
                           const gchar *group,
                           const gchar *key);

void sylpf_load_option_rcfile(SylPluginFactoryOption *option,
                              const gchar *rcname);

void sylpf_init_gettext(const gchar *package,
                        const gchar *dir_name);

GtkWidget *sylpf_pack_widget_with_aligned_frame(GtkWidget *widget,
                                          const gchar *frame_title);

void sylpf_pack_confirm_area(GtkWidget *parent,
                             GCallback ok_cb,
                             GCallback cancel_cb,
                             gpointer data);

gchar *sylpf_get_text_from_message_partial(MsgInfo *msginfo,
                                           ContentType content);

gchar *sylpf_format_diff2html_text(gchar const *text);

gchar *sylpf_append_code_markup(gchar *text,
                                gchar *piece,
                                const gchar *klass);

void sylpf_init_preference_dialog_size(GtkWidget *dialog);

void sylpf_update_folderview_visibility(gboolean visible);

gboolean sylpf_append_config_about_page(GtkWidget *notebook,
                                        GKeyFile *pkey,
                                        const gchar *widget_tab,
                                        const gchar *plugin_label,
                                        const gchar *plugin_desc,
                                        const gchar *copyright);

gchar *sylpf_format_gitcommitmailer_text(gchar *text);

gchar *sylpf_get_repo_name_from_msginfo(MsgInfo *msginfo);
gchar *sylpf_get_commit_hash_from_msginfo(MsgInfo *msginfo);
gchar *sylpf_search_matched_string(gchar *text, const gchar *pattern,
                                   gint ref, gchar *marker);
gboolean sylpf_save_option_rcfile(SylPluginFactoryOption *option);

void sylpf_setup_plugin_onoff_switch(SylPluginFactoryOption *option,
                                     GCallback callback_func,
                                     const char **on_xpm,
                                     const char **off_xpm);
void sylpf_update_plugin_onoff_status(SylPluginFactoryOption *option,
                                      gboolean onoff,
                                      const char *title,
                                      const char *message,
                                      const char *tooltip);

#endif
