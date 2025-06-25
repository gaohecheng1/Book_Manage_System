/**
 * @file book.c
 * @brief 图书管理相关函数的实现
 */

#include "book.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 1000
#define BOOKS_FILE "data/books.csv"
#define MAX_LINE_SIZE 1024

// 图书数组
static Book *books = NULL;
// 图书数量
static int book_count = 0;
// 图书容量
static int book_capacity = 0;

/**
 * @brief 初始化图书管理模块
 * @return 成功返回0，失败返回非0值
 */
int book_init() {
    // 分配内存
    books = (Book *)malloc(sizeof(Book) * MAX_BOOKS);
    if (books == NULL) {
        return -1;
    }
    
    book_capacity = MAX_BOOKS;
    book_count = 0;
    
    // 加载数据
    return book_load_data();
}

/**
 * @brief 添加新图书
 * @param book 要添加的图书信息
 * @return 成功返回0，失败返回非0值
 */
int book_add(Book *book) {
    if (book == NULL) {
        return -1;
    }
    
    // 检查容量
    if (book_count >= book_capacity) {
        return -1;
    }
    
    // 生成ID（如果没有）
    if (strlen(book->id) == 0) {
        generate_id("B", book->id, sizeof(book->id));
    }
    
    // 添加图书
    memcpy(&books[book_count], book, sizeof(Book));
    book_count++;
    
    // 保存数据
    return book_save_data();
}

/**
 * @brief 根据ID删除图书
 * @param id 图书ID
 * @return 成功返回0，失败返回非0值
 */
int book_delete(const char *id) {
    if (id == NULL) {
        return -1;
    }
    
    // 查找图书
    int index = -1;
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    // 删除图书（移动后面的元素）
    for (int i = index; i < book_count - 1; i++) {
        memcpy(&books[i], &books[i + 1], sizeof(Book));
    }
    
    book_count--;
    
    // 保存数据
    return book_save_data();
}

/**
 * @brief 更新图书信息
 * @param book 更新后的图书信息
 * @return 成功返回0，失败返回非0值
 */
int book_update(Book *book) {
    if (book == NULL) {
        return -1;
    }
    
    // 查找图书
    int index = -1;
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].id, book->id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    // 更新图书
    memcpy(&books[index], book, sizeof(Book));
    
    // 保存数据
    return book_save_data();
}

/**
 * @brief 根据ID查找图书
 * @param id 图书ID
 * @param book 用于存储查找结果的图书结构体指针
 * @return 成功返回0，失败返回非0值
 */
int book_find_by_id(const char *id, Book *book) {
    if (id == NULL || book == NULL) {
        return -1;
    }
    
    // 查找图书
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].id, id) == 0) {
            memcpy(book, &books[i], sizeof(Book));
            return 0;
        }
    }
    
    return -1;
}

/**
 * @brief 根据标题查找图书
 * @param title 图书标题
 * @param result_books 用于存储查找结果的图书结构体数组
 * @param max_count 最大返回数量
 * @return 返回找到的图书数量
 */
int book_find_by_title(const char *title, Book *result_books, int max_count) {
    if (title == NULL || result_books == NULL || max_count <= 0) {
        return 0;
    }
    
    int count = 0;
    
    // 查找图书
    for (int i = 0; i < book_count && count < max_count; i++) {
        if (contains_ignore_case(books[i].title, title)) {
            memcpy(&result_books[count], &books[i], sizeof(Book));
            count++;
        }
    }
    
    return count;
}

/**
 * @brief 获取所有图书
 * @param result_books 用于存储图书的结构体数组
 * @param max_count 最大返回数量
 * @return 返回图书总数
 */
int book_get_all(Book *result_books, int max_count) {
    if (result_books == NULL || max_count <= 0) {
        return 0;
    }
    
    int count = (book_count < max_count) ? book_count : max_count;
    
    // 复制图书
    memcpy(result_books, books, sizeof(Book) * count);
    
    return count;
}

/**
 * @brief 保存图书数据到文件
 * @return 成功返回0，失败返回非0值
 */
int book_save_data() {
    FILE *file = fopen(BOOKS_FILE, "w");
    if (file == NULL) {
        return -1;
    }
    
    // 写入标题行
    fprintf(file, "id,title,author,publisher,isbn,publish_year,total_count,available_count\n");
    
    // 写入数据
    for (int i = 0; i < book_count; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%d,%d,%d\n",
                books[i].id,
                books[i].title,
                books[i].author,
                books[i].publisher,
                books[i].isbn,
                books[i].publish_year,
                books[i].total_count,
                books[i].available_count);
    }
    
    fclose(file);
    return 0;
}

/**
 * @brief 从文件加载图书数据
 * @return 成功返回0，失败返回非0值
 */
int book_load_data() {
    // 如果文件不存在，直接返回成功
    if (!file_exists(BOOKS_FILE)) {
        return 0;
    }
    
    FILE *file = fopen(BOOKS_FILE, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[MAX_LINE_SIZE];
    char *fields[8]; // id,title,author,publisher,isbn,publish_year,total_count,available_count
    
    // 跳过标题行
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }
    
    // 读取数据
    book_count = 0;
    while (fgets(line, sizeof(line), file) != NULL && book_count < book_capacity) {
        // 去除换行符
        line[strcspn(line, "\n")] = '\0';
        
        // 解析CSV行
        if (parse_csv_line(line, fields, 8) != 8) {
            continue;
        }
        
        // 填充图书结构体
        strncpy(books[book_count].id, fields[0], sizeof(books[book_count].id) - 1);
        books[book_count].id[sizeof(books[book_count].id) - 1] = '\0';
        
        strncpy(books[book_count].title, fields[1], sizeof(books[book_count].title) - 1);
        books[book_count].title[sizeof(books[book_count].title) - 1] = '\0';
        
        strncpy(books[book_count].author, fields[2], sizeof(books[book_count].author) - 1);
        books[book_count].author[sizeof(books[book_count].author) - 1] = '\0';
        
        strncpy(books[book_count].publisher, fields[3], sizeof(books[book_count].publisher) - 1);
        books[book_count].publisher[sizeof(books[book_count].publisher) - 1] = '\0';
        
        strncpy(books[book_count].isbn, fields[4], sizeof(books[book_count].isbn) - 1);
        books[book_count].isbn[sizeof(books[book_count].isbn) - 1] = '\0';
        
        books[book_count].publish_year = atoi(fields[5]);
        books[book_count].total_count = atoi(fields[6]);
        books[book_count].available_count = atoi(fields[7]);
        
        book_count++;
    }
    
    fclose(file);
    return 0;
}

/**
 * @brief 清理图书管理模块资源
 */
void book_cleanup() {
    if (books != NULL) {
        free(books);
        books = NULL;
    }
    
    book_count = 0;
    book_capacity = 0;
}