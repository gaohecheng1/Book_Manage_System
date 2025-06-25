/**
 * @file reader.h
 * @brief 读者管理相关函数和数据结构的声明
 */

#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 读者结构体
 */
typedef struct {
    char id[20];          /**< 读者ID */
    char name[50];        /**< 读者姓名 */
    char gender[10];      /**< 性别 */
    char phone[20];       /**< 电话号码 */
    char email[50];       /**< 电子邮箱 */
    char address[100];    /**< 地址 */
    int max_borrow_count; /**< 最大借阅数量 */
    int current_borrow_count; /**< 当前借阅数量 */
} Reader;

/**
 * @brief 初始化读者管理模块
 * @return 成功返回0，失败返回非0值
 */
int reader_init();

/**
 * @brief 添加新读者
 * @param reader 要添加的读者信息
 * @return 成功返回0，失败返回非0值
 */
int reader_add(Reader *reader);

/**
 * @brief 根据ID删除读者
 * @param id 读者ID
 * @return 成功返回0，失败返回非0值
 */
int reader_delete(const char *id);

/**
 * @brief 更新读者信息
 * @param reader 更新后的读者信息
 * @return 成功返回0，失败返回非0值
 */
int reader_update(Reader *reader);

/**
 * @brief 根据ID查找读者
 * @param id 读者ID
 * @param reader 用于存储查找结果的读者结构体指针
 * @return 成功返回0，失败返回非0值
 */
int reader_find_by_id(const char *id, Reader *reader);

/**
 * @brief 根据姓名查找读者
 * @param name 读者姓名
 * @param readers 用于存储查找结果的读者结构体数组
 * @param max_count 最大返回数量
 * @return 返回找到的读者数量
 */
int reader_find_by_name(const char *name, Reader *readers, int max_count);

/**
 * @brief 获取所有读者
 * @param readers 用于存储读者的结构体数组
 * @param max_count 最大返回数量
 * @return 返回读者总数
 */
int reader_get_all(Reader *readers, int max_count);

/**
 * @brief 保存读者数据到文件
 * @return 成功返回0，失败返回非0值
 */
int reader_save_data();

/**
 * @brief 从文件加载读者数据
 * @return 成功返回0，失败返回非0值
 */
int reader_load_data();

/**
 * @brief 清理读者管理模块资源
 */
void reader_cleanup();

#endif /* READER_H */