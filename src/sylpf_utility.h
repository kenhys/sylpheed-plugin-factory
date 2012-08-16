#ifndef __SYLPF_UTILITY_H_INCLUDED__
#define __SYLPF_UTILITY_H_INCLUDED__

#define SYLPF_SYLPHEEDRC "sylpheedrc"

#define GET_RC_INT(keyfile, group, key) \
  g_key_file_get_integer((keyfile), (group), (key), NULL)

#define SYLPF_GET_RC_BOOLEAN(keyarg) \
  g_key_file_get_boolean(SYLPF_OPTION.rcfile, SYLPF_ID, keyarg, NULL)

#define SYLPF_SET_RC_BOOLEAN(key, value) \
  g_key_file_set_boolean(SYLPF_OPTION.rcfile, SYLPF_ID, key, value)

gint sylpf_get_rc_integer(const gchar *rcname,
                          const gchar *group,
                          const gchar *key);

#endif
