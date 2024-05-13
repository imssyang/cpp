#!/bin/bash

_() { (set -exv; $@); [[ $? != 0 ]] && exit 1; }

config() {
  _ cmake -S . -B build -DCMAKE_INSTALL_PREFIX=local
}

config_sys() {
  _ cmake -S . -B build -DUSE_MYMATH=OFF
}

compile() {
  _ cmake --build build
}

install() {
  _ cmake --install build --prefix out
  _ tree out
}

testo() {
  LD_LIBRARY_PATH=out/lib ./out/bin/calc
  LD_LIBRARY_PATH=out/lib ./out/bin/calc 9
}

package() {
  cd build
  _ cpack -G ZIP -C Release
  _ cpack --config CPackSourceConfig.cmake
  cd -
}

clean() {
  _ rm -rf build out
}

main() {
  echo $@
  if [[ $1 == 'sys' ]]; then
    config_sys
  else
    config
  fi
  compile
  install
  testo
  package
  clean
}

case "$1" in
  config) shift; config $@ ;;
  compile) shift; compile $@ ;;
  install) shift; install $@ ;;
  testo) shift; testo $@ ;;
  package) shift; package $@ ;;
  clean) shift; clean $@ ;;
  main) shift; main $@ ;;
  *) echo "Usage: ${0##*/} {config|compile|install|testo|package|clean|main}"
     exit 1
     ;;
esac

exit 0
