/**
 * @file book.h
 * @brief 图书管理相关函数和数据结构的声明
 */

#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 图书结构体
 */
typedef struct {
    char id[20];          /**< 图书ID */
    char title[100];      /**< 图书标题 */
    char author[50];      /**< 作者 */
    char publisher[50];   /**< 出版社 */
    char isbn[20];        /**< ISBN */
    int publish_year;     /**< 出版年份 */
    int total_count;      /**< 总数量 */
    int available_count;  /**< 可借数量 */
} Book;

/**
 * @brief 初始化图书管理模块
 * @return 成功返回0，失败返回非0值
 */
int book_init();

/**
 * @brief 添加新图书
 * @param book 要添加的图书信息
 * @return 成功返回0，失败返回非0值
 */
int book_add(Book *book);

/**
 * @brief 根据ID删除图书
 * @param id 图书ID
 * @return 成功返回0，失败返回非0值
 */
int book_delete(const char *id);

/**
 * @brief 更新图书信息
 * @param book 更新后的图书信息
 * @return 成功返回0，失败返回非0值
 */
int book_update(Book *book);

/**
 * @brief 根据ID查找图书
 * @param id 图书ID
 * @param book 用于存储查找结果的图书结构体指针
 * @return 成功返回0，失败返回非0值
 */
int book_find_by_id(const char *id, Book *book);

/**
 * @brief 根据标题查找图书
 * @param title 图书标题
 * @param books 用于存储查找结果的图书结构体数组
 * @param max_count 最大返回数量
 * @return 返回找到的图书数量
 */
int book_find_by_title(const char *title, Book *books, int max_count);

/**
 * @brief 获取所有图书
 * @param books 用于存储图书的结构体数组
 * @param max_count 最大返回数量
 * @return 返回图书总数
 */
int book_get_all(Book *books, int max_count);

/**
 * @brief 保存图书数据到文件
 * @return 成功返回0，失败返回非0值
 */
int book_save_data();

/**
 * @brief 从文件加载图书数据
 * @return 成功返回0，失败返回非0值
 */
int book_load_data();

/**
 * @brief 清理图书管理模块资源
 */
void book_cleanup();

#endif /* BOOK_H */