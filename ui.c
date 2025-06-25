/**
 * @file ui.c
 * @brief 用户界面相关函数的实现
 */

#include "ui.h"
#include "book.h"
#include "reader.h"
#include "borrow.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 主窗口
static GtkWidget *main_window = NULL;

// 图书管理窗口
static GtkWidget *book_window = NULL;
static GtkWidget *book_list_view = NULL;
static GtkListStore *book_list_store = NULL;

// 读者管理窗口
static GtkWidget *reader_window = NULL;
static GtkWidget *reader_list_view = NULL;
static GtkListStore *reader_list_store = NULL;

// 借阅管理窗口
static GtkWidget *borrow_window = NULL;
static GtkWidget *borrow_list_view = NULL;
static GtkListStore *borrow_list_store = NULL;

// 回调函数前向声明
static void on_main_window_destroy(GtkWidget *widget, gpointer data);
static void on_book_management_clicked(GtkWidget *widget, gpointer data);
static void on_reader_management_clicked(GtkWidget *widget, gpointer data);
static void on_borrow_management_clicked(GtkWidget *widget, gpointer data);
static void on_add_book_clicked(GtkWidget *widget, gpointer data);
static void on_edit_book_clicked(GtkWidget *widget, gpointer data);
static void on_delete_book_clicked(GtkWidget *widget, gpointer data);
static void on_add_reader_clicked(GtkWidget *widget, gpointer data);
static void on_edit_reader_clicked(GtkWidget *widget, gpointer data);
static void on_delete_reader_clicked(GtkWidget *widget, gpointer data);
static void on_borrow_book_clicked(GtkWidget *widget, gpointer data);
static void on_return_book_clicked(GtkWidget *widget, gpointer data);
static void on_renew_book_clicked(GtkWidget *widget, gpointer data);

/**
 * @brief 初始化用户界面
 * @param argc 命令行参数数量
 * @param argv 命令行参数
 * @return 成功返回0，失败返回非0值
 */
int ui_init(int argc, char *argv[]) {
    // 初始化GTK+
    gtk_init(&argc, &argv);
    
    // 设置随机数种子
    srand((unsigned int)time(NULL));
    
    return 0;
}

/**
 * @brief 运行用户界面主循环
 */
void ui_run() {
    // 显示主窗口
    ui_show_main_window();
    
    // 运行GTK+主循环
    gtk_main();
}

/**
 * @brief 清理用户界面资源
 */
void ui_cleanup() {
    // GTK+会自动清理资源
}

/**
 * @brief 显示主窗口
 */
void ui_show_main_window() {
    // 创建主窗口
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "图书借阅归还管理系统");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
    g_signal_connect(main_window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    
    // 创建垂直布局
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    
    // 创建标题标签
    GtkWidget *title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), "<span font='20' weight='bold'>图书借阅归还管理系统</span>");
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 20);
    
    // 创建按钮
    GtkWidget *book_button = gtk_button_new_with_label("图书管理");
    gtk_widget_set_size_request(book_button, 200, 50);
    g_signal_connect(book_button, "clicked", G_CALLBACK(on_book_management_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), book_button, FALSE, FALSE, 10);
    
    GtkWidget *reader_button = gtk_button_new_with_label("读者管理");
    gtk_widget_set_size_request(reader_button, 200, 50);
    g_signal_connect(reader_button, "clicked", G_CALLBACK(on_reader_management_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), reader_button, FALSE, FALSE, 10);
    
    GtkWidget *borrow_button = gtk_button_new_with_label("借阅管理");
    gtk_widget_set_size_request(borrow_button, 200, 50);
    g_signal_connect(borrow_button, "clicked", G_CALLBACK(on_borrow_management_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), borrow_button, FALSE, FALSE, 10);
    
    // 显示窗口
    gtk_widget_show_all(main_window);
}

/**
 * @brief 显示图书管理窗口
 */
void ui_show_book_management_window() {
    // 如果窗口已存在，直接显示
    if (book_window != NULL) {
        gtk_window_present(GTK_WINDOW(book_window));
        return;
    }
    
    // 创建窗口
    book_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(book_window), "图书管理");
    gtk_window_set_default_size(GTK_WINDOW(book_window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(book_window), GTK_WIN_POS_CENTER);
    gtk_window_set_transient_for(GTK_WINDOW(book_window), GTK_WINDOW(main_window));
    g_signal_connect(book_window, "destroy", G_CALLBACK(gtk_widget_destroyed), &book_window);
    
    // 创建垂直布局
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(book_window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    
    // 创建工具栏
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
    
    // 添加按钮
    GtkWidget *add_button = gtk_button_new_with_label("添加图书");
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_book_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), add_button, FALSE, FALSE, 0);
    
    GtkWidget *edit_button = gtk_button_new_with_label("编辑图书");
    g_signal_connect(edit_button, "clicked", G_CALLBACK(on_edit_book_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), edit_button, FALSE, FALSE, 0);
    
    GtkWidget *delete_button = gtk_button_new_with_label("删除图书");
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_book_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), delete_button, FALSE, FALSE, 0);
    
    // 创建滚动窗口
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
    
    // 创建列表存储
    book_list_store = gtk_list_store_new(7,
                                        G_TYPE_STRING,  // ID
                                        G_TYPE_STRING,  // 标题
                                        G_TYPE_STRING,  // 作者
                                        G_TYPE_STRING,  // 出版社
                                        G_TYPE_STRING,  // ISBN
                                        G_TYPE_INT,     // 出版年份
                                        G_TYPE_INT);    // 可借数量
    
    // 创建树视图
    book_list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(book_list_store));
    gtk_container_add(GTK_CONTAINER(scrolled_window), book_list_view);
    
    // 添加列
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("标题", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("作者", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("出版社", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ISBN", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("出版年份", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("可借数量", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list_view), column);
    
    // 刷新图书列表
    ui_refresh_book_list();
    
    // 显示窗口
    gtk_widget_show_all(book_window);
}

/**
 * @brief 显示读者管理窗口
 */
void ui_show_reader_management_window() {
    // 如果窗口已存在，直接显示
    if (reader_window != NULL) {
        gtk_window_present(GTK_WINDOW(reader_window));
        return;
    }
    
    // 创建窗口
    reader_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(reader_window), "读者管理");
    gtk_window_set_default_size(GTK_WINDOW(reader_window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(reader_window), GTK_WIN_POS_CENTER);
    gtk_window_set_transient_for(GTK_WINDOW(reader_window), GTK_WINDOW(main_window));
    g_signal_connect(reader_window, "destroy", G_CALLBACK(gtk_widget_destroyed), &reader_window);
    
    // 创建垂直布局
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(reader_window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    
    // 创建工具栏
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
    
    // 添加按钮
    GtkWidget *add_button = gtk_button_new_with_label("添加读者");
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_reader_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), add_button, FALSE, FALSE, 0);
    
    GtkWidget *edit_button = gtk_button_new_with_label("编辑读者");
    g_signal_connect(edit_button, "clicked", G_CALLBACK(on_edit_reader_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), edit_button, FALSE, FALSE, 0);
    
    GtkWidget *delete_button = gtk_button_new_with_label("删除读者");
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_reader_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), delete_button, FALSE, FALSE, 0);
    
    // 创建滚动窗口
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
    
    // 创建列表存储
    reader_list_store = gtk_list_store_new(7,
                                          G_TYPE_STRING,  // ID
                                          G_TYPE_STRING,  // 姓名
                                          G_TYPE_STRING,  // 性别
                                          G_TYPE_STRING,  // 电话
                                          G_TYPE_STRING,  // 邮箱
                                          G_TYPE_INT,     // 最大借阅数量
                                          G_TYPE_INT);    // 当前借阅数量
    
    // 创建树视图
    reader_list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(reader_list_store));
    gtk_container_add(GTK_CONTAINER(scrolled_window), reader_list_view);
    
    // 添加列
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(reader_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("姓名", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(reader_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("性别", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(reader_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("电话", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(reader_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("邮箱", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(reader_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("最大借阅数量", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(reader_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("当前借阅数量", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(reader_list_view), column);
    
    // 刷新读者列表
    ui_refresh_reader_list();
    
    // 显示窗口
    gtk_widget_show_all(reader_window);
}

/**
 * @brief 显示借阅管理窗口
 */
void ui_show_borrow_management_window() {
    // 如果窗口已存在，直接显示
    if (borrow_window != NULL) {
        gtk_window_present(GTK_WINDOW(borrow_window));
        return;
    }
    
    // 创建窗口
    borrow_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(borrow_window), "借阅管理");
    gtk_window_set_default_size(GTK_WINDOW(borrow_window), 900, 600);
    gtk_window_set_position(GTK_WINDOW(borrow_window), GTK_WIN_POS_CENTER);
    gtk_window_set_transient_for(GTK_WINDOW(borrow_window), GTK_WINDOW(main_window));
    g_signal_connect(borrow_window, "destroy", G_CALLBACK(gtk_widget_destroyed), &borrow_window);
    
    // 创建垂直布局
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(borrow_window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    
    // 创建工具栏
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
    
    // 添加按钮
    GtkWidget *borrow_button = gtk_button_new_with_label("借书");
    g_signal_connect(borrow_button, "clicked", G_CALLBACK(on_borrow_book_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), borrow_button, FALSE, FALSE, 0);
    
    GtkWidget *return_button = gtk_button_new_with_label("还书");
    g_signal_connect(return_button, "clicked", G_CALLBACK(on_return_book_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), return_button, FALSE, FALSE, 0);
    
    GtkWidget *renew_button = gtk_button_new_with_label("续借");
    g_signal_connect(renew_button, "clicked", G_CALLBACK(on_renew_book_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), renew_button, FALSE, FALSE, 0);
    
    // 创建滚动窗口
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
    
    // 创建列表存储
    borrow_list_store = gtk_list_store_new(8,
                                          G_TYPE_STRING,  // ID
                                          G_TYPE_STRING,  // 图书ID
                                          G_TYPE_STRING,  // 图书标题
                                          G_TYPE_STRING,  // 读者ID
                                          G_TYPE_STRING,  // 读者姓名
                                          G_TYPE_STRING,  // 借阅日期
                                          G_TYPE_STRING,  // 应还日期
                                          G_TYPE_STRING); // 状态
    
    // 创建树视图
    borrow_list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(borrow_list_store));
    gtk_container_add(GTK_CONTAINER(scrolled_window), borrow_list_view);
    
    // 添加列
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(borrow_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("图书ID", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(borrow_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("图书标题", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(borrow_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("读者ID", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(borrow_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("读者姓名", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(borrow_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("借阅日期", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(borrow_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("应还日期", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(borrow_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("状态", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(borrow_list_view), column);
    
    // 刷新借阅列表
    ui_refresh_borrow_list();
    
    // 显示窗口
    gtk_widget_show_all(borrow_window);
}

/**
 * @brief 显示添加图书对话框
 */
void ui_show_add_book_dialog() {
    // 创建对话框
    GtkWidget *dialog = gtk_dialog_new_with_buttons("添加图书",
                                                  GTK_WINDOW(book_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "取消", GTK_RESPONSE_CANCEL,
                                                  "添加", GTK_RESPONSE_ACCEPT,
                                                  NULL);
    
    // 设置对话框大小
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    
    // 获取内容区域
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);
    
    // 创建表格布局
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // 添加标签和输入框
    GtkWidget *title_label = gtk_label_new("标题:");
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 1, 1);
    GtkWidget *title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 0, 1, 1);
    
    GtkWidget *author_label = gtk_label_new("作者:");
    gtk_grid_attach(GTK_GRID(grid), author_label, 0, 1, 1, 1);
    GtkWidget *author_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 1, 1, 1);
    
    GtkWidget *publisher_label = gtk_label_new("出版社:");
    gtk_grid_attach(GTK_GRID(grid), publisher_label, 0, 2, 1, 1);
    GtkWidget *publisher_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), publisher_entry, 1, 2, 1, 1);
    
    GtkWidget *isbn_label = gtk_label_new("ISBN:");
    gtk_grid_attach(GTK_GRID(grid), isbn_label, 0, 3, 1, 1);
    GtkWidget *isbn_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), isbn_entry, 1, 3, 1, 1);
    
    GtkWidget *year_label = gtk_label_new("出版年份:");
    gtk_grid_attach(GTK_GRID(grid), year_label, 0, 4, 1, 1);
    GtkWidget *year_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), year_entry, 1, 4, 1, 1);
    
    GtkWidget *count_label = gtk_label_new("数量:");
    gtk_grid_attach(GTK_GRID(grid), count_label, 0, 5, 1, 1);
    GtkWidget *count_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), count_entry, 1, 5, 1, 1);
    
    // 显示对话框
    gtk_widget_show_all(dialog);
    
    // 运行对话框
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        // 获取输入的数据
        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *author = gtk_entry_get_text(GTK_ENTRY(author_entry));
        const char *publisher = gtk_entry_get_text(GTK_ENTRY(publisher_entry));
        const char *isbn = gtk_entry_get_text(GTK_ENTRY(isbn_entry));
        const char *year_str = gtk_entry_get_text(GTK_ENTRY(year_entry));
        const char *count_str = gtk_entry_get_text(GTK_ENTRY(count_entry));
        
        // 检查输入
        if (strlen(title) == 0 || strlen(author) == 0 || strlen(publisher) == 0 ||
            strlen(isbn) == 0 || strlen(year_str) == 0 || strlen(count_str) == 0) {
            ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "请填写所有字段");
        } else {
            // 转换年份和数量
            int year = atoi(year_str);
            int count = atoi(count_str);
            
            if (year <= 0 || count <= 0) {
                ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "年份和数量必须大于0");
            } else {
                // 创建图书对象
                Book book;
                memset(&book, 0, sizeof(Book));
                strncpy(book.title, title, sizeof(book.title) - 1);
                strncpy(book.author, author, sizeof(book.author) - 1);
                strncpy(book.publisher, publisher, sizeof(book.publisher) - 1);
                strncpy(book.isbn, isbn, sizeof(book.isbn) - 1);
                book.publish_year = year;
                book.total_count = count;
                book.available_count = count;
                
                // 添加图书
                if (book_add(&book) == 0) {
                    // 刷新图书列表
                    ui_refresh_book_list();
                } else {
                    ui_show_error_dialog(GTK_WINDOW(dialog), "添加失败", "无法添加图书");
                }
            }
        }
    }
    
    // 销毁对话框
    gtk_widget_destroy(dialog);
}

/**
 * @brief 显示编辑图书对话框
 * @param book 要编辑的图书
 */
void ui_show_edit_book_dialog(Book *book) {
    if (book == NULL) {
        return;
    }
    
    // 创建对话框
    GtkWidget *dialog = gtk_dialog_new_with_buttons("编辑图书",
                                                  GTK_WINDOW(book_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "取消", GTK_RESPONSE_CANCEL,
                                                  "保存", GTK_RESPONSE_ACCEPT,
                                                  NULL);
    
    // 设置对话框大小
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    
    // 获取内容区域
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);
    
    // 创建表格布局
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // 添加标签和输入框
    GtkWidget *id_label = gtk_label_new("ID:");
    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(id_entry), book->id);
    gtk_editable_set_editable(GTK_EDITABLE(id_entry), FALSE);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    
    GtkWidget *title_label = gtk_label_new("标题:");
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 1, 1, 1);
    GtkWidget *title_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(title_entry), book->title);
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 1, 1, 1);
    
    GtkWidget *author_label = gtk_label_new("作者:");
    gtk_grid_attach(GTK_GRID(grid), author_label, 0, 2, 1, 1);
    GtkWidget *author_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(author_entry), book->author);
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 2, 1, 1);
    
    GtkWidget *publisher_label = gtk_label_new("出版社:");
    gtk_grid_attach(GTK_GRID(grid), publisher_label, 0, 3, 1, 1);
    GtkWidget *publisher_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(publisher_entry), book->publisher);
    gtk_grid_attach(GTK_GRID(grid), publisher_entry, 1, 3, 1, 1);
    
    GtkWidget *isbn_label = gtk_label_new("ISBN:");
    gtk_grid_attach(GTK_GRID(grid), isbn_label, 0, 4, 1, 1);
    GtkWidget *isbn_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(isbn_entry), book->isbn);
    gtk_grid_attach(GTK_GRID(grid), isbn_entry, 1, 4, 1, 1);
    
    GtkWidget *year_label = gtk_label_new("出版年份:");
    gtk_grid_attach(GTK_GRID(grid), year_label, 0, 5, 1, 1);
    GtkWidget *year_entry = gtk_entry_new();
    char year_str[16];
    snprintf(year_str, sizeof(year_str), "%d", book->publish_year);
    gtk_entry_set_text(GTK_ENTRY(year_entry), year_str);
    gtk_grid_attach(GTK_GRID(grid), year_entry, 1, 5, 1, 1);
    
    GtkWidget *count_label = gtk_label_new("总数量:");
    gtk_grid_attach(GTK_GRID(grid), count_label, 0, 6, 1, 1);
    GtkWidget *count_entry = gtk_entry_new();
    char count_str[16];
    snprintf(count_str, sizeof(count_str), "%d", book->total_count);
    gtk_entry_set_text(GTK_ENTRY(count_entry), count_str);
    gtk_grid_attach(GTK_GRID(grid), count_entry, 1, 6, 1, 1);
    
    // 显示对话框
    gtk_widget_show_all(dialog);
    
    // 运行对话框
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        // 获取输入的数据
        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *author = gtk_entry_get_text(GTK_ENTRY(author_entry));
        const char *publisher = gtk_entry_get_text(GTK_ENTRY(publisher_entry));
        const char *isbn = gtk_entry_get_text(GTK_ENTRY(isbn_entry));
        const char *year_str = gtk_entry_get_text(GTK_ENTRY(year_entry));
        const char *count_str = gtk_entry_get_text(GTK_ENTRY(count_entry));
        
        // 检查输入
        if (strlen(title) == 0 || strlen(author) == 0 || strlen(publisher) == 0 ||
            strlen(isbn) == 0 || strlen(year_str) == 0 || strlen(count_str) == 0) {
            ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "请填写所有字段");
        } else {
            // 转换年份和数量
            int year = atoi(year_str);
            int count = atoi(count_str);
            
            if (year <= 0 || count <= 0) {
                ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "年份和数量必须大于0");
            } else {
                // 更新图书对象
                Book updated_book = *book;
                strncpy(updated_book.title, title, sizeof(updated_book.title) - 1);
                strncpy(updated_book.author, author, sizeof(updated_book.author) - 1);
                strncpy(updated_book.publisher, publisher, sizeof(updated_book.publisher) - 1);
                strncpy(updated_book.isbn, isbn, sizeof(updated_book.isbn) - 1);
                updated_book.publish_year = year;
                
                // 计算可借数量的变化
                int borrowed = updated_book.total_count - updated_book.available_count;
                updated_book.total_count = count;
                updated_book.available_count = count - borrowed;
                if (updated_book.available_count < 0) {
                    updated_book.available_count = 0;
                }
                
                // 更新图书
                if (book_update(&updated_book) == 0) {
                    // 刷新图书列表
                    ui_refresh_book_list();
                } else {
                    ui_show_error_dialog(GTK_WINDOW(dialog), "更新失败", "无法更新图书");
                }
            }
        }
    }
    
    // 销毁对话框
    gtk_widget_destroy(dialog);
}

/**
 * @brief 显示添加读者对话框
 */
void ui_show_add_reader_dialog() {
    // 创建对话框
    GtkWidget *dialog = gtk_dialog_new_with_buttons("添加读者",
                                                  GTK_WINDOW(reader_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "取消", GTK_RESPONSE_CANCEL,
                                                  "添加", GTK_RESPONSE_ACCEPT,
                                                  NULL);
    
    // 设置对话框大小
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    
    // 获取内容区域
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);
    
    // 创建表格布局
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // 添加标签和输入框
    GtkWidget *name_label = gtk_label_new("姓名:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    GtkWidget *name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);
    
    GtkWidget *gender_label = gtk_label_new("性别:");
    gtk_grid_attach(GTK_GRID(grid), gender_label, 0, 1, 1, 1);
    GtkWidget *gender_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gender_combo), "男");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gender_combo), "女");
    gtk_combo_box_set_active(GTK_COMBO_BOX(gender_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), gender_combo, 1, 1, 1, 1);
    
    GtkWidget *phone_label = gtk_label_new("电话:");
    gtk_grid_attach(GTK_GRID(grid), phone_label, 0, 2, 1, 1);
    GtkWidget *phone_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), phone_entry, 1, 2, 1, 1);
    
    GtkWidget *email_label = gtk_label_new("邮箱:");
    gtk_grid_attach(GTK_GRID(grid), email_label, 0, 3, 1, 1);
    GtkWidget *email_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), email_entry, 1, 3, 1, 1);
    
    GtkWidget *address_label = gtk_label_new("地址:");
    gtk_grid_attach(GTK_GRID(grid), address_label, 0, 4, 1, 1);
    GtkWidget *address_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), address_entry, 1, 4, 1, 1);
    
    GtkWidget *max_borrow_label = gtk_label_new("最大借阅数量:");
    gtk_grid_attach(GTK_GRID(grid), max_borrow_label, 0, 5, 1, 1);
    GtkWidget *max_borrow_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(max_borrow_entry), "5"); // 默认值
    gtk_grid_attach(GTK_GRID(grid), max_borrow_entry, 1, 5, 1, 1);
    
    // 显示对话框
    gtk_widget_show_all(dialog);
    
    // 运行对话框
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        // 获取输入的数据
        const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const char *gender = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(gender_combo));
        const char *phone = gtk_entry_get_text(GTK_ENTRY(phone_entry));
        const char *email = gtk_entry_get_text(GTK_ENTRY(email_entry));
        const char *address = gtk_entry_get_text(GTK_ENTRY(address_entry));
        const char *max_borrow_str = gtk_entry_get_text(GTK_ENTRY(max_borrow_entry));
        
        // 检查输入
        if (strlen(name) == 0 || strlen(phone) == 0 || strlen(max_borrow_str) == 0) {
            ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "姓名、电话和最大借阅数量为必填项");
        } else {
            // 转换最大借阅数量
            int max_borrow = atoi(max_borrow_str);
            
            if (max_borrow <= 0) {
                ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "最大借阅数量必须大于0");
            } else {
                // 创建读者对象
                Reader reader;
                memset(&reader, 0, sizeof(Reader));
                strncpy(reader.name, name, sizeof(reader.name) - 1);
                strncpy(reader.gender, gender, sizeof(reader.gender) - 1);
                strncpy(reader.phone, phone, sizeof(reader.phone) - 1);
                strncpy(reader.email, email, sizeof(reader.email) - 1);
                strncpy(reader.address, address, sizeof(reader.address) - 1);
                reader.max_borrow_count = max_borrow;
                reader.current_borrow_count = 0;
                
                // 添加读者
                if (reader_add(&reader) == 0) {
                    // 刷新读者列表
                    ui_refresh_reader_list();
                } else {
                    ui_show_error_dialog(GTK_WINDOW(dialog), "添加失败", "无法添加读者");
                }
            }
        }
    }
    
    // 销毁对话框
    gtk_widget_destroy(dialog);
}

/**
 * @brief 显示编辑读者对话框
 * @param reader 要编辑的读者
 */
void ui_show_edit_reader_dialog(Reader *reader) {
    if (reader == NULL) {
        return;
    }
    
    // 创建对话框
    GtkWidget *dialog = gtk_dialog_new_with_buttons("编辑读者",
                                                  GTK_WINDOW(reader_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "取消", GTK_RESPONSE_CANCEL,
                                                  "保存", GTK_RESPONSE_ACCEPT,
                                                  NULL);
    
    // 设置对话框大小
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    
    // 获取内容区域
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);
    
    // 创建表格布局
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // 添加标签和输入框
    GtkWidget *id_label = gtk_label_new("ID:");
    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(id_entry), reader->id);
    gtk_editable_set_editable(GTK_EDITABLE(id_entry), FALSE);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    
    GtkWidget *name_label = gtk_label_new("姓名:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    GtkWidget *name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry), reader->name);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    
    GtkWidget *gender_label = gtk_label_new("性别:");
    gtk_grid_attach(GTK_GRID(grid), gender_label, 0, 2, 1, 1);
    GtkWidget *gender_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gender_combo), "男");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gender_combo), "女");
    if (strcmp(reader->gender, "女") == 0) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(gender_combo), 1);
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(gender_combo), 0);
    }
    gtk_grid_attach(GTK_GRID(grid), gender_combo, 1, 2, 1, 1);
    
    GtkWidget *phone_label = gtk_label_new("电话:");
    gtk_grid_attach(GTK_GRID(grid), phone_label, 0, 3, 1, 1);
    GtkWidget *phone_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(phone_entry), reader->phone);
    gtk_grid_attach(GTK_GRID(grid), phone_entry, 1, 3, 1, 1);
    
    GtkWidget *email_label = gtk_label_new("邮箱:");
    gtk_grid_attach(GTK_GRID(grid), email_label, 0, 4, 1, 1);
    GtkWidget *email_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(email_entry), reader->email);
    gtk_grid_attach(GTK_GRID(grid), email_entry, 1, 4, 1, 1);
    
    GtkWidget *address_label = gtk_label_new("地址:");
    gtk_grid_attach(GTK_GRID(grid), address_label, 0, 5, 1, 1);
    GtkWidget *address_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(address_entry), reader->address);
    gtk_grid_attach(GTK_GRID(grid), address_entry, 1, 5, 1, 1);
    
    GtkWidget *max_borrow_label = gtk_label_new("最大借阅数量:");
    gtk_grid_attach(GTK_GRID(grid), max_borrow_label, 0, 6, 1, 1);
    GtkWidget *max_borrow_entry = gtk_entry_new();
    char max_borrow_str[16];
    snprintf(max_borrow_str, sizeof(max_borrow_str), "%d", reader->max_borrow_count);
    gtk_entry_set_text(GTK_ENTRY(max_borrow_entry), max_borrow_str);
    gtk_grid_attach(GTK_GRID(grid), max_borrow_entry, 1, 6, 1, 1);
    
    // 显示对话框
    gtk_widget_show_all(dialog);
    
    // 运行对话框
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        // 获取输入的数据
        const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const char *gender = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(gender_combo));
        const char *phone = gtk_entry_get_text(GTK_ENTRY(phone_entry));
        const char *email = gtk_entry_get_text(GTK_ENTRY(email_entry));
        const char *address = gtk_entry_get_text(GTK_ENTRY(address_entry));
        const char *max_borrow_str = gtk_entry_get_text(GTK_ENTRY(max_borrow_entry));
        
        // 检查输入
        if (strlen(name) == 0 || strlen(phone) == 0 || strlen(max_borrow_str) == 0) {
            ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "姓名、电话和最大借阅数量为必填项");
        } else {
            // 转换最大借阅数量
            int max_borrow = atoi(max_borrow_str);
            
            if (max_borrow <= 0) {
                ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "最大借阅数量必须大于0");
            } else if (max_borrow < reader->current_borrow_count) {
                ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "最大借阅数量不能小于当前借阅数量");
            } else {
                // 更新读者对象
                Reader updated_reader = *reader;
                strncpy(updated_reader.name, name, sizeof(updated_reader.name) - 1);
                strncpy(updated_reader.gender, gender, sizeof(updated_reader.gender) - 1);
                strncpy(updated_reader.phone, phone, sizeof(updated_reader.phone) - 1);
                strncpy(updated_reader.email, email, sizeof(updated_reader.email) - 1);
                strncpy(updated_reader.address, address, sizeof(updated_reader.address) - 1);
                updated_reader.max_borrow_count = max_borrow;
                
                // 更新读者
                if (reader_update(&updated_reader) == 0) {
                    // 刷新读者列表
                    ui_refresh_reader_list();
                } else {
                    ui_show_error_dialog(GTK_WINDOW(dialog), "更新失败", "无法更新读者");
                }
            }
        }
    }
    
    // 销毁对话框
    gtk_widget_destroy(dialog);
}

/**
 * @brief 显示借书对话框
 */
void ui_show_borrow_book_dialog() {
    // 创建对话框
    GtkWidget *dialog = gtk_dialog_new_with_buttons("借书",
                                                  GTK_WINDOW(borrow_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "取消", GTK_RESPONSE_CANCEL,
                                                  "借阅", GTK_RESPONSE_ACCEPT,
                                                  NULL);
    
    // 设置对话框大小
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);
    
    // 获取内容区域
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);
    
    // 创建表格布局
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // 添加标签和输入框
    GtkWidget *reader_id_label = gtk_label_new("读者ID:");
    gtk_grid_attach(GTK_GRID(grid), reader_id_label, 0, 0, 1, 1);
    GtkWidget *reader_id_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), reader_id_entry, 1, 0, 1, 1);
    
    GtkWidget *book_id_label = gtk_label_new("图书ID:");
    gtk_grid_attach(GTK_GRID(grid), book_id_label, 0, 1, 1, 1);
    GtkWidget *book_id_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), book_id_entry, 1, 1, 1, 1);
    
    GtkWidget *days_label = gtk_label_new("借阅天数:");
    gtk_grid_attach(GTK_GRID(grid), days_label, 0, 2, 1, 1);
    GtkWidget *days_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(days_entry), "30");
    gtk_grid_attach(GTK_GRID(grid), days_entry, 1, 2, 1, 1);
    
    // 显示对话框
    gtk_widget_show_all(dialog);
    
    // 运行对话框
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        // 获取输入的数据
        const char *reader_id = gtk_entry_get_text(GTK_ENTRY(reader_id_entry));
        const char *book_id = gtk_entry_get_text(GTK_ENTRY(book_id_entry));
        const char *days_str = gtk_entry_get_text(GTK_ENTRY(days_entry));
        
        // 检查输入
        if (strlen(reader_id) == 0 || strlen(book_id) == 0 || strlen(days_str) == 0) {
            ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "所有字段都为必填项");
        } else {
            // 转换借阅天数
            int days = atoi(days_str);
            
            if (days <= 0) {
                ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "借阅天数必须大于0");
            } else {
                // 查找读者
                Reader reader;
                if (reader_find_by_id(reader_id, &reader) != 0) {
                    ui_show_error_dialog(GTK_WINDOW(dialog), "借阅失败", "找不到指定的读者");
                } else if (reader.current_borrow_count >= reader.max_borrow_count) {
                    ui_show_error_dialog(GTK_WINDOW(dialog), "借阅失败", "该读者已达到最大借阅数量");
                } else {
                    // 查找图书
                    Book book;
                    if (book_find_by_id(book_id, &book) != 0) {
                        ui_show_error_dialog(GTK_WINDOW(dialog), "借阅失败", "找不到指定的图书");
                    } else if (book.available_count <= 0) {
                        ui_show_error_dialog(GTK_WINDOW(dialog), "借阅失败", "该图书已无可借数量");
                    } else {
                        // 创建借阅记录
                        BorrowRecord record;
                        memset(&record, 0, sizeof(BorrowRecord));
                        strncpy(record.reader_id, reader_id, sizeof(record.reader_id) - 1);
                        strncpy(record.book_id, book_id, sizeof(record.book_id) - 1);
                        record.borrow_date = get_current_time();
                        record.due_date = record.borrow_date + days * 24 * 60 * 60; // 转换为秒
                        record.status = BORROW_STATUS_BORROWED;
                        
                        // 借阅图书
                        if (borrow_book(book_id, reader_id, &record) == 0) {
                            // 刷新借阅列表
                            ui_refresh_borrow_list();
                            // 刷新图书列表
                            ui_refresh_book_list();
                            // 刷新读者列表
                            ui_refresh_reader_list();
                        } else {
                            ui_show_error_dialog(GTK_WINDOW(dialog), "借阅失败", "无法借阅图书");
                        }
                    }
                }
            }
        }
    }
    
    // 销毁对话框
    gtk_widget_destroy(dialog);
}

/**
 * @brief 显示还书对话框
 */
void ui_show_return_book_dialog() {
    // 获取选中的借阅记录
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(borrow_list_view));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        ui_show_error_dialog(GTK_WINDOW(borrow_window), "错误", "请先选择一条借阅记录");
        return;
    }
    
    // 获取借阅ID
    gchar *borrow_id;
    gchar *status;
    gtk_tree_model_get(model, &iter, 0, &borrow_id, 8, &status, -1);
    
    // 检查状态
    if (strcmp(status, "已归还") == 0) {
        ui_show_error_dialog(GTK_WINDOW(borrow_window), "错误", "该图书已归还");
        g_free(borrow_id);
        g_free(status);
        return;
    }
    
    // 确认还书
    gboolean confirm = ui_show_confirm_dialog(GTK_WINDOW(borrow_window), "确认", "确定要归还这本图书吗？");
    if (!confirm) {
        g_free(borrow_id);
        g_free(status);
        return;
    }
    
    // 查找借阅记录
    BorrowRecord record;
    if (borrow_find_by_id(borrow_id, &record) != 0) {
        ui_show_error_dialog(GTK_WINDOW(borrow_window), "错误", "找不到借阅记录");
        g_free(borrow_id);
        g_free(status);
        return;
    }
    
    // 归还图书
        if (return_book(borrow_id) == 0) {
            // 刷新借阅列表
            ui_refresh_borrow_list();
            // 刷新图书列表
            ui_refresh_book_list();
            // 刷新读者列表
            ui_refresh_reader_list();
            
            ui_show_message_dialog(GTK_WINDOW(borrow_window), GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "成功", "图书归还成功");
        } else {
            ui_show_error_dialog(GTK_WINDOW(borrow_window), "错误", "图书归还失败");
        }
    
    g_free(borrow_id);
    g_free(status);
}

/**
 * @brief 显示续借对话框
 */
void ui_show_renew_book_dialog() {
    // 获取选中的借阅记录
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(borrow_list_view));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        ui_show_error_dialog(GTK_WINDOW(borrow_window), "错误", "请先选择一条借阅记录");
        return;
    }
    
    // 获取借阅ID
    gchar *borrow_id;
    gchar *status;
    gtk_tree_model_get(model, &iter, 0, &borrow_id, 8, &status, -1);
    
    // 检查状态
    if (strcmp(status, "已归还") == 0) {
        ui_show_error_dialog(GTK_WINDOW(borrow_window), "错误", "该图书已归还，无法续借");
        g_free(borrow_id);
        g_free(status);
        return;
    }
    
    // 创建对话框
    GtkWidget *dialog = gtk_dialog_new_with_buttons("续借",
                                                  GTK_WINDOW(borrow_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "取消", GTK_RESPONSE_CANCEL,
                                                  "续借", GTK_RESPONSE_ACCEPT,
                                                  NULL);
    
    // 设置对话框大小
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 150);
    
    // 获取内容区域
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);
    
    // 创建表格布局
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // 添加标签和输入框
    GtkWidget *days_label = gtk_label_new("续借天数:");
    gtk_grid_attach(GTK_GRID(grid), days_label, 0, 0, 1, 1);
    GtkWidget *days_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(days_entry), "30");
    gtk_grid_attach(GTK_GRID(grid), days_entry, 1, 0, 1, 1);
    
    // 显示对话框
    gtk_widget_show_all(dialog);
    
    // 运行对话框
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        // 获取输入的数据
        const char *days_str = gtk_entry_get_text(GTK_ENTRY(days_entry));
        
        // 检查输入
        if (strlen(days_str) == 0) {
            ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "续借天数为必填项");
        } else {
            // 转换续借天数
            int days = atoi(days_str);
            
            if (days <= 0) {
                ui_show_error_dialog(GTK_WINDOW(dialog), "输入错误", "续借天数必须大于0");
            } else {
                // 续借图书
                if (renew_book(borrow_id, days) == 0) {
                    // 刷新借阅列表
                    ui_refresh_borrow_list();
                    
                    ui_show_message_dialog(GTK_WINDOW(borrow_window), GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "成功", "图书续借成功");
                } else {
                    ui_show_error_dialog(GTK_WINDOW(dialog), "续借失败", "无法续借图书");
                }
            }
        }
    }
    
    // 销毁对话框
    gtk_widget_destroy(dialog);
    
    g_free(borrow_id);
    g_free(status);
}

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
                          const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                              GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                              message_type,
                                              buttons_type,
                                              "%s", message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return result;
}

/**
 * @brief 显示确认对话框
 * @param parent 父窗口
 * @param title 标题
 * @param message 消息
 * @return 确认返回TRUE，取消返回FALSE
 */
gboolean ui_show_confirm_dialog(GtkWindow *parent, const char *title, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                              GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                              GTK_MESSAGE_QUESTION,
                                              GTK_BUTTONS_YES_NO,
                                              "%s", message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    return result == GTK_RESPONSE_YES;
}

/**
 * @brief 显示错误对话框
 * @param parent 父窗口
 * @param title 标题
 * @param message 错误消息
 */
void ui_show_error_dialog(GtkWindow *parent, const char *title, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                              GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                              GTK_MESSAGE_ERROR,
                                              GTK_BUTTONS_OK,
                                              "%s", message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/**
 * @brief 主窗口销毁回调函数
 */
static void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

/**
 * @brief 图书管理窗口销毁回调函数
 */
static void on_book_window_destroy(GtkWidget *widget, gpointer data) {
    book_window = NULL;
}

/**
 * @brief 读者管理窗口销毁回调函数
 */
static void on_reader_window_destroy(GtkWidget *widget, gpointer data) {
    reader_window = NULL;
}

/**
 * @brief 借阅管理窗口销毁回调函数
 */
static void on_borrow_window_destroy(GtkWidget *widget, gpointer data) {
    borrow_window = NULL;
}

/**
 * @brief 图书管理按钮点击回调函数
 */
static void on_book_management_clicked(GtkWidget *widget, gpointer data) {
    ui_show_book_management_window();
}

/**
 * @brief 读者管理按钮点击回调函数
 */
static void on_reader_management_clicked(GtkWidget *widget, gpointer data) {
    ui_show_reader_management_window();
}

/**
 * @brief 借阅管理按钮点击回调函数
 */
static void on_borrow_management_clicked(GtkWidget *widget, gpointer data) {
    ui_show_borrow_management_window();
}

/**
 * @brief 添加图书按钮点击回调函数
 */
static void on_add_book_clicked(GtkWidget *widget, gpointer data) {
    ui_show_add_book_dialog();
}

/**
 * @brief 编辑图书按钮点击回调函数
 */
static void on_edit_book_clicked(GtkWidget *widget, gpointer data) {
    // 获取选中的图书
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(book_list_view));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        ui_show_error_dialog(GTK_WINDOW(book_window), "错误", "请先选择一本图书");
        return;
    }
    
    // 获取图书ID
    gchar *book_id;
    gtk_tree_model_get(model, &iter, 0, &book_id, -1);
    
    // 查找图书
    Book book;
    if (book_find_by_id(book_id, &book) == 0) {
        ui_show_edit_book_dialog(&book);
    } else {
        ui_show_error_dialog(GTK_WINDOW(book_window), "错误", "找不到图书");
    }
    
    g_free(book_id);
}

/**
 * @brief 删除图书按钮点击回调函数
 */
static void on_delete_book_clicked(GtkWidget *widget, gpointer data) {
    // 获取选中的图书
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(book_list_view));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        ui_show_error_dialog(GTK_WINDOW(book_window), "错误", "请先选择一本图书");
        return;
    }
    
    // 获取图书ID
    gchar *book_id;
    gtk_tree_model_get(model, &iter, 0, &book_id, -1);
    
    // 确认删除
    gboolean confirm = ui_show_confirm_dialog(GTK_WINDOW(book_window), "确认", "确定要删除这本图书吗？");
    if (!confirm) {
        g_free(book_id);
        return;
    }
    
    // 删除图书
    if (book_delete(book_id) == 0) {
        // 刷新图书列表
        ui_refresh_book_list();
    } else {
        ui_show_error_dialog(GTK_WINDOW(book_window), "错误", "无法删除图书，可能有未归还的借阅记录");
    }
    
    g_free(book_id);
}

/**
 * @brief 添加读者按钮点击回调函数
 */
static void on_add_reader_clicked(GtkWidget *widget, gpointer data) {
    ui_show_add_reader_dialog();
}

/**
 * @brief 编辑读者按钮点击回调函数
 */
static void on_edit_reader_clicked(GtkWidget *widget, gpointer data) {
    // 获取选中的读者
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(reader_list_view));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        ui_show_error_dialog(GTK_WINDOW(reader_window), "错误", "请先选择一位读者");
        return;
    }
    
    // 获取读者ID
    gchar *reader_id;
    gtk_tree_model_get(model, &iter, 0, &reader_id, -1);
    
    // 查找读者
    Reader reader;
    if (reader_find_by_id(reader_id, &reader) == 0) {
        ui_show_edit_reader_dialog(&reader);
    } else {
        ui_show_error_dialog(GTK_WINDOW(reader_window), "错误", "找不到读者");
    }
    
    g_free(reader_id);
}

/**
 * @brief 删除读者按钮点击回调函数
 */
static void on_delete_reader_clicked(GtkWidget *widget, gpointer data) {
    // 获取选中的读者
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(reader_list_view));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        ui_show_error_dialog(GTK_WINDOW(reader_window), "错误", "请先选择一位读者");
        return;
    }
    
    // 获取读者ID
    gchar *reader_id;
    gtk_tree_model_get(model, &iter, 0, &reader_id, -1);
    
    // 确认删除
    gboolean confirm = ui_show_confirm_dialog(GTK_WINDOW(reader_window), "确认", "确定要删除这位读者吗？");
    if (!confirm) {
        g_free(reader_id);
        return;
    }
    
    // 删除读者
    if (reader_delete(reader_id) == 0) {
        // 刷新读者列表
        ui_refresh_reader_list();
    } else {
        ui_show_error_dialog(GTK_WINDOW(reader_window), "错误", "无法删除读者，可能有未归还的借阅记录");
    }
    
    g_free(reader_id);
}

/**
 * @brief 借书按钮点击回调函数
 */
static void on_borrow_book_clicked(GtkWidget *widget, gpointer data) {
    ui_show_borrow_book_dialog();
}

/**
 * @brief 还书按钮点击回调函数
 */
static void on_return_book_clicked(GtkWidget *widget, gpointer data) {
    ui_show_return_book_dialog();
}

/**
 * @brief 续借按钮点击回调函数
 */
static void on_renew_book_clicked(GtkWidget *widget, gpointer data) {
    ui_show_renew_book_dialog();
}

/**
 * @brief 刷新图书列表
 */
void ui_refresh_book_list() {
    if (book_list_store == NULL) {
        return;
    }
    
    // 清空列表
    gtk_list_store_clear(book_list_store);
    
    // 获取所有图书
    Book books[100]; // 假设最多100本图书
    int count = 0;
    count = book_get_all(books, 100);
    
    // 添加到列表
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(book_list_store, &iter);
        gtk_list_store_set(book_list_store, &iter,
                          0, books[i].id,
                          1, books[i].title,
                          2, books[i].author,
                          3, books[i].publisher,
                          4, books[i].isbn,
                          5, books[i].available_count > 0 ? "可借" : "已借出",
                          -1);
    }
    
    // 不需要释放内存，因为books是栈上分配的数组
}

/**
 * @brief 刷新读者列表
 */
void ui_refresh_reader_list() {
    if (reader_list_store == NULL) {
        return;
    }
    
    // 清空列表
    gtk_list_store_clear(reader_list_store);
    
    // 获取所有读者
    Reader readers[100]; // 假设最多100位读者
    int count = 0;
    count = reader_get_all(readers, 100);
    
    // 添加到列表
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(reader_list_store, &iter);
        gtk_list_store_set(reader_list_store, &iter,
                          0, readers[i].id,
                          1, readers[i].name,
                          2, readers[i].phone,
                          -1);
    }
    
    // 不需要释放内存，因为readers是栈上分配的数组
}

/**
 * @brief 刷新借阅列表
 */
void ui_refresh_borrow_list() {
    if (borrow_list_store == NULL) {
        return;
    }
    
    // 清空列表
    gtk_list_store_clear(borrow_list_store);
    
    // 获取所有借阅记录
    BorrowRecord records[100]; // 假设最多100条记录
    int count = 0;
    count = borrow_get_all(records, 100);
    
    // 添加到列表
    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(borrow_list_store, &iter);
        
        // 获取图书信息
        Book book;
        book_find_by_id(records[i].book_id, &book);
        
        // 获取读者信息
        Reader reader;
        reader_find_by_id(records[i].reader_id, &reader);
        
        // 转换时间戳为字符串
        char borrow_date[64] = {0};
        char due_date[64] = {0};
        char return_date[64] = {0};
        
        time_to_string(records[i].borrow_date, borrow_date, sizeof(borrow_date), "%Y-%m-%d");
        time_to_string(records[i].due_date, due_date, sizeof(due_date), "%Y-%m-%d");
        
        if (records[i].status == BORROW_STATUS_RETURNED) {
            time_to_string(records[i].return_date, return_date, sizeof(return_date), "%Y-%m-%d");
        } else {
            strcpy(return_date, "未归还");
        }
        
        // 状态文本
        const char *status_text = "";
        switch (records[i].status) {
            case BORROW_STATUS_BORROWED:
                status_text = "借出";
                break;
            case BORROW_STATUS_RETURNED:
                status_text = "已归还";
                break;
            case BORROW_STATUS_OVERDUE:
                status_text = "逾期";
                break;
            case BORROW_STATUS_RENEWED:
                status_text = "已续借";
                break;
            default:
                status_text = "未知";
                break;
        }
        
        gtk_list_store_set(borrow_list_store, &iter,
                          0, records[i].id,
                          1, book.title,
                          2, reader.name,
                          3, borrow_date,
                          4, due_date,
                          5, return_date,
                          6, status_text,
                          -1);
    }
    
    // 不需要释放内存，因为records是栈上分配的数组
}