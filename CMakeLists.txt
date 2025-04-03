cmake_minimum_required(VERSION 3.10)
project(c-printer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 GTK+3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# 设置源代码目录
set(SRC_DIR src)
set(UTIL_DIR src/util)
set(INC_DIR include)

# 查找所有 .cpp 文件
file(GLOB SRCS "${SRC_DIR}/*.cpp" "${UTIL_DIR}/*.cpp")

# 生成可执行文件
add_executable(pja ${SRCS})

# 设置头文件搜索路径
target_include_directories(pja PRIVATE ${INC_DIR} ${GTK3_INCLUDE_DIRS})

# 链接 GTK+ 和其他库
if(UNIX)
    target_link_libraries(pja PRIVATE ${GTK3_LIBRARIES} cups)
elseif(WIN32)
    target_link_libraries(pja PRIVATE winspool comdlg32)
endif
