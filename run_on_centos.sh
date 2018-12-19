#!/bin/sh

#docker run -it -v "${PWD}:/opt/src/" centos:5 sh -c "cd /opt/src/build-musl && pwd && ./apps/musl_test_main"
docker run -it -v "${PWD}:/opt/src/" centos:7 sh -c "cd /opt/src/build-musl && stat plugin/libmusl_test_plugin_plugin.so && cd apps && ./musl_test_main"
