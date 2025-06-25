/**
 * @file borrow.c
 * @brief 借阅管理相关函数的实现
 */

#include "borrow.h"
#include "book.h"
#include "reader.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BORROWS 5000
#define BORROWS_FILE "data/borrows.csv"
#define MAX_LINE_SIZE 1024
#define DEFAULT_BORROW_DAYS 30  // 默认借阅期限（天）
#define MAX_RENEW_COUNT 2      // 最大续借次数
#define RENEW_DAYS 15          // 续借延长天数

// 借阅记录数组
static BorrowRecord *borrows = NULL;
// 借阅记录数量
static int borrow_count = 0;
// 借阅记录容量
static int borrow_capacity = 0;

/**
 * @brief 初始化借阅管理模块
 * @return 成功返回0，失败返回非0值
 */
int borrow_init() {
    // 分配内存
    borrows = (BorrowRecord *)malloc(sizeof(BorrowRecord) * MAX_BORROWS);
    if (borrows == NULL) {
        return -1;
    }
    
    borrow_capacity = MAX_BORROWS;
    borrow_count = 0;
    
    // 加载数据
    return borrow_load_data();
}

/**
 * @brief 借阅图书
 * @param book_id 图书ID
 * @param reader_id 读者ID
 * @param record 用于存储借阅记录的结构体指针
 * @return 成功返回0，失败返回非0值
 */
int borrow_book(const char *book_id, const char *reader_id, BorrowRecord *record) {
    if (book_id == NULL || reader_id == NULL || record == NULL) {
        return -1;
    }
    
    // 检查容量
    if (borrow_count >= borrow_capacity) {
        return -1;
    }
    
    // 查找图书
    Book book;
    if (book_find_by_id(book_id, &book) != 0) {
        return -2; // 图书不存在
    }
    
    // 检查图书是否可借
    if (book.available_count <= 0) {
        return -3; // 图书已全部借出
    }
    
    // 查找读者
    Reader reader;
    if (reader_find_by_id(reader_id, &reader) != 0) {
        return -4; // 读者不存在
    }
    
    // 检查读者是否可借
    if (reader.current_borrow_count >= reader.max_borrow_count) {
        return -5; // 读者借阅数量已达上限
    }
    
    // 生成借阅记录
    memset(record, 0, sizeof(BorrowRecord));
    generate_id("BR", record->id, sizeof(record->id));
    strncpy(record->book_id, book_id, sizeof(record->book_id) - 1);
    record->book_id[sizeof(record->book_id) - 1] = '\0';
    strncpy(record->reader_id, reader_id, sizeof(record->reader_id) - 1);
    record->reader_id[sizeof(record->reader_id) - 1] = '\0';
    
    record->borrow_date = get_current_time();
    record->due_date = record->borrow_date + DEFAULT_BORROW_DAYS * 24 * 60 * 60; // 默认借阅期限
    record->return_date = 0;
    record->status = BORROW_STATUS_BORROWED;
    record->renew_count = 0;
    
    // 添加借阅记录
    memcpy(&borrows[borrow_count], record, sizeof(BorrowRecord));
    borrow_count++;
    
    // 更新图书可借数量
    book.available_count--;
    book_update(&book);
    
    // 更新读者当前借阅数量
    reader.current_borrow_count++;
    reader_update(&reader);
    
    // 保存数据
    return borrow_save_data();
}

/**
 * @brief 归还图书
 * @param record_id 借阅记录ID
 * @return 成功返回0，失败返回非0值
 */
int return_book(const char *record_id) {
    if (record_id == NULL) {
        return -1;
    }
    
    // 查找借阅记录
    int index = -1;
    for (int i = 0; i < borrow_count; i++) {
        if (strcmp(borrows[i].id, record_id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -2; // 借阅记录不存在
    }
    
    // 检查是否已归还
    if (borrows[index].status == BORROW_STATUS_RETURNED) {
        return -3; // 已归还
    }
    
    // 查找图书
    Book book;
    if (book_find_by_id(borrows[index].book_id, &book) != 0) {
        return -4; // 图书不存在
    }
    
    // 查找读者
    Reader reader;
    if (reader_find_by_id(borrows[index].reader_id, &reader) != 0) {
        return -5; // 读者不存在
    }
    
    // 更新借阅记录
    borrows[index].return_date = get_current_time();
    borrows[index].status = BORROW_STATUS_RETURNED;
    
    // 更新图书可借数量
    book.available_count++;
    book_update(&book);
    
    // 更新读者当前借阅数量
    if (reader.current_borrow_count > 0) {
        reader.current_borrow_count--;
        reader_update(&reader);
    }
    
    // 保存数据
    return borrow_save_data();
}

/**
 * @brief 续借图书
 * @param record_id 借阅记录ID
 * @param new_due_date 新的应还日期
 * @return 成功返回0，失败返回非0值
 */
int renew_book(const char *record_id, time_t new_due_date) {
    if (record_id == NULL) {
        return -1;
    }
    
    // 查找借阅记录
    int index = -1;
    for (int i = 0; i < borrow_count; i++) {
        if (strcmp(borrows[i].id, record_id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -2; // 借阅记录不存在
    }
    
    // 检查是否已归还
    if (borrows[index].status == BORROW_STATUS_RETURNED) {
        return -3; // 已归还，不能续借
    }
    
    // 检查续借次数
    if (borrows[index].renew_count >= MAX_RENEW_COUNT) {
        return -4; // 超过最大续借次数
    }
    
    // 检查是否逾期
    time_t current_time = get_current_time();
    if (borrows[index].due_date < current_time) {
        borrows[index].status = BORROW_STATUS_OVERDUE;
        return -5; // 已逾期，不能续借
    }
    
    // 更新借阅记录
    if (new_due_date == 0) {
        // 如果没有指定新的应还日期，则默认延长RENEW_DAYS天
        borrows[index].due_date += RENEW_DAYS * 24 * 60 * 60;
    } else {
        borrows[index].due_date = new_due_date;
    }
    
    borrows[index].renew_count++;
    borrows[index].status = BORROW_STATUS_RENEWED;
    
    // 保存数据
    return borrow_save_data();
}

/**
 * @brief 根据ID查找借阅记录
 * @param id 借阅记录ID
 * @param record 用于存储查找结果的借阅记录结构体指针
 * @return 成功返回0，失败返回非0值
 */
int borrow_find_by_id(const char *id, BorrowRecord *record) {
    if (id == NULL || record == NULL) {
        return -1;
    }
    
    // 查找借阅记录
    for (int i = 0; i < borrow_count; i++) {
        if (strcmp(borrows[i].id, id) == 0) {
            memcpy(record, &borrows[i], sizeof(BorrowRecord));
            return 0;
        }
    }
    
    return -1;
}

/**
 * @brief 查找读者的借阅记录
 * @param reader_id 读者ID
 * @param records 用于存储查找结果的借阅记录结构体数组
 * @param max_count 最大返回数量
 * @return 返回找到的借阅记录数量
 */
int borrow_find_by_reader(const char *reader_id, BorrowRecord *records, int max_count) {
    if (reader_id == NULL || records == NULL || max_count <= 0) {
        return 0;
    }
    
    int count = 0;
    
    // 查找借阅记录
    for (int i = 0; i < borrow_count && count < max_count; i++) {
        if (strcmp(borrows[i].reader_id, reader_id) == 0) {
            memcpy(&records[count], &borrows[i], sizeof(BorrowRecord));
            count++;
        }
    }
    
    return count;
}

/**
 * @brief 查找图书的借阅记录
 * @param book_id 图书ID
 * @param records 用于存储查找结果的借阅记录结构体数组
 * @param max_count 最大返回数量
 * @return 返回找到的借阅记录数量
 */
int borrow_find_by_book(const char *book_id, BorrowRecord *records, int max_count) {
    if (book_id == NULL || records == NULL || max_count <= 0) {
        return 0;
    }
    
    int count = 0;
    
    // 查找借阅记录
    for (int i = 0; i < borrow_count && count < max_count; i++) {
        if (strcmp(borrows[i].book_id, book_id) == 0) {
            memcpy(&records[count], &borrows[i], sizeof(BorrowRecord));
            count++;
        }
    }
    
    return count;
}

/**
 * @brief 获取所有借阅记录
 * @param records 用于存储借阅记录的结构体数组
 * @param max_count 最大返回数量
 * @return 返回借阅记录总数
 */
int borrow_get_all(BorrowRecord *records, int max_count) {
    if (records == NULL || max_count <= 0) {
        return 0;
    }
    
    int count = (borrow_count < max_count) ? borrow_count : max_count;
    
    // 复制借阅记录
    memcpy(records, borrows, sizeof(BorrowRecord) * count);
    
    return count;
}

/**
 * @brief 获取逾期的借阅记录
 * @param records 用于存储借阅记录的结构体数组
 * @param max_count 最大返回数量
 * @return 返回逾期借阅记录数量
 */
int borrow_get_overdue(BorrowRecord *records, int max_count) {
    if (records == NULL || max_count <= 0) {
        return 0;
    }
    
    int count = 0;
    time_t current_time = get_current_time();
    
    // 查找逾期借阅记录
    for (int i = 0; i < borrow_count && count < max_count; i++) {
        if (borrows[i].status != BORROW_STATUS_RETURNED && 
            borrows[i].due_date < current_time) {
            // 更新状态为逾期
            borrows[i].status = BORROW_STATUS_OVERDUE;
            
            memcpy(&records[count], &borrows[i], sizeof(BorrowRecord));
            count++;
        }
    }
    
    // 如果有状态更新，保存数据
    if (count > 0) {
        borrow_save_data();
    }
    
    return count;
}

/**
 * @brief 保存借阅数据到文件
 * @return 成功返回0，失败返回非0值
 */
int borrow_save_data() {
    FILE *file = fopen(BORROWS_FILE, "w");
    if (file == NULL) {
        return -1;
    }
    
    // 写入标题行
    fprintf(file, "id,book_id,reader_id,borrow_date,due_date,return_date,status,renew_count\n");
    
    // 写入数据
    for (int i = 0; i < borrow_count; i++) {
        fprintf(file, "%s,%s,%s,%ld,%ld,%ld,%d,%d\n",
                borrows[i].id,
                borrows[i].book_id,
                borrows[i].reader_id,
                (long)borrows[i].borrow_date,
                (long)borrows[i].due_date,
                (long)borrows[i].return_date,
                borrows[i].status,
                borrows[i].renew_count);
    }
    
    fclose(file);
    return 0;
}

/**
 * @brief 从文件加载借阅数据
 * @return 成功返回0，失败返回非0值
 */
int borrow_load_data() {
    // 如果文件不存在，直接返回成功
    if (!file_exists(BORROWS_FILE)) {
        return 0;
    }
    
    FILE *file = fopen(BORROWS_FILE, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[MAX_LINE_SIZE];
    char *fields[8]; // id,book_id,reader_id,borrow_date,due_date,return_date,status,renew_count
    
    // 跳过标题行
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }
    
    // 读取数据
    borrow_count = 0;
    while (fgets(line, sizeof(line), file) != NULL && borrow_count < borrow_capacity) {
        // 去除换行符
        line[strcspn(line, "\n")] = '\0';
        
        // 解析CSV行
        if (parse_csv_line(line, fields, 8) != 8) {
            continue;
        }
        
        // 填充借阅记录结构体
        strncpy(borrows[borrow_count].id, fields[0], sizeof(borrows[borrow_count].id) - 1);
        borrows[borrow_count].id[sizeof(borrows[borrow_count].id) - 1] = '\0';
        
        strncpy(borrows[borrow_count].book_id, fields[1], sizeof(borrows[borrow_count].book_id) - 1);
        borrows[borrow_count].book_id[sizeof(borrows[borrow_count].book_id) - 1] = '\0';
        
        strncpy(borrows[borrow_count].reader_id, fields[2], sizeof(borrows[borrow_count].reader_id) - 1);
        borrows[borrow_count].reader_id[sizeof(borrows[borrow_count].reader_id) - 1] = '\0';
        
        borrows[borrow_count].borrow_date = (time_t)atol(fields[3]);
        borrows[borrow_count].due_date = (time_t)atol(fields[4]);
        borrows[borrow_count].return_date = (time_t)atol(fields[5]);
        borrows[borrow_count].status = (BorrowStatus)atoi(fields[6]);
        borrows[borrow_count].renew_count = atoi(fields[7]);
        
        borrow_count++;
    }
    
    fclose(file);
    return 0;
}

/**
 * @brief 清理借阅管理模块资源
 */
void borrow_cleanup() {
    if (borrows != NULL) {
        free(borrows);
        borrows = NULL;
    }
    
    borrow_count = 0;
    borrow_capacity = 0;
}