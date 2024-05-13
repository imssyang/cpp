#!/bin/bash

_() { (set -exv; $@); [[ $? != 0 ]] && exit 1; }

config() {
  _ cmake -B build -S . -DCMAKE_INSTALL_PREFIX=local
}

compile() {
  _ cmake --build build
}

install() {
  _ cmake --install build --prefix out
  _ tree out
}

testo() {
  cd out/bin
  _ ./calc
  _ ./calc 9
  cd -
}

clean() {
  _ rm -rf build out
}

main() {
  config
  compile
  install
  testo
  clean
}

case "$1" in
  config) config ;;
  compile) compile ;;
  install) install ;;
  testo) testo ;;
  clean) clean ;;
  main) main ;;
  *) echo "Usage: ${0##*/} {config|compile|install|testo|clean|main}"
     exit 1
     ;;
esac

exit 0
