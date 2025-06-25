/**
 * @file ui.h
 * @brief 用户界面相关函数和数据结构的声明
 */

#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include "book.h"
#include "reader.h"
#include "borrow.h"

/**
 * @brief 初始化用户界面
 * @param argc 命令行参数数量
 * @param argv 命令行参数
 * @return 成功返回0，失败返回非0值
 */
int ui_init(int argc, char *argv[]);

/**
 * @brief 运行用户界面主循环
 */
void ui_run();

/**
 * @brief 清理用户界面资源
 */
void ui_cleanup();

/**
 * @brief 显示主窗口
 */
void ui_show_main_window();

/**
 * @brief 显示图书管理窗口
 */
void ui_show_book_management_window();

/**
 * @brief 显示读者管理窗口
 */
void ui_show_reader_management_window();

/**
 * @brief 显示借阅管理窗口
 */
void ui_show_borrow_management_window();

/**
 * @brief 显示添加图书对话框
 */
void ui_show_add_book_dialog();

/**
 * @brief 显示编辑图书对话框
 * @param book 要编辑的图书
 */
void ui_show_edit_book_dialog(Book *book);

/**
 * @brief 显示添加读者对话框
 */
void ui_show_add_reader_dialog();

/**
 * @brief 显示编辑读者对话框
 * @param reader 要编辑的读者
 */
void ui_show_edit_reader_dialog(Reader *reader);

/**
 * @brief 显示借书对话框
 */
void ui_show_borrow_book_dialog();

/**
 * @brief 显示还书对话框
 */
void ui_show_return_book_dialog();

/**
 * @brief 显示续借对话框
 */
void ui_show_renew_book_dialog();

/**
 * @brief 显示消息对话框
 * @param parent 父窗口
 * @param message_type 消息类型
 * @param buttons_type 按钮类型
 * @param title 标题
 * @param message 消息内容
 * @return 用户响应
 */
int ui_show_message_dialog(GtkWindow *parent, GtkMessageType message_type, 
                          GtkButtonsType buttons_type, const char *title, 
                          const char *message);

/**
 * @brief 显示确认对话框
 * @param parent 父窗口
 * @param title 标题
 * @param message 消息内容
 * @return 用户确认返回TRUE，否则返回FALSE
 */
gboolean ui_show_confirm_dialog(GtkWindow *parent, const char *title, const char *message);

/**
 * @brief 显示错误对话框
 * @param parent 父窗口
 * @param title 标题
 * @param message 错误消息
 */
void ui_show_error_dialog(GtkWindow *parent, const char *title, const char *message);

/**
 * @brief 刷新图书列表
 */
void ui_refresh_book_list();

/**
 * @brief 刷新读者列表
 */
void ui_refresh_reader_list();

/**
 * @brief 刷新借阅列表
 */
void ui_refresh_borrow_list();

#endif /* UI_H */