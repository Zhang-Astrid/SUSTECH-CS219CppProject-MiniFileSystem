# MiniFileExplorer - 迷你文件管理器

一个基于终端的文件管理器，支持文件/目录的创建、删除、搜索、复制、移动等操作。

## 📁 项目结构

```
MiniFileSystem/
├── src/                      # 源代码目录
│   ├── main.cpp             # 程序入口
│   └── MiniFileExplorer.cpp # 主类实现
├── include/                  # 头文件目录
│   └── MiniFileExplorer.h   # 主类定义
├── Makefile                 # 编译脚本
└── README.md                # 本文件
```

## 🚀 快速开始

### 1. 编译项目

```bash
make
```

这会生成可执行文件 `MiniFileExplorer`

### 2. 运行程序

**方式一：使用当前目录**
```bash
./MiniFileExplorer
```

**方式二：指定初始目录**
```bash
./MiniFileExplorer /path/to/directory
```

### 3. 使用命令

程序启动后，会显示当前目录，然后等待你输入命令：

```
Current Directory: /home/user/project
Enter command (type 'help' for all commands): 
```

输入 `help` 查看所有可用命令。

## 📋 支持的命令

| 命令 | 说明 | 示例 |
|------|------|------|
| `cd [path]` | 切换目录 | `cd ../..` 或 `cd ~` |
| `ls [options]` | 列出文件 | `ls` 或 `ls -s` 或 `ls -t` |
| `touch [file]` | 创建文件 | `touch note.txt` |
| `mkdir [dir]` | 创建目录 | `mkdir data` |
| `rm [file]` | 删除文件 | `rm note.txt` |
| `rmdir [dir]` | 删除目录 | `rmdir data` |
| `stat [name]` | 文件信息 | `stat note.txt` |
| `search [keyword]` | 搜索文件 | `search note` |
| `cp [src] [dst]` | 复制文件 | `cp a.txt b.txt` |
| `mv [src] [dst]` | 移动文件 | `mv a.txt b.txt` |
| `du [dir]` | 目录大小 | `du data` |
| `help` | 显示帮助 | `help` |
| `exit` | 退出程序 | `exit` |

## 🛠️ 开发说明

### 架构设计

项目采用面向对象设计，核心类是 `MiniFileExplorer`：

- **主循环**: `run()` 方法持续接收用户输入
- **命令处理**: `handleCommand()` 解析命令并分发到对应的处理方法
- **命令实现**: 每个命令有独立的处理方法（如 `cmdCd()`, `cmdLs()` 等）

### 当前状态

✅ **已完成**:
- 项目结构搭建
- 类框架设计
- 命令解析框架
- help 和 exit 命令

🚧 **待实现**:
- 所有具体命令的功能实现（目前都是 TODO）

### 下一步开发

按照以下顺序实现功能：

1. **基础功能**（优先）
   - `cd` - 目录切换
   - `ls` - 文件列表
   - `touch` / `mkdir` - 创建操作
   - `rm` / `rmdir` - 删除操作
   - `stat` - 文件信息

2. **进阶功能**
   - `search` - 文件搜索
   - `cp` / `mv` - 复制移动
   - `du` - 目录大小
   - `ls -s` / `ls -t` - 排序功能

## 🔧 编译选项

如果需要修改编译选项，编辑 `Makefile`：

- `CXX`: 编译器（默认 g++）
- `CXXFLAGS`: 编译选项
  - `-std=c++17`: C++标准（必须，因为使用了 std::filesystem）
  - `-Wall -Wextra`: 警告选项
  - `-g`: 调试信息

## 📝 注意事项

1. **C++17 要求**: 项目使用了 `std::filesystem`，需要 C++17 或更高版本
2. **跨平台**: 代码使用标准库，理论上支持 Windows/Linux/Mac，但可能需要调整路径处理
3. **错误处理**: 每个命令实现时都要添加完善的错误检查和用户提示

## 🐛 清理编译文件

```bash
make clean
```

这会删除所有 `.o` 文件和可执行文件。

## 📚 参考

- [C++ Filesystem Library](https://en.cppreference.com/w/cpp/filesystem)
- 项目要求文档: `minifilesystem.md`

