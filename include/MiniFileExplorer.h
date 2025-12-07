#ifndef MINIFILEEXPLORER_H
#define MINIFILEEXPLORER_H

#include <string>
#include <vector>
#include <filesystem>

/**
 * MiniFileExplorer - 迷你文件管理器主类
 * 
 * 这个类包含了所有文件管理器的功能：
 * - 目录切换 (cd)
 * - 文件/目录列表 (ls)
 * - 文件/目录创建 (touch, mkdir)
 * - 文件/目录删除 (rm, rmdir)
 * - 文件信息查询 (stat)
 * - 文件搜索 (search)
 * - 文件复制/移动 (cp, mv)
 * - 目录大小计算 (du)
 */
class MiniFileExplorer {
public:
    /**
     * 构造函数
     * @param initialPath 初始路径，如果为空则使用当前工作目录
     */
    MiniFileExplorer(const std::string& initialPath = "");
    
    /**
     * 主循环 - 程序的核心，持续接收用户命令并执行
     */
    void run();

private:
    // 当前工作目录路径
    std::filesystem::path currentPath;

    /**
     * 处理用户输入的命令
     * @param line 用户输入的完整命令字符串
     */
    void handleCommand(const std::string& line);

    // ========== 命令处理方法 ==========
    
    /**
     * cd 命令 - 切换目录
     * 用法: cd [路径]
     * 示例: cd ../.. 或 cd ./test 或 cd ~
     */
    void cmdCd(const std::vector<std::string>& args);
    
    /**
     * ls 命令 - 列出当前目录内容
     * 用法: ls [选项]
     * 选项: -s (按大小排序), -t (按时间排序)
     */
    void cmdLs(const std::vector<std::string>& args);
    
    /**
     * touch 命令 - 创建空文件
     * 用法: touch [文件名]
     */
    void cmdTouch(const std::vector<std::string>& args);
    
    /**
     * mkdir 命令 - 创建目录
     * 用法: mkdir [目录名]
     */
    void cmdMkdir(const std::vector<std::string>& args);
    
    /**
     * rm 命令 - 删除文件
     * 用法: rm [文件名]
     */
    void cmdRm(const std::vector<std::string>& args);
    
    /**
     * rmdir 命令 - 删除空目录
     * 用法: rmdir [目录名]
     */
    void cmdRmdir(const std::vector<std::string>& args);
    
    /**
     * stat 命令 - 显示文件/目录详细信息
     * 用法: stat [文件名/目录名]
     */
    void cmdStat(const std::vector<std::string>& args);
    
    /**
     * search 命令 - 搜索文件/目录
     * 用法: search [关键词]
     */
    void cmdSearch(const std::vector<std::string>& args);
    
    /**
     * cp 命令 - 复制文件
     * 用法: cp [源文件] [目标路径]
     */
    void cmdCp(const std::vector<std::string>& args);
    
    /**
     * mv 命令 - 移动/重命名文件或目录
     * 用法: mv [源] [目标]
     */
    void cmdMv(const std::vector<std::string>& args);
    
    /**
     * du 命令 - 计算目录大小
     * 用法: du [目录名]
     */
    void cmdDu(const std::vector<std::string>& args);
    
    /**
     * help 命令 - 显示帮助信息
     */
    void printHelp();
    
    /**
     * exit 命令 - 退出程序
     */
    void cmdExit();
};

/**
 * 工具函数：将命令字符串分割成参数列表
 * 
 * 示例:
 *   split("cd ../test")  -> {"cd", "../test"}
 *   split("ls -s")       -> {"ls", "-s"}
 *   split("touch a.txt") -> {"touch", "a.txt"}
 * 
 * @param line 完整的命令字符串
 * @return 分割后的参数向量，第一个元素是命令名
 */
std::vector<std::string> split(const std::string& line);

#endif // MINIFILEEXPLORER_H

