

red=31
yellow=33
cyan=36

colored() {
  color=$1
  shift
  echo -e "\033[1;${color}m$@\033[0m"
}

run() {
  "$@"
  result=$?

  if [ $result -ne 0 ]
  then
    echo -n $(colored $red "Failed: ")
    echo -n $(colored $cyan "$@")
    echo $(colored $yellow " [$PWD]")
    exit $result
  fi

  return 0
}

sudo apt-get update -qq
sudo apt-get install -qq libwebkitgtk-dev libonig-dev

case "$SYLPHEED_STAGE" in
    master|head)
	run svn checkout svn://sylpheed.sraoss.jp/sylpheed/trunk sylpheed
	(cd sylpheed && ./autogen.sh && ./configure --prefix=/usr && make && sudo make install)
	;;
    3.4.0|3.4|beta)
	run wget http://sylpheed.sraoss.jp/sylpheed/v3.4beta/sylpheed-3.4.0beta3.tar.bz2
	run tar ixf sylpheed-3.4.0beta3.tar.bz2
	run ln -sf sylpheed-3.4.0beta3 sylpheed
	(cd sylpheed-3.4.0beta3; ./configure --prefix=/usr; make; sudo make install)
	;;
    3.3.0|3.3|stable|*)
	run wget http://sylpheed.sraoss.jp/sylpheed/v3.3/sylpheed-3.3.0.tar.bz2
	run tar ixf sylpheed-3.3.0.tar.bz2
	run ln -sf sylpheed-3.3.0 sylpheed
	(cd sylpheed-3.3.0 && ./configure --prefix=/usr && make && sudo make install)
	;;
esac

