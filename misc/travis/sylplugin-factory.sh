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

run git clone https://github.com/kenhys/sylpheed-plugin-factory.git
SYLPHEED_BUILD_DIR=`pwd`/sylpheed
(cd sylpheed-plugin-factory && ./autogen.sh && ./configure --prefix=/usr --with-sylpheed-build-dir=$SYLPHEED_BUILD_DIR && make && sudo make install)
