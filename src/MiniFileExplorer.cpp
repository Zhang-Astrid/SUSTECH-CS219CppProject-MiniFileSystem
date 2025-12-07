#include "../include/MiniFileExplorer.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>  // for exit(), getenv
#include <fstream>  // for file operations
#include <chrono>   // for time conversion
#include <ctime>    // for time formatting

// 跨平台支持：Windows 和 Linux/Mac 使用不同的函数获取当前目录
#ifdef _WIN32
#include <direct.h>  // Windows: _getcwd
#include <windows.h> // Windows: MAX_PATH
#define getcwd_func _getcwd
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
#else

#include <unistd.h>  // Linux/Mac: getcwd
#include <limits.h>  // Linux/Mac: PATH_MAX
#include <sys/stat.h>  // Linux/Mac: stat() for file times

#define getcwd_func getcwd
// Linux 上 PATH_MAX 可能未定义，使用默认值
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#endif

// ========== 构造函数：启动初始化（10分）==========
// 这部分实现了项目要求的"启动初始化"功能：
// 1. 程序启动时默认加载当前工作目录（通过 getcwd() 函数获取）
// 2. 支持启动时通过命令行参数指定初始目录
// 3. 若指定目录不存在，提示错误并退出
MiniFileExplorer::MiniFileExplorer(const std::string &initialPath) {
    if (initialPath.empty()) {
        // ========== 要求1：默认加载当前工作目录 ==========
        // 如果没有指定初始路径，使用 getcwd() 获取当前工作目录

        // 使用 PATH_MAX 常量，自动适配不同操作系统的最大路径长度
        // Windows: MAX_PATH (260) 或更长
        // Linux: PATH_MAX (通常是 4096)
        // macOS: MAXPATHLEN (1024)
        const size_t buffer_size = PATH_MAX;
        char *buffer = new char[buffer_size];

        // 使用宏定义，自动适配 Windows 和 Linux/Mac
        char *cwd = getcwd_func(buffer, buffer_size);

        if (cwd != nullptr) {
            currentPath = std::filesystem::path(cwd);
            delete[] buffer;
        } else {
            // 如果 getcwd() 失败（可能是路径太长或其他原因），
            // 使用 std::filesystem::current_path() 作为备选方案
            delete[] buffer;
            currentPath = std::filesystem::current_path();
        }
    } else {
        // ========== 要求2：支持命令行参数指定初始目录 ==========
        // 使用指定的初始路径
        currentPath = std::filesystem::path(initialPath);

        // ========== 要求3：校验目录合法性 ==========
        // 检查路径是否存在
        if (!std::filesystem::exists(currentPath)) {
            std::cout << "Directory not found: " << initialPath << std::endl;
            exit(1);
        }

        // 检查是否是目录（而不是文件）
        if (!std::filesystem::is_directory(currentPath)) {
            std::cout << "Not a directory: " << initialPath << std::endl;
            exit(1);
        }
    }

    // ========== 显示当前目录路径（格式：Current Directory: /path/to/dir）==========
    std::cout << "Current Directory: " << currentPath.string() << std::endl;
}

// ========== 主循环 ==========
void MiniFileExplorer::run() {
    std::string line;

    while (true) {
        // 显示命令提示符
        std::cout << "Enter command (type 'help' for all commands): ";

        // 读取用户输入的一行命令
        if (!std::getline(std::cin, line)) {
            // 如果读取失败（比如Ctrl+D），退出
            break;
        }

        // 处理命令
        handleCommand(line);
    }
}

// ========== 命令处理 ==========
void MiniFileExplorer::handleCommand(const std::string &line) {
    // 分割命令和参数
    std::vector <std::string> args = split(line);

    // 如果没有输入任何内容，直接返回
    if (args.empty()) {
        return;
    }

    // 获取命令名（第一个参数）
    std::string command = args[0];

    // 移除命令名，剩下的就是参数
    args.erase(args.begin());

    // 根据命令名调用对应的处理方法
    if (command == "cd") {
        cmdCd(args);
    } else if (command == "ls") {
        cmdLs(args);
    } else if (command == "touch") {
        cmdTouch(args);
    } else if (command == "mkdir") {
        cmdMkdir(args);
    } else if (command == "rm") {
        cmdRm(args);
    } else if (command == "rmdir") {
        cmdRmdir(args);
    } else if (command == "stat") {
        cmdStat(args);
    } else if (command == "search") {
        cmdSearch(args);
    } else if (command == "cp") {
        cmdCp(args);
    } else if (command == "mv") {
        cmdMv(args);
    } else if (command == "du") {
        cmdDu(args);
    } else if (command == "help") {
        printHelp();
    } else if (command == "exit") {
        cmdExit();
    } else {
        // 未知命令
        std::cout << "Unknown command: " << command << std::endl;
        std::cout << "Type 'help' for all commands." << std::endl;
    }
}

// ========== 命令实现（暂时为空，后续填充） ==========

void MiniFileExplorer::cmdCd(const std::vector <std::string> &args) {
    // ========== 目录切换操作（15分）==========
    // 1. 支持输入 cd [目录路径] 切换目录
    // 2. 切换时校验目录合法性
    // 3. 支持输入 cd ~ 快速切换到当前用户的主目录

    // 检查参数
    if (args.empty()) {
        // 如果没有参数，可以切换到主目录（可选功能，项目要求未明确说明）
        // 这里我们提示需要参数
        std::cout << "Missing path: Please enter 'cd [path]'" << std::endl;
        return;
    }

    std::string targetPath = args[0];
    std::filesystem::path newPath;

    // ========== 要求3：支持 cd ~ 切换到主目录 ==========
    if (targetPath == "~") {
        // 获取主目录路径
        const char *homeDir = nullptr;

#ifdef _WIN32
        // Windows: 使用 USERPROFILE 环境变量
        homeDir = std::getenv("USERPROFILE");
        if (homeDir == nullptr) {
            // 备选：使用 HOMEDRIVE + HOMEPATH
            const char* drive = std::getenv("HOMEDRIVE");
            const char* path = std::getenv("HOMEPATH");
            if (drive != nullptr && path != nullptr) {
                std::string homePath = std::string(drive) + std::string(path);
                newPath = std::filesystem::path(homePath);
            } else {
                std::cout << "Cannot determine home directory" << std::endl;
                return;
            }
        } else {
            newPath = std::filesystem::path(homeDir);
        }
#else
        // Linux/Mac: 使用 HOME 环境变量
        homeDir = std::getenv("HOME");
        if (homeDir == nullptr) {
            std::cout << "Cannot determine home directory" << std::endl;
            return;
        }
        newPath = std::filesystem::path(homeDir);
#endif
    } else {
        // ========== 要求1：支持相对路径和绝对路径 ==========
        // 处理相对路径（如 ../.. 或 ./test）
        // 处理绝对路径（如 /home/user/docs）

        if (std::filesystem::path(targetPath).is_absolute()) {
            // 绝对路径：直接使用
            newPath = std::filesystem::path(targetPath);
        } else {
            // 相对路径：基于当前目录构建完整路径
            newPath = currentPath / targetPath;
        }

        // 规范化路径（处理 .. 和 . 等符号）
        // 先转换为绝对路径
        newPath = std::filesystem::absolute(newPath);

        // 如果路径存在，使用 canonical 进一步规范化（解析符号链接等）
        // 如果不存在，保持 absolute 的结果，稍后会检查并报错
        if (std::filesystem::exists(newPath)) {
            try {
                newPath = std::filesystem::canonical(newPath);
            } catch (const std::filesystem::filesystem_error &e) {
                // 如果 canonical 失败（比如权限问题），继续使用 absolute 的结果
            }
        }
    }

    // ========== 要求2：校验目录合法性 ==========

    // 检查路径是否存在
    if (!std::filesystem::exists(newPath)) {
        std::cout << "Invalid directory: " << targetPath << std::endl;
        return;
    }

    // 检查是否是目录（而不是文件）
    if (!std::filesystem::is_directory(newPath)) {
        std::cout << "Not a directory: " << targetPath << std::endl;
        return;
    }

    // ========== 切换目录 ==========
    // 更新当前路径
    currentPath = newPath;

    // 显示新的当前目录（保持与启动时一致的格式）
    std::cout << "Current Directory: " << currentPath.string() << std::endl;
}

void MiniFileExplorer::cmdLs(const std::vector <std::string> &args) {
    // ========== 目录内容列表展示：ls 命令（10分）==========
    // 输入 ls 时，以列表形式展示当前目录下的所有文件和文件夹
    // 区分显示类型（文件夹名后加/，如data/；文件名正常显示，如note.txt）
    // 列表需包含 "名称、类型、大小（字节）、修改时间" 4列，格式对齐
    // 支持选项：-s (按大小排序), -t (按时间排序)
    
    // 解析选项
    bool sortBySize = false;
    bool sortByTime = false;
    for (const auto& arg : args) {
        if (arg == "-s") {
            sortBySize = true;
        } else if (arg == "-t") {
            sortByTime = true;
        }
    }
    
    // 定义条目信息结构
    struct EntryInfo {
        std::string name;
        std::string type;
        std::string sizeStr;
        std::string modifyTime;
        uintmax_t fileSize;  // 用于排序
        std::filesystem::file_time_type modifyTimePoint;  // 用于排序
    };
    
    try {
        // 遍历当前目录下的所有文件和文件夹，收集信息
        std::vector<EntryInfo> entries;
        for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
            EntryInfo info;
            std::filesystem::path path = entry.path();
            info.name = path.filename().string();
            
            // 判断类型并设置名称显示
            if (std::filesystem::is_directory(entry.status())) {
                // 目录：名称后加 /
                info.name += "/";
                info.type = "Dir";
                info.sizeStr = "-";
                info.fileSize = 0;  // 目录大小设为0用于排序
            } else {
                // 文件：正常显示
                info.type = "File";
                // 获取文件大小
                try {
                    info.fileSize = std::filesystem::file_size(path);
                    info.sizeStr = std::to_string(info.fileSize);
                } catch (const std::filesystem::filesystem_error&) {
                    info.sizeStr = "-";
                    info.fileSize = 0;
                }
            }
            
            // 获取修改时间
            try {
                info.modifyTimePoint = std::filesystem::last_write_time(path);
                
                // 将文件系统时间转换为系统时间
                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    info.modifyTimePoint - std::filesystem::file_time_type::clock::now() + 
                    std::chrono::system_clock::now());
                
                // 转换为 time_t
                std::time_t time = std::chrono::system_clock::to_time_t(sctp);
                
                // 格式化为字符串：YYYY-MM-DD HH:MM:SS
                std::tm* timeinfo = std::localtime(&time);
                char buffer[32];
                std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
                info.modifyTime = std::string(buffer);
            } catch (const std::filesystem::filesystem_error&) {
                info.modifyTime = "-";
                // 设置一个默认时间点用于排序
                info.modifyTimePoint = std::filesystem::file_time_type::min();
            }
            
            entries.push_back(info);
        }
        
        // 如果没有条目，直接返回（不显示表头）
        if (entries.empty()) {
            return;
        }
        
        // 根据选项排序
        if (sortBySize) {
            // 按大小排序（降序）
            std::sort(entries.begin(), entries.end(), 
                [](const EntryInfo& a, const EntryInfo& b) {
                    return a.fileSize > b.fileSize;
                });
        } else if (sortByTime) {
            // 按时间排序（降序，最新的在前）
            std::sort(entries.begin(), entries.end(), 
                [](const EntryInfo& a, const EntryInfo& b) {
                    return a.modifyTimePoint > b.modifyTimePoint;
                });
        }
        // 如果没有指定排序选项，保持默认顺序（文件系统顺序）
        
        // 打印表头
        std::cout << std::left << std::setw(20) << "Name" 
                  << std::setw(10) << "Type" 
                  << std::setw(15) << "Size(B)" 
                  << "Modify Time" << std::endl;
        
        // 打印分隔线
        std::cout << std::string(20, '-') << " " 
                  << std::string(10, '-') << " " 
                  << std::string(15, '-') << " " 
                  << std::string(19, '-') << std::endl;
        
        // 遍历并打印每个条目
        for (const auto& info : entries) {
            std::cout << std::left << std::setw(20) << info.name
                      << std::setw(10) << info.type
                      << std::setw(15) << info.sizeStr
                      << info.modifyTime << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error reading directory: " << e.what() << std::endl;
    }
}

void MiniFileExplorer::cmdTouch(const std::vector<std::string>& args) {
    // ========== 文件/文件夹创建：touch 命令（10分）==========
    // 输入 touch [文件名] 创建空文件
    // 若文件已存在，提示 "File already exists: [文件名]"
    
    // 检查参数
    if (args.empty()) {
        std::cout << "Missing filename: Please enter 'touch [filename]'" << std::endl;
        return;
    }
    
    std::string filename = args[0];
    std::filesystem::path filePath;
    
    // 处理路径（相对路径或绝对路径）
    if (std::filesystem::path(filename).is_absolute()) {
        // 绝对路径：直接使用
        filePath = std::filesystem::path(filename);
    } else {
        // 相对路径：基于当前目录构建完整路径
        filePath = currentPath / filename;
    }
    
    // 规范化路径
    filePath = std::filesystem::absolute(filePath);
    
    // 检查文件是否已存在
    if (std::filesystem::exists(filePath)) {
        std::cout << "File already exists: " << filename << std::endl;
        return;
    }
    
    // 创建空文件
    // 使用 ofstream 创建文件，如果文件不存在会自动创建
    std::ofstream file(filePath);
    if (file.is_open()) {
        file.close();
        // 文件创建成功，不需要额外输出（符合 Unix touch 命令的行为）
    } else {
        std::cout << "Failed to create file: " << filename << std::endl;
    }
}

void MiniFileExplorer::cmdMkdir(const std::vector<std::string>& args) {
    // ========== 文件/文件夹创建：mkdir 命令（10分）==========
    // 输入 mkdir [文件夹名] 创建空文件夹
    // 若文件夹已存在，提示 "Directory already exists: [文件夹名]"
    
    // 检查参数
    if (args.empty()) {
        std::cout << "Missing directory name: Please enter 'mkdir [dirname]'" << std::endl;
        return;
    }
    
    std::string dirname = args[0];
    std::filesystem::path dirPath;
    
    // 处理路径（相对路径或绝对路径）
    if (std::filesystem::path(dirname).is_absolute()) {
        // 绝对路径：直接使用
        dirPath = std::filesystem::path(dirname);
    } else {
        // 相对路径：基于当前目录构建完整路径
        dirPath = currentPath / dirname;
    }
    
    // 规范化路径
    dirPath = std::filesystem::absolute(dirPath);
    
    // 检查目录是否已存在
    if (std::filesystem::exists(dirPath)) {
        std::cout << "Directory already exists: " << dirname << std::endl;
        return;
    }
    
    // 创建目录
    // create_directories 可以创建多级目录，create_directory 只能创建单级目录
    // 这里使用 create_directory，因为项目要求是创建"空文件夹"
    if (std::filesystem::create_directory(dirPath)) {
        // 目录创建成功，不需要额外输出（符合 Unix mkdir 命令的行为）
    } else {
        std::cout << "Failed to create directory: " << dirname << std::endl;
    }
}

void MiniFileExplorer::cmdRm(const std::vector <std::string> &args) {
    // ========== 文件删除：rm 命令 ==========
    // 输入 rm [文件名] 删除指定文件，删除前需二次确认
    
    // 检查参数
    if (args.empty()) {
        std::cout << "Missing filename: Please enter 'rm [filename]'" << std::endl;
        return;
    }
    
    std::string filename = args[0];
    std::filesystem::path filePath;
    
    // 处理路径（相对路径或绝对路径）
    if (std::filesystem::path(filename).is_absolute()) {
        // 绝对路径：直接使用
        filePath = std::filesystem::path(filename);
    } else {
        // 相对路径：基于当前目录构建完整路径
        filePath = currentPath / filename;
    }
    
    // 规范化路径
    filePath = std::filesystem::absolute(filePath);
    
    // 检查文件是否存在
    if (!std::filesystem::exists(filePath)) {
        std::cout << "File not found: " << filename << std::endl;
        return;
    }
    
    // 检查是否是文件（而不是目录）
    if (!std::filesystem::is_regular_file(filePath) && !std::filesystem::is_symlink(filePath)) {
        std::cout << "Not a file: " << filename << std::endl;
        return;
    }
    
    // ========== 二次确认 ==========
    std::cout << "Are you sure to delete " << filename << "? (y/n): ";
    std::string confirmation;
    if (!std::getline(std::cin, confirmation)) {
        // 如果读取失败，取消操作
        return;
    }
    
    // 仅输入 "y" 时执行删除，其他输入取消操作
    if (confirmation == "y") {
        // 删除文件
        if (std::filesystem::remove(filePath)) {
            // 删除成功，不需要额外输出（符合 Unix rm 命令的行为）
        } else {
            std::cout << "Failed to delete file: " << filename << std::endl;
        }
    } else {
        // 取消操作，不需要输出（符合 Unix rm 命令的行为）
    }
}

void MiniFileExplorer::cmdRmdir(const std::vector <std::string> &args) {
    // ========== 目录删除：rmdir 命令 ==========
    // 输入 rmdir [文件夹名] 删除指定空文件夹
    // 若文件夹非空，提示 "Directory not empty: [文件夹名]"
    // 若文件夹不存在，提示 "Directory not found: [文件夹名]"
    
    // 检查参数
    if (args.empty()) {
        std::cout << "Missing directory name: Please enter 'rmdir [dirname]'" << std::endl;
        return;
    }
    
    std::string dirname = args[0];
    std::filesystem::path dirPath;
    
    // 处理路径（相对路径或绝对路径）
    if (std::filesystem::path(dirname).is_absolute()) {
        // 绝对路径：直接使用
        dirPath = std::filesystem::path(dirname);
    } else {
        // 相对路径：基于当前目录构建完整路径
        dirPath = currentPath / dirname;
    }
    
    // 规范化路径
    dirPath = std::filesystem::absolute(dirPath);
    
    // 检查目录是否存在
    if (!std::filesystem::exists(dirPath)) {
        std::cout << "Directory not found: " << dirname << std::endl;
        return;
    }
    
    // 检查是否是目录（而不是文件）
    if (!std::filesystem::is_directory(dirPath)) {
        std::cout << "Not a directory: " << dirname << std::endl;
        return;
    }
    
    // 检查目录是否为空
    if (!std::filesystem::is_empty(dirPath)) {
        std::cout << "Directory not empty: " << dirname << std::endl;
        return;
    }
    
    // 删除空目录
    if (std::filesystem::remove(dirPath)) {
        // 删除成功，不需要额外输出（符合 Unix rmdir 命令的行为）
    } else {
        std::cout << "Failed to delete directory: " << dirname << std::endl;
    }
}

// 辅助函数：将文件系统时间转换为格式化的字符串
std::string formatFileTime(const std::filesystem::file_time_type& fileTime) {
    try {
        // 将文件系统时间转换为系统时间
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            fileTime - std::filesystem::file_time_type::clock::now() + 
            std::chrono::system_clock::now());
        
        // 转换为 time_t
        std::time_t time = std::chrono::system_clock::to_time_t(sctp);
        
        // 格式化为字符串：YYYY-MM-DD HH:MM:SS
        std::tm* timeinfo = std::localtime(&time);
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return std::string(buffer);
    } catch (...) {
        return "-";
    }
}

void MiniFileExplorer::cmdStat(const std::vector <std::string> &args) {
    // ========== 文件信息查询：stat 命令（15分）==========
    // 输入 stat [文件名/文件夹名] 时，显示目标的详细信息：
    // 类型（文件 / 文件夹）、路径、大小（文件为字节数，文件夹为 "-"）、
    // 创建时间、修改时间、访问时间
    // 若目标不存在，提示 "Target not found: [目标名]"
    // 若输入时未指定目标（仅输入stat），提示 "Missing target: Please enter'stat [name]'"
    
    // 检查参数
    if (args.empty()) {
        std::cout << "Missing target: Please enter 'stat [name]'" << std::endl;
        return;
    }
    
    std::string targetName = args[0];
    std::filesystem::path targetPath;
    
    // 处理路径（相对路径或绝对路径）
    if (std::filesystem::path(targetName).is_absolute()) {
        // 绝对路径：直接使用
        targetPath = std::filesystem::path(targetName);
    } else {
        // 相对路径：基于当前目录构建完整路径
        targetPath = currentPath / targetName;
    }
    
    // 规范化路径
    targetPath = std::filesystem::absolute(targetPath);
    
    // 检查目标是否存在
    if (!std::filesystem::exists(targetPath)) {
        std::cout << "Target not found: " << targetName << std::endl;
        return;
    }
    
    // 获取目标信息
    std::string type;
    std::string sizeStr;
    std::string createTime = "-";
    std::string modifyTime = "-";
    std::string accessTime = "-";
    
    // 判断类型
    if (std::filesystem::is_directory(targetPath)) {
        type = "文件夹";
        sizeStr = "-";
    } else {
        type = "文件";
        // 获取文件大小
        try {
            auto fileSize = std::filesystem::file_size(targetPath);
            sizeStr = std::to_string(fileSize);
        } catch (const std::filesystem::filesystem_error&) {
            sizeStr = "-";
        }
    }
    
    // 获取修改时间
    try {
        auto modifyTimePoint = std::filesystem::last_write_time(targetPath);
        modifyTime = formatFileTime(modifyTimePoint);
    } catch (const std::filesystem::filesystem_error&) {
        modifyTime = "-";
    }
    
    // 获取创建时间和访问时间
    // 注意：C++17 的 std::filesystem 不直接支持创建时间和访问时间
    // 在 Windows 上可以尝试通过其他方式获取，但为了跨平台兼容性，这里显示 "-"
    // 或者可以使用修改时间作为创建时间的近似值（某些文件系统上创建时间不可用）
    #ifdef _WIN32
    // Windows 上可以尝试获取创建时间
    try {
        WIN32_FILE_ATTRIBUTE_DATA fileInfo;
        if (GetFileAttributesExA(targetPath.string().c_str(), GetFileExInfoStandard, &fileInfo)) {
            // 转换 Windows FILETIME 为系统时间
            ULARGE_INTEGER ul;
            ul.LowPart = fileInfo.ftCreationTime.dwLowDateTime;
            ul.HighPart = fileInfo.ftCreationTime.dwHighDateTime;
            
            std::time_t createTime_t = (ul.QuadPart / 10000000ULL) - 11644473600ULL;
            std::tm* timeinfo = std::localtime(&createTime_t);
            char buffer[32];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
            createTime = std::string(buffer);
            
            // 获取访问时间
            ul.LowPart = fileInfo.ftLastAccessTime.dwLowDateTime;
            ul.HighPart = fileInfo.ftLastAccessTime.dwHighDateTime;
            std::time_t accessTime_t = (ul.QuadPart / 10000000ULL) - 11644473600ULL;
            timeinfo = std::localtime(&accessTime_t);
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
            accessTime = std::string(buffer);
        }
    } catch (...) {
        // 如果获取失败，保持默认值 "-"
    }
    #else
    // Linux/Mac 上，使用 stat() 系统调用获取访问时间
    try {
        struct stat fileStat;
        if (stat(targetPath.string().c_str(), &fileStat) == 0) {
            // 获取访问时间
            std::time_t accessTime_t = fileStat.st_atime;
            std::tm* timeinfo = std::localtime(&accessTime_t);
            char buffer[32];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
            accessTime = std::string(buffer);
            
            // 在某些文件系统上，创建时间（birth time）可能可用
            // 但为了兼容性，这里不尝试获取，保持 "-"
            // 如果需要，可以使用 statx() 系统调用（Linux 4.11+）
        }
    } catch (...) {
        // 如果获取失败，保持默认值 "-"
    }
    #endif
    
    // 显示详细信息
    std::cout << "\n=== File/Directory Information ===" << std::endl;
    std::cout << "Type:        " << type << std::endl;
    std::cout << "Path:        " << targetPath.string() << std::endl;
    std::cout << "Size:        " << sizeStr << (type == "文件" ? " bytes" : "") << std::endl;
    std::cout << "Create Time: " << createTime << std::endl;
    std::cout << "Modify Time: " << modifyTime << std::endl;
    std::cout << "Access Time: " << accessTime << std::endl;
    std::cout << std::endl;
}

void MiniFileExplorer::cmdSearch(const std::vector <std::string> &args) {
    // TODO: 实现文件搜索
    std::cout << "search command - Not implemented yet" << std::endl;
}

void MiniFileExplorer::cmdCp(const std::vector <std::string> &args) {
    // TODO: 实现文件复制
    std::cout << "cp command - Not implemented yet" << std::endl;
}

void MiniFileExplorer::cmdMv(const std::vector <std::string> &args) {
    // TODO: 实现文件移动
    std::cout << "mv command - Not implemented yet" << std::endl;
}

void MiniFileExplorer::cmdDu(const std::vector <std::string> &args) {
    // TODO: 实现目录大小计算
    std::cout << "du command - Not implemented yet" << std::endl;
}

void MiniFileExplorer::printHelp() {
    std::cout << "\n=== MiniFileExplorer Commands ===\n" << std::endl;
    std::cout << "cd [path]          - Switch to target directory" << std::endl;
    std::cout << "ls [options]       - List all files and directories" << std::endl;
    std::cout << "                   - Options: -s (sort by size), -t (sort by time)" << std::endl;
    std::cout << "touch [filename]   - Create an empty file" << std::endl;
    std::cout << "mkdir [dirname]    - Create a directory" << std::endl;
    std::cout << "rm [filename]      - Delete a file" << std::endl;
    std::cout << "rmdir [dirname]    - Delete an empty directory" << std::endl;
    std::cout << "stat [name]        - Show detailed information" << std::endl;
    std::cout << "search [keyword]   - Search files/directories" << std::endl;
    std::cout << "cp [src] [dst]     - Copy a file" << std::endl;
    std::cout << "mv [src] [dst]     - Move/rename a file or directory" << std::endl;
    std::cout << "du [dirname]       - Calculate directory size" << std::endl;
    std::cout << "help               - Show this help message" << std::endl;
    std::cout << "exit               - Exit the program" << std::endl;
    std::cout << std::endl;
}

void MiniFileExplorer::cmdExit() {
    std::cout << "MiniFileExplorer closed successfully" << std::endl;
    exit(0);
}

// ========== 工具函数 ==========
std::vector <std::string> split(const std::string &line) {
    std::vector <std::string> result;
    std::istringstream iss(line);
    std::string word;

    // 使用字符串流按空格分割
    while (iss >> word) {
        result.push_back(word);
    }

    return result;
}

