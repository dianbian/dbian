#! /bin/bash

#shell 有点玄 中括号的空格不能省略
# -d 文件夹, -f 文件 取反 !
#非本机可执行命令 顾写脚本也可安装
if [ $1 ]; then
    if [ -f "$1" ]; then
        echo /home/bianzhenhua/go/bin/go run $1
        /home/bianzhenhua/go/bin/go run $1
    fi
fi
