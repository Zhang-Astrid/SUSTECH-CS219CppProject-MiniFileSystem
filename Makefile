# MiniFileExplorer Makefile
# 
# 使用方法:
#   make        - 编译项目
#   make clean  - 清理编译文件
#   make run    - 编译并运行

# 编译器
CXX = g++

# 编译选项
# -std=c++17: 使用C++17标准（支持std::filesystem）
# -Wall: 显示所有警告
# -Wextra: 显示额外警告
# -g: 包含调试信息
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# 目标可执行文件名
TARGET = MiniFileExplorer

# 源文件目录
SRC_DIR = src
INCLUDE_DIR = include

# 所有源文件
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/MiniFileExplorer.cpp

# 所有目标文件（.o文件）
OBJECTS = $(SOURCES:.cpp=.o)

# 默认目标：编译整个项目
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Build successful! Run with: ./$(TARGET)"

# 编译每个源文件为目标文件
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/MiniFileExplorer.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# 清理编译生成的文件
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# 编译并运行
run: $(TARGET)
	./$(TARGET)

# 声明伪目标（不是实际文件）
.PHONY: all clean run

