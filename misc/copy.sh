make 

nm .libs/*.so | grep 'U '|gawk '{print $2}' | egrep -v '^g_|^gtk_|^webkit|^syl_|^procmime_|^procmsg_|.*@@GLIBC_'
/bin/cp -f .libs/*.so ~/.sylpheed-2.0/plugins
