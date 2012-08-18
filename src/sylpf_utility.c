
#include <glib.h>
#include <glib/gprintf.h>
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
