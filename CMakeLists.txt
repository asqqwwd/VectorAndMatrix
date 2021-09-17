# 指定最小支持版本，如果太小，会出现找不到具体cmake指令的错误
cmake_minimum_required(VERSION 3.10)

project(MainSolution)

# 在src目录下寻找所有源文件，加入SRC变量
file(GLOB_RECURSE SRC src/*.cpp src/*.h src/*.hpp)  # 递归遍历文件夹

# 添加源码到可执行项目。有main入口的可执行文件
add_executable(VectorAndMatrix ${SRC})

message("***** "  ${PROJECT_NAME}  " ***** "  ${SRC}  " *****")

# cmake -G "Visual Studio 16 2019" -A x64 -S ./ -B "build"
# cmake --build build --config Release