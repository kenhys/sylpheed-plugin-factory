
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

  ivalue = GET_RC_INT(rcfile, group, key);

  g_key_file_free(rcfile);
  g_free(rcpath);
  return ivalue;
}
