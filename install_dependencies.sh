#!/bin/bash

# 图书管理系统依赖安装脚本

# 检测操作系统类型
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux系统
    if [ -f /etc/debian_version ]; then
        # Debian/Ubuntu系统
        echo "检测到Debian/Ubuntu系统，使用apt安装依赖..."
        sudo apt-get update
        sudo apt-get install -y build-essential libgtk-3-dev
    elif [ -f /etc/fedora-release ]; then
        # Fedora系统
        echo "检测到Fedora系统，使用dnf安装依赖..."
        sudo dnf install -y gcc make gtk3-devel
    elif [ -f /etc/redhat-release ]; then
        # CentOS/RHEL系统
        echo "检测到CentOS/RHEL系统，使用yum安装依赖..."
        sudo yum install -y gcc make gtk3-devel
    else
        echo "未能识别的Linux发行版，请手动安装GTK+3开发库"
        exit 1
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS系统
    echo "检测到macOS系统，使用Homebrew安装依赖..."
    
    # 检查是否安装了Homebrew
    if ! command -v brew &> /dev/null; then
        echo "未找到Homebrew，正在安装..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    
    # 安装GTK+3
    brew install gtk+3
elif [[ "$OSTYPE" == "msys"* || "$OSTYPE" == "cygwin"* ]]; then
    # Windows系统（MSYS2或Cygwin环境）
    echo "检测到Windows系统（MSYS2或Cygwin环境）"
    
    if command -v pacman &> /dev/null; then
        # MSYS2环境
        echo "使用MSYS2的pacman安装依赖..."
        pacman -S --noconfirm mingw-w64-x86_64-gtk3 mingw-w64-x86_64-gcc make
    else
        echo "请确保已安装MSYS2并使用以下命令安装依赖："
        echo "pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-gcc make"
        exit 1
    fi
else
    echo "未能识别的操作系统类型：$OSTYPE"
    echo "请手动安装GTK+3开发库"
    exit 1
fi

echo "依赖安装完成！"
echo "现在可以使用'make'命令编译项目了。"