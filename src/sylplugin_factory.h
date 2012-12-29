#ifndef __SYLPF_UTILITY_H_INCLUDED__
#define __SYLPF_UTILITY_H_INCLUDED__

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

#define SYLPF_ALIGN_TOP 3
#define SYLPF_ALIGN_BOTTOM 3
#define SYLPF_ALIGN_LEFT 6
#define SYLPF_ALIGN_RIGHT 6
#define SYLPF_BOX_SPACE 6

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
  debug_print("[DEBUG][%s] %s:%ld (%08x)\n", SYLPF_ID, msg, val, val)

#define SYLPF_DEBUG_STR(msg, val) \
  debug_print("[DEBUG][%s] %s:%s\n", SYLPF_ID, msg, val)

#define SYLPF_DEBUG_PTR(msg, val) \
  debug_print("[DEBUG][%s] %s:%p\n", SYLPF_ID, msg, val)

#define SYLPF_START_FUNC \
  debug_print("[START][%s] %s called.\n", SYLPF_ID, __func__);

#define SYLPF_END_FUNC \
  debug_print("[END][%s] %s() called.\n", SYLPF_ID, __func__);
  
#define SYLPF_GET_RC_INTEGER(keyfile, group, key)              \
  g_key_file_get_integer((keyfile), (group), (key), NULL)

#define SYLPF_GET_RC_BOOLEAN(keyarg) \
  g_key_file_get_boolean(SYLPF_OPTION.rcfile, SYLPF_ID, keyarg, NULL)

#define SYLPF_SET_RC_BOOLEAN(key, value) \
  g_key_file_set_boolean(SYLPF_OPTION.rcfile, SYLPF_ID, key, value)

#define SYLPF_GET_RC_STRING(keyfile, group, key) \
  g_key_file_get_string((keyfile), (group), (key), NULL)

#define SYLPF_SET_RC_STRING(keyfile, group, key, value) \
  g_key_file_set_string((keyfile), (group), (key), (value))

#define SYLPF_GET_RC_SHOW_ATTACH_TAB \
  sylpf_get_rc_integer(SYLPF_SYLPHEEDRC, "Common", "show_attach_tab")

#define SYLPF_GET_RC_MESSAGE_FONT_NAME \
  sylpf_get_rc_string(SYLPF_SYLPHEEDRC, "Common", "message_font_name")

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

#endif
