/**
 * @file borrow.h
 * @brief 借阅管理相关函数和数据结构的声明
 */

#ifndef BORROW_H
#define BORROW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "book.h"
#include "reader.h"

/**
 * @brief 借阅状态枚举
 */
typedef enum {
    BORROW_STATUS_BORROWED = 0,  /**< 已借出 */
    BORROW_STATUS_RETURNED = 1,   /**< 已归还 */
    BORROW_STATUS_OVERDUE = 2,    /**< 已逾期 */
    BORROW_STATUS_RENEWED = 3     /**< 已续借 */
} BorrowStatus;

/**
 * @brief 借阅记录结构体
 */
typedef struct {
    char id[20];              /**< 借阅记录ID */
    char book_id[20];         /**< 图书ID */
    char reader_id[20];       /**< 读者ID */
    time_t borrow_date;       /**< 借阅日期 */
    time_t due_date;          /**< 应还日期 */
    time_t return_date;       /**< 实际归还日期 */
    BorrowStatus status;      /**< 借阅状态 */
    int renew_count;          /**< 续借次数 */
} BorrowRecord;

/**
 * @brief 初始化借阅管理模块
 * @return 成功返回0，失败返回非0值
 */
int borrow_init();

/**
 * @brief 借阅图书
 * @param book_id 图书ID
 * @param reader_id 读者ID
 * @param record 用于存储借阅记录的结构体指针
 * @return 成功返回0，失败返回非0值
 */
int borrow_book(const char *book_id, const char *reader_id, BorrowRecord *record);

/**
 * @brief 归还图书
 * @param record_id 借阅记录ID
 * @return 成功返回0，失败返回非0值
 */
int return_book(const char *record_id);

/**
 * @brief 续借图书
 * @param record_id 借阅记录ID
 * @param new_due_date 新的应还日期
 * @return 成功返回0，失败返回非0值
 */
int renew_book(const char *record_id, time_t new_due_date);

/**
 * @brief 根据ID查找借阅记录
 * @param id 借阅记录ID
 * @param record 用于存储查找结果的借阅记录结构体指针
 * @return 成功返回0，失败返回非0值
 */
int borrow_find_by_id(const char *id, BorrowRecord *record);

/**
 * @brief 查找读者的借阅记录
 * @param reader_id 读者ID
 * @param records 用于存储查找结果的借阅记录结构体数组
 * @param max_count 最大返回数量
 * @return 返回找到的借阅记录数量
 */
int borrow_find_by_reader(const char *reader_id, BorrowRecord *records, int max_count);

/**
 * @brief 查找图书的借阅记录
 * @param book_id 图书ID
 * @param records 用于存储查找结果的借阅记录结构体数组
 * @param max_count 最大返回数量
 * @return 返回找到的借阅记录数量
 */
int borrow_find_by_book(const char *book_id, BorrowRecord *records, int max_count);

/**
 * @brief 获取所有借阅记录
 * @param records 用于存储借阅记录的结构体数组
 * @param max_count 最大返回数量
 * @return 返回借阅记录总数
 */
int borrow_get_all(BorrowRecord *records, int max_count);

/**
 * @brief 获取逾期的借阅记录
 * @param records 用于存储借阅记录的结构体数组
 * @param max_count 最大返回数量
 * @return 返回逾期借阅记录数量
 */
int borrow_get_overdue(BorrowRecord *records, int max_count);

/**
 * @brief 保存借阅数据到文件
 * @return 成功返回0，失败返回非0值
 */
int borrow_save_data();

/**
 * @brief 从文件加载借阅数据
 * @return 成功返回0，失败返回非0值
 */
int borrow_load_data();

/**
 * @brief 清理借阅管理模块资源
 */
void borrow_cleanup();

#endif /* BORROW_H */