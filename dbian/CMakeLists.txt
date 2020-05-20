# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (Serv)
# 指定生成目标 单个文件
add_executable(Serv main.cpp)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量  多个文件
#aux_source_directory(. DIR_SRCS)
#add_executable(Serv ${DIR_SRCS}) 
# 添加 math 子目录  多个目录的文件 该目录下也要cmakeList
# add_subdirectory(math)
# 指定生成目标
# add_executable(Demo main.cc)
# 添加链接库
# target_link_libraries(Demo MathFunctions)
# 子目
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
#aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库
#add_library (MathFunctions ${DIR_LIB_SRCS})

#添加系统关联库
link_libraries(pthread)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -Wno-unused-variable -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -pthread")
add_definitions(-std=c++11)