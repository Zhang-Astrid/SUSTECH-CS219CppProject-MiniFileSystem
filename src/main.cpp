#include <iostream>
#include <string>
#include "../include/MiniFileExplorer.h"

/**
 * 程序入口点
 * 
 * 用法:
 *   ./MiniFileExplorer              - 使用当前工作目录
 *   ./MiniFileExplorer /path/to/dir - 使用指定目录
 */
int main(int argc, char* argv[]) {
    // 创建文件管理器实例
    // 如果提供了命令行参数，使用指定的目录；否则使用当前工作目录
    MiniFileExplorer explorer(argc > 1 ? std::string(argv[1]) : std::string());
    
    // 运行主循环
    explorer.run();
    
    return 0;
}

