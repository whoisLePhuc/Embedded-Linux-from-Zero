#!/bin/bash
#===============================================================================
# Script Name: file_manager.sh
# Description: File management utility với nhiều chức năng
# Author: Solution
#===============================================================================

set -euo pipefail

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

#===============================================================================
# Function Definitions
#===============================================================================

# Hiển thị hướng dẫn
usage() {
    cat << EOF
File Manager Script

Usage:
  $0 -l <directory>          List all files with details
  $0 -s <pattern> <dir>      Search files by pattern
  $0 -c <directory>          Count files by type
  $0 -t <directory>          Cleanup temporary files
  $0 -T <directory>          Show directory tree (2 levels)
  $0 -o <old> <new> <dir>    Batch rename files
  $0 -h                      Show this help

Examples:
  $0 -l /tmp
  $0 -s ".txt" /home
  $0 -c ./project
  $0 -t /tmp
  $0 -T /var/log
  $0 -o ".bak" ".old" ./config
EOF
    exit 0
}

# Kiểm tra thư mục tồn tại
check_dir() {
    local dir="$1"
    if [ ! -d "$dir" ]; then
        echo -e "${RED}[ERROR]${NC} Directory '$dir' does not exist or is not accessible" >&2
        return 1
    fi
    if [ ! -r "$dir" ]; then
        echo -e "${RED}[ERROR]${NC} No read permission for '$dir'" >&2
        return 1
    fi
    return 0
}

# 1. List files
list_files() {
    local dir="${1:-.}"
    check_dir "$dir" || return 1

    echo -e "${CYAN}Contents of: $(realpath "$dir")${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    local count=0
    for item in "$dir"/* "$dir"/.*; do
        # Skip . and ..
        [ "$(basename "$item")" = "." ] && continue
        [ "$(basename "$item")" = ".." ] && continue

        if [ -e "$item" ]; then
            local type_str=""
            local size_str=""
            local name
            name=$(basename "$item")

            if [ -L "$item" ]; then
                type_str="${BLUE}LINK${NC}"
                target=$(readlink "$item")
                name="$name -> $target"
                size_str=$(stat -c%s "$item" 2>/dev/null || echo "0")
            elif [ -d "$item" ]; then
                type_str="${YELLOW}DIR ${NC}"
                size_str=$(stat -c%s "$item" 2>/dev/null || echo "0")
            elif [ -f "$item" ]; then
                if [ -x "$item" ]; then
                    type_str="${GREEN}EXEC${NC}"
                else
                    type_str="FILE"
                fi
                size_str=$(stat -c%s "$item" 2>/dev/null || echo "0")
            else
                type_str="????"
                size_str="?"
            fi

            printf "  [%s] %-50s %8s bytes\n" "$type_str" "$name" "$size_str"
            ((count++))
        fi
    done

    if [ "$count" -eq 0 ]; then
        echo "  (empty directory)"
    fi
    echo -e "${YELLOW}----------------------------------------${NC}"
    echo "Total: $count items"
}

# 2. Search files
search_files() {
    local pattern="$1"
    local dir="${2:-.}"

    if [ -z "$pattern" ]; then
        echo -e "${RED}[ERROR]${NC} Search pattern is required" >&2
        return 1
    fi

    check_dir "$dir" || return 1

    echo -e "${CYAN}Searching for '$pattern' in: $(realpath "$dir")${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    local count=0
    while IFS= read -r -d '' file; do
        if [ -f "$file" ]; then
            local size
            size=$(stat -c%s "$file" 2>/dev/null || echo "?")
            local human_size
            if [ "$size" -ge 1048576 ]; then
                human_size=$(awk "BEGIN {printf \"%.1f MB\", $size/1048576}")
            elif [ "$size" -ge 1024 ]; then
                human_size=$(awk "BEGIN {printf \"%.1f KB\", $size/1024}")
            else
                human_size="${size} B"
            fi
            printf "  %-60s %s\n" "$file" "$human_size"
            ((count++))
        fi
    done < <(find "$dir" -name "*${pattern}*" -print0 2>/dev/null)

    echo -e "${YELLOW}----------------------------------------${NC}"
    echo "Found: $count file(s)"
}

# 3. Count by type
count_by_type() {
    local dir="${1:-.}"
    check_dir "$dir" || return 1

    echo -e "${CYAN}File count by type in: $(realpath "$dir")${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    local reg_count=0 dir_count=0 link_count=0 exec_count=0 total=0

    while IFS= read -r -d '' item; do
        if [ -L "$item" ]; then
            ((link_count++))
        elif [ -d "$item" ]; then
            ((dir_count++))
        elif [ -f "$item" ] && [ -x "$item" ]; then
            ((exec_count++))
        elif [ -f "$item" ]; then
            ((reg_count++))
        fi
        ((total++))
    done < <(find "$dir" -maxdepth 1 -print0 2>/dev/null)

    # Subtract . and ..
    ((dir_count-=2))

    printf "  %-30s : %4d\n" "Regular files" "$reg_count"
    printf "  %-30s : %4d\n" "Directories (1 level)" "$dir_count"
    printf "  %-30s : %4d\n" "Symbolic links" "$link_count"
    printf "  %-30s : %4d\n" "Executables" "$exec_count"
    printf "  %-30s : %4d\n" "Total" "$total"
}

# 4. Cleanup temp files
cleanup_temp() {
    local dir="${1:-.}"
    check_dir "$dir" || return 1

    echo -e "${YELLOW}[WARNING]${NC} This will remove *.tmp, *.log, *.bak files"
    echo -e "Directory: $(realpath "$dir")"
    echo

    # Collect temp files
    local temp_files=()
    while IFS= read -r -d '' file; do
        temp_files+=("$file")
    done < <(find "$dir" -type f \( -name "*.tmp" -o -name "*.log" -o -name "*.bak" \) -print0 2>/dev/null)

    if [ ${#temp_files[@]} -eq 0 ]; then
        echo "No temporary files found."
        return 0
    fi

    echo "Found ${#temp_files[@]} temporary file(s):"
    for file in "${temp_files[@]}"; do
        local size
        size=$(stat -c%s "$file" 2>/dev/null || echo "?")
        printf "  %-60s (%s bytes)\n" "$file" "$size"
    done

    echo
    read -rp "Remove these files? (y/N): " confirm

    if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
        local removed=0
        for file in "${temp_files[@]}"; do
            if rm "$file" 2>/dev/null; then
                echo -e "${GREEN}[DELETED]${NC} $file"
                ((removed++))
            else
                echo -e "${RED}[ERROR]${NC} Cannot delete $file" >&2
            fi
        done
        echo "Removed $removed/${#temp_files[@]} files."
    else
        echo "Cleanup cancelled."
    fi
}

# 5. Show tree (2 levels)
show_tree() {
    local dir="${1:-.}"
    check_dir "$dir" || return 1

    local dirname
    dirname=$(basename "$(realpath "$dir")")
    echo "$dirname"
    echo "|"

    local count=0
    local total=0
    for item in "$dir"/* "$dir"/.*; do
        [ "$(basename "$item")" = "." ] && continue
        [ "$(basename "$item")" = ".." ] && continue
        [ ! -e "$item" ] && continue

        local name
        name=$(basename "$item")
        ((total++))

        # Check if it's the last item
        local is_last=true
        for next in "$dir"/*; do
            if [ "$next" = "$item" ]; then
                continue
            fi
            if [ "$count" -lt "$total" ]; then
                is_last=false
            fi
            break
        done

        if $is_last; then
            echo -n "└── "
        else
            echo -n "├── "
        fi

        if [ -d "$item" ]; then
            echo -e "${YELLOW}$name/${NC}"
            # Level 2
            local sub_count=0 sub_total=0
            for subitem in "$item"/*; do
                [ ! -e "$subitem" ] && continue
                ((sub_total++))
            done
            for subitem in "$item"/*; do
                [ ! -e "$subitem" ] && continue
                ((sub_count++))
                local subname
                subname=$(basename "$subitem")
                if [ "$sub_count" -eq "$sub_total" ]; then
                    if $is_last; then
                        echo "    └── $subname"
                    else
                        echo "    └── $subname"
                    fi
                else
                    if $is_last; then
                        echo "    ├── $subname"
                    else
                        echo "    ├── $subname"
                    fi
                fi
            done
        else
            if [ -L "$item" ]; then
                target=$(readlink "$item")
                echo -e "${BLUE}$name -> $target${NC}"
            elif [ -x "$item" ]; then
                echo -e "${GREEN}$name*${NC}"
            else
                echo "$name"
            fi
        fi

        ((count++))
    done
}

# 6. Batch rename (Bonus)
batch_rename() {
    local old_str="$1"
    local new_str="$2"
    local dir="${3:-.}"

    if [ -z "$old_str" ] || [ -z "$new_str" ]; then
        echo -e "${RED}[ERROR]${NC} Usage: $0 -o <old> <new> <dir>" >&2
        return 1
    fi

    check_dir "$dir" || return 1

    echo -e "${CYAN}Batch rename in: $(realpath "$dir")${NC}"
    echo -e "${YELLOW}Replace: '$old_str' -> '$new_str'${NC}"
    echo

    local count=0
    for item in "$dir"/*; do
        [ ! -e "$item" ] && continue
        local name
        name=$(basename "$item")
        if [[ "$name" == *"$old_str"* ]]; then
            local new_name="${name//$old_str/$new_str}"
            echo "  $name -> $new_name"
            if mv "$item" "$dir/$new_name" 2>/dev/null; then
                ((count++))
            else
                echo -e "${RED}[ERROR]${NC} Cannot rename '$name'" >&2
            fi
        fi
    done

    echo "Renamed $count file(s)."
}

#===============================================================================
# Main
#===============================================================================

# Kiểm tra arguments
if [ $# -eq 0 ]; then
    usage
fi

# Xử lý arguments với getopts
FIRST_ARG="${1:-}"

case "$FIRST_ARG" in
    -l)
        list_files "${2:-.}"
        ;;
    -s)
        search_files "${2:-}" "${3:-.}"
        ;;
    -c)
        count_by_type "${2:-.}"
        ;;
    -t)
        cleanup_temp "${2:-.}"
        ;;
    -T)
        show_tree "${2:-.}"
        ;;
    -o)
        batch_rename "${2:-}" "${3:-}" "${4:-.}"
        ;;
    -h|--help)
        usage
        ;;
    *)
        echo -e "${RED}[ERROR]${NC} Unknown option: $1" >&2
        usage
        ;;
esac
