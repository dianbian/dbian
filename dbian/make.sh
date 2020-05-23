#! /bin/bash

#shell 有点玄 中括号的空格不能省略
# -d 文件夹, -f 文件 取反 !
if [ -d './CMakeFiles' ]; then
    rm CMakeFiles -rf
fi
if [ -f './CMakeCache.txt' ]; then
    rm CMakeCache.txt
fi
if [ -f './cmake_install.cmake' ]; then
    rm cmake_install.cmake
fi
if [ -f './Makefile' ]; then
    rm Makefile
fi

if [ -f './serv' ]; then
	rm serv
fi
