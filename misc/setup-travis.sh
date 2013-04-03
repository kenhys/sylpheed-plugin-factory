

sudo apt-get update -qq
sudo apt-get install -qq libwebkitgtk-dev

case "$SYLPHEED_STAGE" in
    master|head)
	svn checkout svn://sylpheed.sraoss.jp/sylpheed/trunk sylpheed
	(cd sylpheed; ./autogen.sh; ./configure --prefix=/usr; make; sudo make install)
	;;
    3.4.0|3.4|beta)
	wget http://sylpheed.sraoss.jp/sylpheed/v3.4beta/sylpheed-3.4.0beta3.tar.bz2
	tar ixf sylpheed-3.4.0beta3.tar.bz2
	(cd sylpheed-3.4.0beta3; ./configure --prefix=/usr; make; sudo make install)
	ln -sf sylpheed-3.4.0beta3 sylpheed
	;;
    3.3.0|3.3|stable|*)
	wget http://sylpheed.sraoss.jp/sylpheed/v3.3/sylpheed-3.3.0.tar.bz2
	tar ixf sylpheed-3.3.0.tar.bz2
	(cd sylpheed-3.3.0; ./configure --prefix=/usr; make; sudo make install)
	ln -sf sylpheed-3.3.0 sylpheed
	;;
esac
