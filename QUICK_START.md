# 🎯 快速开始指南 - 给80岁老奶奶的教程

## 第一步：看看项目长什么样

现在你的项目结构是这样的：

```
MiniFileSystem/
├── src/                      ← 源代码放在这里
│   ├── main.cpp             ← 程序从这里开始
│   └── MiniFileExplorer.cpp ← 所有功能的实现
├── include/                  ← 头文件放在这里
│   └── MiniFileExplorer.h   ← 类的定义（像说明书）
├── Makefile                 ← 编译脚本（告诉电脑怎么编译）
└── README.md                ← 说明文档
```

## 第二步：编译项目（把代码变成可执行程序）

打开终端（Terminal），进入项目目录，然后输入：

```bash
make
```

**这行命令做了什么？**
- 电脑会读取 `Makefile` 文件
- 把 `src/` 里的 `.cpp` 文件编译成 `.o` 文件
- 再把所有 `.o` 文件链接成一个可执行程序 `MiniFileExplorer`

**如果成功了，你会看到：**
```
Build successful! Run with: ./MiniFileExplorer
```

## 第三步：运行程序

输入：

```bash
./MiniFileExplorer
```

**你会看到：**
```
Current Directory: /你的当前目录路径
Enter command (type 'help' for all commands): 
```

现在可以输入命令了！试试输入 `help` 看看有哪些命令。

## 📚 代码是怎么工作的？

### 1. 程序从哪里开始？

**`src/main.cpp`** - 这是程序的入口，就像房子的前门：

```cpp
int main(int argc, char* argv[]) {
    // 创建一个文件管理器
    MiniFileExplorer explorer(...);
    
    // 让它开始工作
    explorer.run();
}
```

### 2. 核心类：MiniFileExplorer

**`include/MiniFileExplorer.h`** - 这是类的"说明书"，告诉你这个类有什么功能：

```cpp
class MiniFileExplorer {
    void run();              // 主循环
    void cmdCd(...);         // cd命令
    void cmdLs(...);         // ls命令
    // ... 其他命令
};
```

**`src/MiniFileExplorer.cpp`** - 这是类的"实现"，具体怎么做：

```cpp
void MiniFileExplorer::run() {
    while (true) {
        // 1. 显示提示符
        std::cout << "Enter command...";
        
        // 2. 读取用户输入
        std::getline(std::cin, line);
        
        // 3. 处理命令
        handleCommand(line);
    }
}
```

### 3. 命令是怎么处理的？

**流程：**
```
用户输入 "cd ../test"
    ↓
split() 函数分割成 {"cd", "../test"}
    ↓
handleCommand() 识别是 "cd" 命令
    ↓
调用 cmdCd({"../test"})
    ↓
执行目录切换操作
```

## 🛠️ 现在要做什么？

### 当前状态

✅ **已经完成：**
- 项目结构搭建好了
- 类框架设计好了
- 命令解析框架好了
- `help` 和 `exit` 命令可以用了

🚧 **需要实现：**
- 所有其他命令（cd, ls, touch等）目前只是占位符
- 每个命令的 `cmdXxx()` 方法里写着 "Not implemented yet"

### 下一步：实现第一个命令

**建议从 `cd` 命令开始：**

1. 打开 `src/MiniFileExplorer.cpp`
2. 找到 `cmdCd()` 方法
3. 把 `// TODO: 实现目录切换` 替换成真正的代码

**示例代码思路：**
```cpp
void MiniFileExplorer::cmdCd(const std::vector<std::string>& args) {
    // 1. 检查参数
    if (args.empty()) {
        std::cout << "Missing path: Please enter 'cd [path]'" << std::endl;
        return;
    }
    
    // 2. 处理特殊路径（如 ~）
    std::string path = args[0];
    if (path == "~") {
        path = std::getenv("HOME");  // Linux/Mac
        // 或 std::getenv("USERPROFILE");  // Windows
    }
    
    // 3. 构建完整路径
    std::filesystem::path newPath = currentPath / path;
    newPath = std::filesystem::canonical(newPath);  // 规范化路径
    
    // 4. 检查路径是否存在
    if (!std::filesystem::exists(newPath)) {
        std::cout << "Invalid directory: " << path << std::endl;
        return;
    }
    
    // 5. 检查是否是目录
    if (!std::filesystem::is_directory(newPath)) {
        std::cout << "Not a directory: " << path << std::endl;
        return;
    }
    
    // 6. 切换目录
    currentPath = newPath;
    std::cout << "Current Directory: " << currentPath.string() << std::endl;
}
```

## 💡 开发小贴士

1. **一次实现一个功能**：不要贪多，先把一个命令做好
2. **测试很重要**：每实现一个功能就测试一下
3. **看错误信息**：编译出错时，仔细看错误信息，通常能告诉你问题在哪
4. **查文档**：不知道某个函数怎么用时，查 [cppreference.com](https://en.cppreference.com)

## 🐛 常见问题

### Q: 编译出错 "filesystem: No such file or directory"
**A:** 你的编译器可能不支持 C++17，或者需要链接 filesystem 库：
```bash
# 在 Makefile 的链接行添加 -lstdc++fs (GCC) 或 -lc++fs (Clang)
```

### Q: Windows 下编译出错
**A:** Windows 可能需要：
- 使用 MinGW-w64 或 Visual Studio 2017+
- 确保支持 C++17

### Q: 程序运行后输入命令没反应
**A:** 检查 `handleCommand()` 是否正确识别了命令名，确保大小写匹配。

## 📖 学习资源

- **C++ Filesystem**: https://en.cppreference.com/w/cpp/filesystem
- **std::filesystem::path**: 路径操作的核心类
- **std::filesystem::directory_iterator**: 遍历目录

---

**记住：** 编程就像搭积木，先搭好框架，再一块一块添加功能。现在框架已经搭好了，接下来就是填充每个命令的具体实现！💪

