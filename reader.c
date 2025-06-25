/**
 * @file reader.c
 * @brief 读者管理相关函数的实现
 */

#include "reader.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_READERS 1000
#define READERS_FILE "data/readers.csv"
#define MAX_LINE_SIZE 1024

// 读者数组
static Reader *readers = NULL;
// 读者数量
static int reader_count = 0;
// 读者容量
static int reader_capacity = 0;

/**
 * @brief 初始化读者管理模块
 * @return 成功返回0，失败返回非0值
 */
int reader_init() {
    // 分配内存
    readers = (Reader *)malloc(sizeof(Reader) * MAX_READERS);
    if (readers == NULL) {
        return -1;
    }
    
    reader_capacity = MAX_READERS;
    reader_count = 0;
    
    // 加载数据
    return reader_load_data();
}

/**
 * @brief 添加新读者
 * @param reader 要添加的读者信息
 * @return 成功返回0，失败返回非0值
 */
int reader_add(Reader *reader) {
    if (reader == NULL) {
        return -1;
    }
    
    // 检查容量
    if (reader_count >= reader_capacity) {
        return -1;
    }
    
    // 生成ID（如果没有）
    if (strlen(reader->id) == 0) {
        generate_id("R", reader->id, sizeof(reader->id));
    }
    
    // 添加读者
    memcpy(&readers[reader_count], reader, sizeof(Reader));
    reader_count++;
    
    // 保存数据
    return reader_save_data();
}

/**
 * @brief 根据ID删除读者
 * @param id 读者ID
 * @return 成功返回0，失败返回非0值
 */
int reader_delete(const char *id) {
    if (id == NULL) {
        return -1;
    }
    
    // 查找读者
    int index = -1;
    for (int i = 0; i < reader_count; i++) {
        if (strcmp(readers[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    // 检查是否有未归还的图书
    if (readers[index].current_borrow_count > 0) {
        return -2; // 有未归还的图书，不能删除
    }
    
    // 删除读者（移动后面的元素）
    for (int i = index; i < reader_count - 1; i++) {
        memcpy(&readers[i], &readers[i + 1], sizeof(Reader));
    }
    
    reader_count--;
    
    // 保存数据
    return reader_save_data();
}

/**
 * @brief 更新读者信息
 * @param reader 更新后的读者信息
 * @return 成功返回0，失败返回非0值
 */
int reader_update(Reader *reader) {
    if (reader == NULL) {
        return -1;
    }
    
    // 查找读者
    int index = -1;
    for (int i = 0; i < reader_count; i++) {
        if (strcmp(readers[i].id, reader->id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    // 保存当前借阅数量
    int current_borrow_count = readers[index].current_borrow_count;
    
    // 更新读者
    memcpy(&readers[index], reader, sizeof(Reader));
    
    // 恢复当前借阅数量（防止被覆盖）
    readers[index].current_borrow_count = current_borrow_count;
    
    // 保存数据
    return reader_save_data();
}

/**
 * @brief 根据ID查找读者
 * @param id 读者ID
 * @param reader 用于存储查找结果的读者结构体指针
 * @return 成功返回0，失败返回非0值
 */
int reader_find_by_id(const char *id, Reader *reader) {
    if (id == NULL || reader == NULL) {
        return -1;
    }
    
    // 查找读者
    for (int i = 0; i < reader_count; i++) {
        if (strcmp(readers[i].id, id) == 0) {
            memcpy(reader, &readers[i], sizeof(Reader));
            return 0;
        }
    }
    
    return -1;
}

/**
 * @brief 根据姓名查找读者
 * @param name 读者姓名
 * @param result_readers 用于存储查找结果的读者结构体数组
 * @param max_count 最大返回数量
 * @return 返回找到的读者数量
 */
int reader_find_by_name(const char *name, Reader *result_readers, int max_count) {
    if (name == NULL || result_readers == NULL || max_count <= 0) {
        return 0;
    }
    
    int count = 0;
    
    // 查找读者
    for (int i = 0; i < reader_count && count < max_count; i++) {
        if (contains_ignore_case(readers[i].name, name)) {
            memcpy(&result_readers[count], &readers[i], sizeof(Reader));
            count++;
        }
    }
    
    return count;
}

/**
 * @brief 获取所有读者
 * @param result_readers 用于存储读者的结构体数组
 * @param max_count 最大返回数量
 * @return 返回读者总数
 */
int reader_get_all(Reader *result_readers, int max_count) {
    if (result_readers == NULL || max_count <= 0) {
        return 0;
    }
    
    int count = (reader_count < max_count) ? reader_count : max_count;
    
    // 复制读者
    memcpy(result_readers, readers, sizeof(Reader) * count);
    
    return count;
}

/**
 * @brief 保存读者数据到文件
 * @return 成功返回0，失败返回非0值
 */
int reader_save_data() {
    FILE *file = fopen(READERS_FILE, "w");
    if (file == NULL) {
        return -1;
    }
    
    // 写入标题行
    fprintf(file, "id,name,gender,phone,email,address,max_borrow_count,current_borrow_count\n");
    
    // 写入数据
    for (int i = 0; i < reader_count; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s,%d,%d\n",
                readers[i].id,
                readers[i].name,
                readers[i].gender,
                readers[i].phone,
                readers[i].email,
                readers[i].address,
                readers[i].max_borrow_count,
                readers[i].current_borrow_count);
    }
    
    fclose(file);
    return 0;
}

/**
 * @brief 从文件加载读者数据
 * @return 成功返回0，失败返回非0值
 */
int reader_load_data() {
    // 如果文件不存在，直接返回成功
    if (!file_exists(READERS_FILE)) {
        return 0;
    }
    
    FILE *file = fopen(READERS_FILE, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[MAX_LINE_SIZE];
    char *fields[8]; // id,name,gender,phone,email,address,max_borrow_count,current_borrow_count
    
    // 跳过标题行
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }
    
    // 读取数据
    reader_count = 0;
    while (fgets(line, sizeof(line), file) != NULL && reader_count < reader_capacity) {
        // 去除换行符
        line[strcspn(line, "\n")] = '\0';
        
        // 解析CSV行
        if (parse_csv_line(line, fields, 8) != 8) {
            continue;
        }
        
        // 填充读者结构体
        strncpy(readers[reader_count].id, fields[0], sizeof(readers[reader_count].id) - 1);
        readers[reader_count].id[sizeof(readers[reader_count].id) - 1] = '\0';
        
        strncpy(readers[reader_count].name, fields[1], sizeof(readers[reader_count].name) - 1);
        readers[reader_count].name[sizeof(readers[reader_count].name) - 1] = '\0';
        
        strncpy(readers[reader_count].gender, fields[2], sizeof(readers[reader_count].gender) - 1);
        readers[reader_count].gender[sizeof(readers[reader_count].gender) - 1] = '\0';
        
        strncpy(readers[reader_count].phone, fields[3], sizeof(readers[reader_count].phone) - 1);
        readers[reader_count].phone[sizeof(readers[reader_count].phone) - 1] = '\0';
        
        strncpy(readers[reader_count].email, fields[4], sizeof(readers[reader_count].email) - 1);
        readers[reader_count].email[sizeof(readers[reader_count].email) - 1] = '\0';
        
        strncpy(readers[reader_count].address, fields[5], sizeof(readers[reader_count].address) - 1);
        readers[reader_count].address[sizeof(readers[reader_count].address) - 1] = '\0';
        
        readers[reader_count].max_borrow_count = atoi(fields[6]);
        readers[reader_count].current_borrow_count = atoi(fields[7]);
        
        reader_count++;
    }
    
    fclose(file);
    return 0;
}

/**
 * @brief 清理读者管理模块资源
 */
void reader_cleanup() {
    if (readers != NULL) {
        free(readers);
        readers = NULL;
    }
    
    reader_count = 0;
    reader_capacity = 0;
}