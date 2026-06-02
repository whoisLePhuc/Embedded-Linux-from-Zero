#!/bin/bash
#===============================================================================
# Script Name: system_info.sh
# Description: Hiển thị thông tin hệ thống chi tiết
# Author: Solution
#===============================================================================

set -euo pipefail

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Function: Hiển thị section header
print_section() {
    local title="$1"
    echo
    echo -e "${CYAN}=== $title ===${NC}"
}

# Function: Hiển thị key-value với căn chỉnh
print_info() {
    local key="$1"
    local value="$2"
    printf "  %-25s : %s\n" "$key" "$value"
}

# Function: Kiểm tra lệnh có tồn tại không
check_command() {
    if ! command -v "$1" &> /dev/null; then
        echo -e "${RED}[ERROR]${NC} Command '$1' not found. Please install it first."
        return 1
    fi
    return 0
}

# Function: In thông tin CPU từ /proc/cpuinfo
get_cpu_info() {
    local model
    model=$(grep "model name" /proc/cpuinfo | head -1 | cut -d':' -f2 | sed 's/^[ \t]*//')
    local cores
    cores=$(nproc 2>/dev/null || grep -c "^processor" /proc/cpuinfo)
    echo "$model ($cores cores)"
}

# Function: Lấy thông tin RAM
get_ram_info() {
    local total used available
    if check_command "free"; then
        total=$(free -h | awk '/^Mem:/ {print $2}')
        used=$(free -h | awk '/^Mem:/ {print $3}')
        available=$(free -h | awk '/^Mem:/ {print $7}')
        echo "${used} / ${total} (available: ${available})"
    else
        echo "N/A (free not available)"
    fi
}

# MAIN
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}   SYSTEM INFORMATION REPORT${NC}"
echo -e "${GREEN}========================================${NC}"

# 1. System Info
print_section "System Information"
print_info "Hostname" "$(hostname)"
print_info "Kernel" "$(uname -r)"
print_info "OS" "$(uname -o)"
print_info "Architecture" "$(uname -m)"
print_info "Uptime" "$(uptime -p 2>/dev/null || echo 'N/A')"
print_info "Date/Time" "$(date '+%Y-%m-%d %H:%M:%S')"

# 2. CPU Info
print_section "CPU Information"
print_info "CPU Model" "$(get_cpu_info)"
print_info "Load Average" "$(uptime | awk -F'load average:' '{print $2}' | sed 's/^[ \t]*//')"

# 3. Memory Info
print_section "Memory Information"
print_info "RAM" "$(get_ram_info)"
if check_command "free"; then
    print_info "Swap" "$(free -h | awk '/^Swap:/ {print $3 " / " $2}')"
fi

# 4. Disk Info
print_section "Disk Information"
if check_command "df"; then
    df -h / | awk 'NR==2 {printf "  %-25s : %s (%s used of %s)\n", "Root (/)", $5, $3, $2}'
    # Thêm thông tin các partition quan trọng
    for mount in /boot /home /var; do
        if mountpoint -q "$mount" 2>/dev/null; then
            df -h "$mount" | awk -v m="$mount" 'NR==2 {printf "  %-25s : %s (%s used of %s)\n", m, $5, $3, $2}'
        fi
    done
fi

# 5. Network Info
print_section "Network Information"
print_info "Hostname" "$(hostname)"
if check_command "hostname"; then
    print_info "IP Address" "$(hostname -I 2>/dev/null | awk '{print $1}')"
fi
if check_command "ip"; then
    print_info "Default Gateway" "$(ip route | grep default | awk '{print $3}')"
fi

# 6. Additional Info (User/Process)
print_section "System Load"
print_info "Logged-in Users" "$(who | wc -l)"
print_info "Running Processes" "$(ps aux --no-headers 2>/dev/null | wc -l)"

echo
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}   REPORT GENERATED: $(date '+%Y-%m-%d %H:%M:%S')${NC}"
echo -e "${GREEN}========================================${NC}"
