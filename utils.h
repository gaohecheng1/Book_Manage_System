/**
 * @file utils.h
 * @brief 工具函数的声明
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief 生成唯一ID
 * @param prefix ID前缀
 * @param id 用于存储生成的ID的字符串
 * @param size id字符串的大小
 */
void generate_id(const char *prefix, char *id, size_t size);

/**
 * @brief 获取当前时间戳
 * @return 当前时间戳
 */
time_t get_current_time();

/**
 * @brief 将时间戳转换为字符串
 * @param timestamp 时间戳
 * @param buffer 用于存储转换结果的字符串
 * @param size buffer的大小
 * @param format 时间格式
 */
void time_to_string(time_t timestamp, char *buffer, size_t size, const char *format);

/**
 * @brief 将字符串转换为时间戳
 * @param str 时间字符串
 * @param format 时间格式
 * @return 转换后的时间戳
 */
time_t string_to_time(const char *str, const char *format);

/**
 * @brief 计算两个日期之间的天数差
 * @param date1 日期1
 * @param date2 日期2
 * @return 天数差
 */
int days_between(time_t date1, time_t date2);

/**
 * @brief 检查文件是否存在
 * @param filename 文件名
 * @return 存在返回1，不存在返回0
 */
int file_exists(const char *filename);

/**
 * @brief 创建目录
 * @param path 目录路径
 * @return 成功返回0，失败返回非0值
 */
int create_directory(const char *path);

/**
 * @brief 将字符串转换为小写
 * @param str 要转换的字符串
 */
void to_lower_case(char *str);

/**
 * @brief 去除字符串两端的空白字符
 * @param str 要处理的字符串
 */
void trim(char *str);

/**
 * @brief 检查字符串是否包含子串（不区分大小写）
 * @param str 要检查的字符串
 * @param substr 子串
 * @return 包含返回1，不包含返回0
 */
int contains_ignore_case(const char *str, const char *substr);

/**
 * @brief 将CSV行解析为字段数组
 * @param line CSV行
 * @param fields 用于存储字段的数组
 * @param max_fields 最大字段数
 * @return 解析出的字段数
 */
int parse_csv_line(const char *line, char **fields, int max_fields);

/**
 * @brief 将字段数组转换为CSV行
 * @param fields 字段数组
 * @param num_fields 字段数
 * @param line 用于存储CSV行的字符串
 * @param size line的大小
 */
void fields_to_csv_line(char **fields, int num_fields, char *line, size_t size);

#endif /* UTILS_H */