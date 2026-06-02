#!/bin/bash
#===============================================================================
# Script Name: user_group_manager.sh
# Description: Quản lý User & Group trên Linux
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
# Utility Functions
#===============================================================================

usage() {
    cat << EOF
User & Group Manager

Usage:
  $0 -u                    List all users
  $0 -g                    List all groups
  $0 -i <username>         Show user info
  $0 -c <username>         Create new user
  $0 -L <username>         Lock user account
  $0 -U <username>         Unlock user account
  $0 -p <path>             Show file/directory permissions
  $0 --expire <username>   Show account expiry info
  $0 -h                    Show help

Examples:
  $0 -u
  $0 -i john
  $0 -c newuser
  $0 -p /etc/passwd
EOF
    exit 0
}

check_root_warning() {
    if [ "$(id -u)" -ne 0 ]; then
        echo -e "${YELLOW}[WARNING]${NC} Some operations require root privileges." >&2
        echo -e "Run with: ${CYAN}sudo $0 $*${NC}" >&2
        echo >&2
    fi
}

print_table_header() {
    printf "  %-20s %-8s %-8s %-30s %-15s\n" "USERNAME" "UID" "GID" "HOME" "SHELL"
    echo "  $(printf '%.0s-' {1..85})"
}

print_table_row() {
    printf "  %-20s %-8s %-8s %-30s %-15s\n" "$1" "$2" "$3" "$4" "$5"
}

#===============================================================================
# Feature Functions
#===============================================================================

# 1. List all users
list_users() {
    echo -e "\n${CYAN}System Users${NC}"
    echo "  Total: $(cat /etc/passwd | wc -l) users"
    echo
    print_table_header

    while IFS=':' read -r user pass uid gid desc home shell; do
        local uid_num=$((uid))
        if [ "$uid_num" -eq 0 ]; then
            printf "  ${RED}%-20s${NC} %-8s %-8s %-30s %-15s\n" "$user" "$uid" "$gid" "$home" "$shell"
        elif [ "$uid_num" -lt 1000 ]; then
            printf "  ${YELLOW}%-20s${NC} %-8s %-8s %-30s %-15s\n" "$user" "$uid" "$gid" "$home" "$shell"
        else
            print_table_row "$user" "$uid" "$gid" "$home" "$shell"
        fi
    done < /etc/passwd

    echo
    echo -e "  ${RED}red${NC}=root  ${YELLOW}yellow${NC}=system  default=regular"
}

# 2. List all groups
list_groups() {
    echo -e "\n${CYAN}System Groups${NC}"
    echo "  Total: $(cat /etc/group | wc -l) groups"
    echo
    printf "  %-20s %-8s %s\n" "GROUP" "GID" "MEMBERS"
    echo "  $(printf '%.0s-' {1..80})"

    while IFS=':' read -r group pass gid members; do
        local gid_num=$((gid))
        if [ "$gid_num" -eq 0 ]; then
            printf "  ${RED}%-20s${NC} %-8s %s\n" "$group" "$gid" "${members:-(none)}"
        elif [ "$gid_num" -lt 1000 ]; then
            printf "  ${YELLOW}%-20s${NC} %-8s %s\n" "$group" "$gid" "${members:-(none)}"
        else
            printf "  %-20s %-8s %s\n" "$group" "$gid" "${members:-(none)}"
        fi
    done < /etc/group
}

# 3. Show detailed user info
user_info() {
    local username="$1"
    if [ -z "$username" ]; then
        echo -e "${RED}[ERROR]${NC} Username is required" >&2
        return 1
    fi

    # Kiểm tra user tồn tại
    if ! id "$username" &>/dev/null; then
        echo -e "${RED}[ERROR]${NC} User '$username' does not exist" >&2
        return 1
    fi

    echo -e "\n${CYAN}User Information: $username${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    # Thông tin từ /etc/passwd
    local user_info
    user_info=$(grep "^${username}:" /etc/passwd) || true
    if [ -n "$user_info" ]; then
        IFS=':' read -r user pass uid gid desc home shell <<< "$user_info"
        echo "  Username      : $user"
        echo "  UID           : $uid"
        echo "  GID           : $gid"
        echo "  Full Name     : ${desc:-N/A}"
        echo "  Home Directory: $home"
        echo "  Shell         : $shell"
    fi

    # Groups
    local groups
    groups=$(groups "$username" 2>/dev/null | cut -d: -f2)
    echo "  Groups        :$groups"

    # Thông tin từ /etc/shadow (cần root)
    if [ "$(id -u)" -eq 0 ]; then
        local shadow_info
        shadow_info=$(grep "^${username}:" /etc/shadow 2>/dev/null) || true
        if [ -n "$shadow_info" ]; then
            IFS=':' read -r user enc_pwd lastchg min max warn inact expire <<< "$shadow_info"
            local lastchg_date="N/A"
            if [ "$lastchg" -ge 0 ] 2>/dev/null; then
                lastchg_date=$(date -d "1970-01-01 + $lastchg days" '+%Y-%m-%d' 2>/dev/null || echo "N/A")
            fi
            echo "  Password Set  : $lastchg_date"
            echo "  Password Expires: ${max:-(never)} days after change"
            if [ "${enc_pwd:0:1}" = "!" ] || [ "${enc_pwd:0:1}" = "*" ]; then
                echo -e "  Account Status: ${RED}LOCKED${NC}"
            else
                echo -e "  Account Status: ${GREEN}ACTIVE${NC}"
            fi
        fi
    fi

    # Last login
    if command -v lastlog &>/dev/null; then
        local last_login
        last_login=$(lastlog -u "$username" 2>/dev/null | tail -1 | awk '{for(i=3;i<=NF;i++) printf "%s ", $i}')
        echo "  Last Login    : $last_login"
    fi

    # Running processes của user
    local proc_count
    proc_count=$(ps --no-headers -u "$username" 2>/dev/null | wc -l) || proc_count=0
    echo "  Processes     : $proc_count running"
}

# 4. Create new user (cần root)
create_user() {
    local username="$1"
    if [ -z "$username" ]; then
        echo -e "${RED}[ERROR]${NC} Username is required" >&2
        return 1
    fi

    if [ "$(id -u)" -ne 0 ]; then
        echo -e "${RED}[ERROR]${NC} Root privileges required to create users" >&2
        return 1
    fi

    # Kiểm tra user đã tồn tại
    if id "$username" &>/dev/null; then
        echo -e "${RED}[ERROR]${NC} User '$username' already exists" >&2
        return 1
    fi

    # Tạo user
    echo -e "${CYAN}Creating user: $username${NC}"

    # Tạo password ngẫu nhiên
    local password
    password=$(tr -dc 'A-Za-z0-9!@#$%^&*' < /dev/urandom 2>/dev/null | head -c 12 || echo "TempPass123!")

    # Tạo user với home directory và bash shell
    useradd -m -s /bin/bash -c "Created by user_group_manager" "$username" 2>/dev/null || {
        echo -e "${RED}[ERROR]${NC} Failed to create user '$username'" >&2
        return 1
    }

    # Set password
    echo "$username:$password" | chpasswd 2>/dev/null || {
        echo -e "${YELLOW}[WARNING]${NC} Created user but failed to set password" >&2
    }

    # Thêm vào group users
    usermod -aG users "$username" 2>/dev/null || true

    echo -e "${GREEN}[SUCCESS]${NC} User '$username' created successfully"
    echo "  Home Directory: /home/$username"
    echo "  Temporary Password: $password"
    echo "  ${YELLOW}Please change password at first login!${NC}"
}

# 5. Lock/Unlock user
lock_user() {
    local username="$1"
    if [ -z "$username" ]; then
        echo -e "${RED}[ERROR]${NC} Username is required" >&2
        return 1
    fi

    if [ "$(id -u)" -ne 0 ]; then
        echo -e "${RED}[ERROR]${NC} Root privileges required" >&2
        return 1
    fi

    if ! id "$username" &>/dev/null; then
        echo -e "${RED}[ERROR]${NC} User '$username' does not exist" >&2
        return 1
    fi

    if passwd -l "$username" &>/dev/null; then
        echo -e "${GREEN}[SUCCESS]${NC} User '$username' has been LOCKED"
    else
        echo -e "${RED}[ERROR]${NC} Failed to lock user '$username'" >&2
        return 1
    fi
}

unlock_user() {
    local username="$1"
    if [ -z "$username" ]; then
        echo -e "${RED}[ERROR]${NC} Username is required" >&2
        return 1
    fi

    if [ "$(id -u)" -ne 0 ]; then
        echo -e "${RED}[ERROR]${NC} Root privileges required" >&2
        return 1
    fi

    if ! id "$username" &>/dev/null; then
        echo -e "${RED}[ERROR]${NC} User '$username' does not exist" >&2
        return 1
    fi

    if passwd -u "$username" &>/dev/null; then
        echo -e "${GREEN}[SUCCESS]${NC} User '$username' has been UNLOCKED"
    else
        echo -e "${RED}[ERROR]${NC} Failed to unlock user '$username'" >&2
        return 1
    fi
}

# 6. Show permissions
show_permissions() {
    local path="$1"
    if [ -z "$path" ]; then
        echo -e "${RED}[ERROR]${NC} Path is required" >&2
        return 1
    fi

    if [ ! -e "$path" ]; then
        echo -e "${RED}[ERROR]${NC} Path '$path' does not exist" >&2
        return 1
    fi

    echo -e "\n${CYAN}Permissions for: $(realpath "$path")${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    local ls_output
    ls_output=$(ls -ld "$path" 2>/dev/null) || {
        echo -e "${RED}[ERROR]${NC} Cannot access '$path'" >&2
        return 1
    fi

    local perms link_count owner group size date name
    read -r perms link_count owner group size date name <<< "$ls_output"

    echo "  Type & Perms : $perms"
    echo "  Owner        : $owner"
    echo "  Group        : $group"
    echo "  Size         : $size bytes"
    echo "  Links        : $link_count"
    echo "  Modified     : $date $name"

    # Phân tích permissions
    echo
    echo "  Permission Breakdown:"

    local owner_perm="${perms:1:3}"
    local group_perm="${perms:4:3}"
    local other_perm="${perms:7:3}"

    decode_perms() {
        local p="$1"
        local r=0 w=0 x=0
        [[ "${p:0:1}" == "r" ]] && r=4
        [[ "${p:1:1}" == "w" ]] && w=2
        [[ "${p:2:1}" == "x" ]] && x=1
        echo $((r + w + x))
    }

    printf "    Owner : %s (0%d0)\n" "$owner_perm" "$(decode_perms "$owner_perm")"
    printf "    Group : %s (0%d0)\n" "$group_perm" "$(decode_perms "$group_perm")"
    printf "    Other : %s (0%d0)\n" "$other_perm" "$(decode_perms "$other_perm")"

    # Numeric mode
    local numeric_mode
    numeric_mode=$(stat -c "%a" "$path" 2>/dev/null)
    echo "  Numeric Mode : $numeric_mode"

    # ACL (nếu có)
    if command -v getfacl &>/dev/null; then
        echo
        echo "  ACL:"
        getfacl "$path" 2>/dev/null | sed 's/^/    /'
    fi
}

# Bonus: Account expiry info
account_expire() {
    local username="$1"
    if [ -z "$username" ]; then
        echo -e "${RED}[ERROR]${NC} Username is required" >&2
        return 1
    fi

    if ! id "$username" &>/dev/null; then
        echo -e "${RED}[ERROR]${NC} User '$username' does not exist" >&2
        return 1
    fi

    echo -e "\n${CYAN}Account Expiry Information: $username${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    if command -v chage &>/dev/null; then
        chage -l "$username" 2>/dev/null | sed 's/^/  /'
    else
        echo "  'chage' command not available" >&2
    fi
}

#===============================================================================
# Main
#===============================================================================

if [ $# -eq 0 ]; then
    usage
fi

case "${1:-}" in
    -u)
        list_users
        ;;
    -g)
        list_groups
        ;;
    -i)
        user_info "${2:-}"
        ;;
    -c)
        create_user "${2:-}"
        ;;
    -L)
        lock_user "${2:-}"
        ;;
    -U)
        unlock_user "${2:-}"
        ;;
    -p)
        show_permissions "${2:-}"
        ;;
    --expire)
        account_expire "${2:-}"
        ;;
    -h|--help)
        usage
        ;;
    *)
        echo -e "${RED}[ERROR]${NC} Unknown option: ${1:-}" >&2
        usage
        ;;
esac
