#!/bin/bash
#===============================================================================
# Script Name: env_manager.sh
# Description: Environment Variables Manager
# Author: Solution
#===============================================================================

set -euo pipefail

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

#===============================================================================
# Functions
#===============================================================================

usage() {
    cat << EOF
Environment Variables Manager

Usage:
  $0 -l                 List all environment variables (sorted)
  $0 -g <VAR>           Get value of a variable
  $0 -s <VAR=value>     Set and export a variable
  $0 -d <VAR>           Delete (unset) a variable
  $0 -p                 Analyze PATH variable
  $0 -c <file1> <file2> Compare environment from two files
  $0 -o <filename>      Export all env vars to file
  $0 -e <filename>      Generate cross-compilation env module
  $0 -h                 Show help

Examples:
  $0 -l
  $0 -g PATH
  $0 -s "MY_VAR=hello"
  $0 -p
  $0 -o backup_env.txt
EOF
    exit 0
}

# 1. List all env vars sorted
list_all() {
    echo -e "\n${CYAN}Environment Variables (sorted)${NC}"
    echo -e "${YELLOW}Total: $(printenv | wc -l) variables${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    local count=0
    printenv | sort | while IFS='=' read -r key value; do
        # Kiểm tra nếu value chứa : (PATH-like) thì hiển thị gọn
        if [[ "$value" == *:* ]]; then
            printf "  %-30s = %s\n" "$key" "${value:0:60}..."
        else
            printf "  %-30s = %s\n" "$key" "$value"
        fi
        ((count++))
    done
}

# 2. Get value of a variable
get_value() {
    local var_name="$1"
    if [ -z "$var_name" ]; then
        echo -e "${RED}[ERROR]${NC} Variable name is required" >&2
        return 1
    fi

    if [ -z "${!var_name+x}" ]; then
        echo -e "${YELLOW}[INFO]${NC} Variable '$var_name' is not set"
        return 1
    fi

    local value="${!var_name}"
    echo -e "${CYAN}$var_name${NC} = $value"

    # Thêm thông tin nếu là PATH-like
    if [[ "$value" == *:* ]]; then
        local IFS=':'
        local count=0
        for segment in $value; do
            ((count++))
        done
        echo "  (Contains $count segments)"
    fi
}

# 3. Set variable
set_variable() {
    local assignment="$1"
    if [ -z "$assignment" ]; then
        echo -e "${RED}[ERROR]${NC} Usage: $0 -s VAR=value" >&2
        return 1
    fi

    if [[ "$assignment" != *"="* ]]; then
        echo -e "${RED}[ERROR]${NC} Format must be VAR=value" >&2
        return 1
    fi

    local var_name="${assignment%%=*}"
    local var_value="${assignment#*=}"

    if [ -z "$var_name" ]; then
        echo -e "${RED}[ERROR]${NC} Variable name cannot be empty" >&2
        return 1
    fi

    # Kiểm tra tên biến hợp lệ
    if ! [[ "$var_name" =~ ^[a-zA-Z_][a-zA-Z0-9_]*$ ]]; then
        echo -e "${RED}[ERROR]${NC} Invalid variable name: '$var_name'" >&2
        return 1
    fi

    export "$var_name=$var_value"
    echo -e "${GREEN}[SET]${NC} $var_name = $var_value"
    echo "  (Environment variable set in current shell only)"
    echo "  To make permanent, add to ~/.bashrc or ~/.profile"
}

# 4. Delete variable
delete_variable() {
    local var_name="$1"
    if [ -z "$var_name" ]; then
        echo -e "${RED}[ERROR]${NC} Variable name is required" >&2
        return 1
    fi

    if [ -z "${!var_name+x}" ]; then
        echo -e "${YELLOW}[INFO]${NC} Variable '$var_name' is not set, nothing to delete"
        return 0
    fi

    local old_value="${!var_name}"
    unset "$var_name"
    echo -e "${GREEN}[DELETED]${NC} $var_name (was: $old_value)"
}

# 5. Analyze PATH
analyze_path() {
    echo -e "\n${CYAN}PATH Analysis${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    if [ -z "${PATH+x}" ]; then
        echo -e "${RED}[ERROR]${NC} PATH is not set!" >&2
        return 1
    fi

    local IFS=':'
    local dirs=($PATH)
    local total=${#dirs[@]}
    local existing=0 missing=0

    echo "  PATH has $total directories"
    echo

    local index=0
    for dir in "${dirs[@]}"; do
        ((index++))
        if [ -d "$dir" ]; then
            echo -e "  [${GREEN}OK${NC}] $dir"
            ((existing++))
        else
            echo -e "  [${RED}MISSING${NC}] $dir"
            ((missing++))
        fi
    done

    echo
    echo "  Summary: $existing existing, $missing missing directories"
    echo

    # Phát hiện duplicates
    echo -e "${YELLOW}Duplicate Check:${NC}"
    local seen=()
    for dir in "${dirs[@]}"; do
        local dup=false
        for s in "${seen[@]}"; do
            if [ "$s" = "$dir" ]; then
                dup=true
                break
            fi
        done
        if $dup; then
            echo -e "  ${YELLOW}[DUP]${NC} $dir"
        fi
        seen+=("$dir")
    done
}

# 6. Compare env files
compare_env() {
    local file1="$1"
    local file2="$2"

    if [ -z "$file1" ] || [ -z "$file2" ]; then
        echo -e "${RED}[ERROR]${NC} Usage: $0 -c <file1> <file2>" >&2
        return 1
    fi

    if [ ! -f "$file1" ]; then
        echo -e "${RED}[ERROR]${NC} File not found: $file1" >&2
        return 1
    fi
    if [ ! -f "$file2" ]; then
        echo -e "${RED}[ERROR]${NC} File not found: $file2" >&2
        return 1
    fi

    echo -e "\n${CYAN}Comparing: $file1 vs $file2${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    # Chỉ lấy các dòng có KEY=VALUE (bỏ comment và empty lines)
    local vars1 vars2
    vars1=$(grep -E '^[a-zA-Z_][a-zA-Z0-9_]*=' "$file1" | sort)
    vars2=$(grep -E '^[a-zA-Z_][a-zA-Z0-9_]*=' "$file2" | sort)

    # Lấy danh sách keys
    local keys1 keys2
    keys1=$(echo "$vars1" | cut -d= -f1)
    keys2=$(echo "$vars2" | cut -d= -f1)

    # Keys chỉ có trong file1
    echo -e "  ${GREEN}Only in $file1:${NC}"
    local found=false
    while IFS= read -r key; do
        if ! echo "$keys2" | grep -qx "$key"; then
            local val
            val=$(echo "$vars1" | grep "^$key=" | cut -d= -f2-)
            echo "    $key=$val"
            found=true
        fi
    done <<< "$keys1"
    $found || echo "    (none)"

    # Keys chỉ có trong file2
    echo -e "  ${RED}Only in $file2:${NC}"
    found=false
    while IFS= read -r key; do
        if ! echo "$keys1" | grep -qx "$key"; then
            local val
            val=$(echo "$vars2" | grep "^$key=" | cut -d= -f2-)
            echo "    $key=$val"
            found=true
        fi
    done <<< "$keys2"
    $found || echo "    (none)"

    # Keys khác giá trị
    echo -e "  ${YELLOW}Different values:${NC}"
    found=false
    while IFS= read -r key; do
        if echo "$keys2" | grep -qx "$key"; then
            local val1 val2
            val1=$(echo "$vars1" | grep "^$key=" | cut -d= -f2-)
            val2=$(echo "$vars2" | grep "^$key=" | cut -d= -f2-)
            if [ "$val1" != "$val2" ]; then
                echo "    $key:"
                echo "      $file1: $val1"
                echo "      $file2: $val2"
                found=true
            fi
        fi
    done <<< "$keys1"
    $found || echo "    (all match)"
}

# 7. Export all env vars to file
export_to_file() {
    local filename="$1"
    if [ -z "$filename" ]; then
        echo -e "${RED}[ERROR]${NC} Filename is required" >&2
        return 1
    fi

    if [ -f "$filename" ]; then
        echo -e "${YELLOW}[WARNING]${NC} File '$filename' already exists. Overwrite? (y/N): "
        read -r confirm
        if [ "$confirm" != "y" ] && [ "$confirm" != "Y" ]; then
            echo "Export cancelled."
            return 0
        fi
    fi

    echo "# Environment Variables Export" > "$filename"
    echo "# Generated: $(date)" >> "$filename"
    echo "# Host: $(hostname)" >> "$filename"
    echo "# User: $(whoami)" >> "$filename"
    echo >> "$filename"

    printenv | sort | while IFS='=' read -r key value; do
        # Escape special characters for shell
        local escaped="${value//\"/\\\"}"
        echo "export $key=\"$escaped\"" >> "$filename"
    done

    echo -e "${GREEN}[SUCCESS]${NC} Exported $(printenv | wc -l) variables to '$filename'"
    echo "  To load: source $filename"
}

# Bonus: Generate cross-compilation env module
generate_cross_env() {
    local filename="${1:-cross-compile.env}"
    if [ -z "$1" ]; then
        echo -e "${YELLOW}[INFO]${NC} Using default filename: $filename"
    fi

    cat > "$filename" << 'EOF'
#===============================================================================
# Cross-compilation Environment for Embedded Linux
# Source this file to setup cross-compilation environment
# Usage: source cross-compile.env
#===============================================================================

# Target architecture
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

# Toolchain
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}g++
export AR=${CROSS_COMPILE}ar
export AS=${CROSS_COMPILE}as
export LD=${CROSS_COMPILE}ld
export STRIP=${CROSS_COMPILE}strip
export OBJCOPY=${CROSS_COMPILE}objcopy
export OBJDUMP=${CROSS_COMPILE}objdump

# Compiler flags
export CFLAGS="-O2 -march=armv7-a -mfpu=neon -mfloat-abi=hard"
export CXXFLAGS="${CFLAGS}"
export LDFLAGS="-L${PWD}/lib"

# Paths
export SYSROOT=${PWD}/sysroot
export STAGING_DIR=${PWD}/staging
export PKG_CONFIG_PATH="${SYSROOT}/usr/lib/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="${SYSROOT}"

# Header search paths
export CFLAGS="${CFLAGS} -I${SYSROOT}/usr/include"

# Library search paths
export LDFLAGS="${LDFLAGS} -L${SYSROOT}/usr/lib"

# Number of parallel jobs for make
export MAKEFLAGS="-j$(nproc)"

echo "Cross-compilation environment loaded:"
echo "  Target    : ${ARCH}"
echo "  Toolchain : ${CROSS_COMPILE}"
echo "  Sysroot   : ${SYSROOT}"
echo "  Jobs      : $(nproc)"
EOF

    echo -e "${GREEN}[SUCCESS]${NC} Created cross-compilation env file: $filename"
    echo "  To use: source $filename"
}

#===============================================================================
# Main
#===============================================================================

if [ $# -eq 0 ]; then
    usage
fi

case "${1:-}" in
    -l)
        list_all
        ;;
    -g)
        get_value "${2:-}"
        ;;
    -s)
        set_variable "${2:-}"
        ;;
    -d)
        delete_variable "${2:-}"
        ;;
    -p)
        analyze_path
        ;;
    -c)
        compare_env "${2:-}" "${3:-}"
        ;;
    -o)
        export_to_file "${2:-}"
        ;;
    -e)
        generate_cross_env "${2:-}"
        ;;
    -h|--help)
        usage
        ;;
    *)
        echo -e "${RED}[ERROR]${NC} Unknown option: ${1:-}" >&2
        usage
        ;;
esac
