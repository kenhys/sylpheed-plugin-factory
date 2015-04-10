#!/bin/sh

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
sudo apt-get install -qq libgtkhtml3.14-dev
sudo apt-get install -qq intltool
sudo apt-get install -qq libgpgme11-dev

BASE_URL=http://sylpheed.sraoss.jp/sylpheed
if [ "$SYLPHEED_STAGE"  = "master" ]; then
    run svn checkout svn://sylpheed.sraoss.jp/sylpheed/trunk sylpheed
    (cd sylpheed && ./autogen.sh && ./configure --prefix=/usr && make && sudo make install)
else
    VERSION=
    case "$SYLPHEED_STAGE" in
	3.3)
	    VERSION=3.3.1
	    run wget $BASE_URL/v3.3/sylpheed-$VERSION.tar.bz2
	    ;;
	3.4)
	    VERSION=3.4.2
	    run wget $BASE_URL/v3.4/sylpheed-$VERSION.tar.bz2
	    ;;
	3.5|beta|*)
	    VERSION=3.5.0beta1
	    run wget $BASE_URL/v3.5beta/sylpheed-$VERSION.tar.bz2
	    ;;
    esac
    run tar ixf sylpheed-$VERSION.tar.bz2
    run ln -sf sylpheed-$VERSION sylpheed
    (cd sylpheed && ./configure --prefix=/usr && make && sudo make install)
fi

if [ "$USE_SYLFILTER" = "yes" ]; then
    wget http://sylpheed.sraoss.jp/sylfilter/src/sylfilter-0.8.tar.gz
    tar xf sylfilter-0.8.tar.gz
    (cd sylfilter-0.8 && ./configure --prefix=/usr && make && sudo make install)
fi
