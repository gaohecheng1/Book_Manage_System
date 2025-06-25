# 图书管理系统Makefile

# 编译器和选项
CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

# 目标文件
TARGET = book_manager

# 源文件
SRCS = main.c book.c reader.c borrow.c utils.c ui.c

# 目标文件
OBJS = $(SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 链接规则
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理规则
clean:
	rm -f $(OBJS) $(TARGET)

# 运行规则
run: $(TARGET)
	./$(TARGET)

# 安装规则
install: $(TARGET)
	mkdir -p $(DESTDIR)/usr/local/bin
	cp $(TARGET) $(DESTDIR)/usr/local/bin/

# 卸载规则
uninstall:
	rm -f $(DESTDIR)/usr/local/bin/$(TARGET)

# 依赖关系
main.o: main.c book.h reader.h borrow.h utils.h ui.h
book.o: book.c book.h utils.h
reader.o: reader.c reader.h utils.h
borrow.o: borrow.c borrow.h book.h reader.h utils.h
utils.o: utils.c utils.h
ui.o: ui.c ui.h book.h reader.h borrow.h utils.h

.PHONY: all clean run install uninstall