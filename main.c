/**
 * @file main.c
 * @brief 程序入口
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "book.h"
#include "reader.h"
#include "borrow.h"
#include "ui.h"
#include "utils.h"

/**
 * @brief 初始化应用程序
 * @return 成功返回0，失败返回非0值
 */
static int init_application() {
    // 创建数据目录
    if (create_directory("data") != 0) {
        fprintf(stderr, "Error: Failed to create data directory\n");
        return -1;
    }
    
    // 初始化各模块
    if (book_init() != 0) {
        fprintf(stderr, "Error: Failed to initialize book module\n");
        return -1;
    }
    
    if (reader_init() != 0) {
        fprintf(stderr, "Error: Failed to initialize reader module\n");
        book_cleanup();
        return -1;
    }
    
    if (borrow_init() != 0) {
        fprintf(stderr, "Error: Failed to initialize borrow module\n");
        reader_cleanup();
        book_cleanup();
        return -1;
    }
    
    return 0;
}

/**
 * @brief 清理应用程序资源
 */
static void cleanup_application() {
    borrow_cleanup();
    reader_cleanup();
    book_cleanup();
}

/**
 * @brief 程序入口
 * @param argc 命令行参数数量
 * @param argv 命令行参数
 * @return 程序退出码
 */
int main(int argc, char *argv[]) {
    // 设置本地化环境，支持中文显示
    setlocale(LC_ALL, "");
    
    // 初始化应用程序
    if (init_application() != 0) {
        return EXIT_FAILURE;
    }
    
    // 初始化UI
    if (ui_init(argc, argv) != 0) {
        fprintf(stderr, "Error: Failed to initialize UI\n");
        cleanup_application();
        return EXIT_FAILURE;
    }
    
    // 运行UI主循环
    ui_run();
    
    // 清理资源
    ui_cleanup();
    cleanup_application();
    
    return EXIT_SUCCESS;
}