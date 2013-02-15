update-po:
	xgettext src/*.[ch] -k_ -kN_ -o po/$(PLUGIN_NAME).pot
	if [ ! -f po/ja.po ]; then \
		if [ -f po/$(PLUGIN_NAME).pot ]; then \
			cp po/$(PLUGIN_NAME).pot po/ja.po; \
		fi \
	else \
		msgmerge po/ja.po po/$(PLUGIN_NAME).pot -o po/ja.po; \
		msgfmt po/ja.po -o po/ja.mo; \
	fi

copy:
	nm -u src/.libs/*.so | \
		awk '{print $$2}' | \
		egrep -v '^g_|^gtk_|^webkit|^syl_|^procmime_|^procmsg_|.*@@GLIBC_'
	if [ -f $(PLUGIN_SO) ]; then \
		cp -f src/.libs/*.so ~/.sylpheed-2.0/plugins; \
	fi

copying:
	echo "const gchar *copyright =\"" > src/COPYING.h
	cat COPYING >> src/COPYING.h
	echo "\";" >> src/COPYING.h
	sed -i 's/"AS IS"/\\"AS IS\\"/g' src/COPYING.h
