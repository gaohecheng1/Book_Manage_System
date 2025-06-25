/**
 * @file utils.c
 * @brief 工具函数的实现
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <unistd.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

/**
 * @brief 生成唯一ID
 * @param prefix ID前缀
 * @param id 用于存储生成的ID的字符串
 * @param size id字符串的大小
 */
void generate_id(const char *prefix, char *id, size_t size) {
    if (prefix == NULL || id == NULL || size == 0) {
        return;
    }
    
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    
    // 生成格式：前缀 + 年月日时分秒 + 随机数
    snprintf(id, size, "%s%04d%02d%02d%02d%02d%02d%03d",
             prefix,
             tm_info->tm_year + 1900,
             tm_info->tm_mon + 1,
             tm_info->tm_mday,
             tm_info->tm_hour,
             tm_info->tm_min,
             tm_info->tm_sec,
             rand() % 1000);
}

/**
 * @brief 获取当前时间戳
 * @return 当前时间戳
 */
time_t get_current_time() {
    return time(NULL);
}

/**
 * @brief 将时间戳转换为字符串
 * @param timestamp 时间戳
 * @param buffer 用于存储转换结果的字符串
 * @param size buffer的大小
 * @param format 时间格式
 */
void time_to_string(time_t timestamp, char *buffer, size_t size, const char *format) {
    if (buffer == NULL || size == 0) {
        return;
    }
    
    if (timestamp == 0) {
        strncpy(buffer, "N/A", size - 1);
        buffer[size - 1] = '\0';
        return;
    }
    
    struct tm *tm_info = localtime(&timestamp);
    strftime(buffer, size, format != NULL ? format : "%Y-%m-%d %H:%M:%S", tm_info);
}

/**
 * @brief 将字符串转换为时间戳
 * @param str 时间字符串
 * @param format 时间格式
 * @return 转换后的时间戳
 */
time_t string_to_time(const char *str, const char *format) {
    if (str == NULL) {
        return 0;
    }
    
    struct tm tm_info = {0};
    if (strptime(str, format != NULL ? format : "%Y-%m-%d %H:%M:%S", &tm_info) == NULL) {
        return 0;
    }
    
    return mktime(&tm_info);
}

/**
 * @brief 计算两个日期之间的天数差
 * @param date1 日期1
 * @param date2 日期2
 * @return 天数差
 */
int days_between(time_t date1, time_t date2) {
    // 转换为天数
    int days1 = (int)(date1 / (24 * 60 * 60));
    int days2 = (int)(date2 / (24 * 60 * 60));
    
    return days2 - days1;
}

/**
 * @brief 检查文件是否存在
 * @param filename 文件名
 * @return 存在返回1，不存在返回0
 */
int file_exists(const char *filename) {
    if (filename == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    
    return 0;
}

/**
 * @brief 创建目录
 * @param path 目录路径
 * @return 成功返回0，失败返回非0值
 */
int create_directory(const char *path) {
    if (path == NULL) {
        return -1;
    }
    
    // 检查目录是否已存在
    struct stat st = {0};
    if (stat(path, &st) == 0) {
        // 目录已存在
        return 0;
    }
    
    // 创建目录
    return MKDIR(path);
}

/**
 * @brief 将字符串转换为小写
 * @param str 要转换的字符串
 */
void to_lower_case(char *str) {
    if (str == NULL) {
        return;
    }
    
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

/**
 * @brief 去除字符串两端的空白字符
 * @param str 要处理的字符串
 */
void trim(char *str) {
    if (str == NULL) {
        return;
    }
    
    // 去除前导空白字符
    char *p = str;
    while (isspace(*p)) {
        p++;
    }
    
    if (p != str) {
        memmove(str, p, strlen(p) + 1);
    }
    
    // 去除尾部空白字符
    p = str + strlen(str) - 1;
    while (p >= str && isspace(*p)) {
        *p = '\0';
        p--;
    }
}

/**
 * @brief 检查字符串是否包含子串（不区分大小写）
 * @param str 要检查的字符串
 * @param substr 子串
 * @return 包含返回1，不包含返回0
 */
int contains_ignore_case(const char *str, const char *substr) {
    if (str == NULL || substr == NULL) {
        return 0;
    }
    
    // 转换为小写进行比较
    char *str_lower = strdup(str);
    char *substr_lower = strdup(substr);
    
    if (str_lower == NULL || substr_lower == NULL) {
        if (str_lower) free(str_lower);
        if (substr_lower) free(substr_lower);
        return 0;
    }
    
    to_lower_case(str_lower);
    to_lower_case(substr_lower);
    
    int result = strstr(str_lower, substr_lower) != NULL;
    
    free(str_lower);
    free(substr_lower);
    
    return result;
}

/**
 * @brief 将CSV行解析为字段数组
 * @param line CSV行
 * @param fields 用于存储字段的数组
 * @param max_fields 最大字段数
 * @return 解析出的字段数
 */
int parse_csv_line(const char *line, char **fields, int max_fields) {
    if (line == NULL || fields == NULL || max_fields <= 0) {
        return 0;
    }
    
    int field_count = 0;
    const char *p = line;
    const char *field_start = p;
    int in_quotes = 0;
    
    // 为每个字段分配内存
    for (int i = 0; i < max_fields; i++) {
        fields[i] = (char *)malloc(strlen(line) + 1);
        if (fields[i] == NULL) {
            // 释放已分配的内存
            for (int j = 0; j < i; j++) {
                free(fields[j]);
                fields[j] = NULL;
            }
            return 0;
        }
        fields[i][0] = '\0';
    }
    
    while (*p && field_count < max_fields) {
        if (*p == '"') {
            // 处理引号
            in_quotes = !in_quotes;
        } else if (*p == ',' && !in_quotes) {
            // 字段结束
            int len = p - field_start;
            strncpy(fields[field_count], field_start, len);
            fields[field_count][len] = '\0';
            
            // 去除引号
            if (fields[field_count][0] == '"' && fields[field_count][len - 1] == '"') {
                memmove(fields[field_count], fields[field_count] + 1, len - 2);
                fields[field_count][len - 2] = '\0';
            }
            
            field_count++;
            field_start = p + 1;
        }
        
        p++;
    }
    
    // 处理最后一个字段
    if (field_count < max_fields) {
        int len = p - field_start;
        strncpy(fields[field_count], field_start, len);
        fields[field_count][len] = '\0';
        
        // 去除引号
        if (fields[field_count][0] == '"' && fields[field_count][len - 1] == '"') {
            memmove(fields[field_count], fields[field_count] + 1, len - 2);
            fields[field_count][len - 2] = '\0';
        }
        
        field_count++;
    }
    
    return field_count;
}

/**
 * @brief 将字段数组转换为CSV行
 * @param fields 字段数组
 * @param num_fields 字段数
 * @param line 用于存储CSV行的字符串
 * @param size line的大小
 */
void fields_to_csv_line(char **fields, int num_fields, char *line, size_t size) {
    if (fields == NULL || line == NULL || size == 0 || num_fields <= 0) {
        return;
    }
    
    line[0] = '\0';
    size_t pos = 0;
    
    for (int i = 0; i < num_fields; i++) {
        // 检查字段是否需要引号（包含逗号、引号或空白字符）
        int need_quotes = 0;
        if (fields[i] != NULL) {
            for (int j = 0; fields[i][j]; j++) {
                if (fields[i][j] == ',' || fields[i][j] == '"' || isspace(fields[i][j])) {
                    need_quotes = 1;
                    break;
                }
            }
        }
        
        // 添加字段
        if (need_quotes) {
            // 添加带引号的字段
            int len = snprintf(line + pos, size - pos, "\"%s\"", fields[i] != NULL ? fields[i] : "");
            if (len < 0 || (size_t)len >= size - pos) {
                // 缓冲区不足
                break;
            }
            pos += len;
        } else {
            // 添加不带引号的字段
            int len = snprintf(line + pos, size - pos, "%s", fields[i] != NULL ? fields[i] : "");
            if (len < 0 || (size_t)len >= size - pos) {
                // 缓冲区不足
                break;
            }
            pos += len;
        }
        
        // 添加逗号（除了最后一个字段）
        if (i < num_fields - 1) {
            if (pos < size - 1) {
                line[pos++] = ',';
                line[pos] = '\0';
            } else {
                // 缓冲区不足
                break;
            }
        }
    }
}