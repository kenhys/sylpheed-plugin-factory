
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include "alertpanel.h"

#include "sylpf_utility.h"

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

#if G_OS_WIN32
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
