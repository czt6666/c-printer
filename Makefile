# 指定编译器
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra

# 目标可执行文件
TARGET := bin/pja

# 目标操作系统（Linux/macOS 或 Windows）
OS := $(shell uname)

# 需要链接的库（根据 OS 选择）
ifeq ($(OS), Linux)
    GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
    LDFLAGS := -lcups $(shell pkg-config --libs gtk+-3.0)
else ifeq ($(OS), Darwin)
    GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
    LDFLAGS := -lcups $(shell pkg-config --libs gtk+-3.0)
else
    GTK_CFLAGS :=
    LDFLAGS := -lwinspool -lcomdlg32
endif

# 源文件和头文件的目录
SRC_DIR := src
UTIL_DIR := src/util
INC_DIR := include

# 源文件列表
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(UTIL_DIR)/*.cpp)

# 目标文件（.o 文件）
OBJS := $(SRCS:.cpp=.o)

# 生成最终的可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# 生成每个 .o 文件（加上 GTK CFLAGS）
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(GTK_CFLAGS) -c $< -o $@

# 清理编译生成的文件
clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标（防止和文件名冲突）
.PHONY: clean
