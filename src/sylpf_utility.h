#define GET_RC_INT(keyfile, group, key) \
  g_key_file_get_integer((keyfile), (group), (key), NULL)
