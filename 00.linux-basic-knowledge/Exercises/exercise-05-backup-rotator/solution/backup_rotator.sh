#!/bin/bash
#===============================================================================
# Script Name: backup_rotator.sh
# Description: Automated backup and rotation utility
# Author: Solution
# License: MIT
#===============================================================================

set -euo pipefail

#===============================================================================
# Configuration
#===============================================================================

DEFAULT_BACKUP_DIR="./backups"
LOG_FILE="./backup.log"
DATE_FORMAT="+%Y%m%d_%H%M%S"
MAX_SCHEDULED_MINUTES=60

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
Backup Rotator - Automated Backup & Rotation Utility

Usage:
  $0 -c <source> [dest_dir]          Create backup
  $0 -l [backup_dir]                 List backups
  $0 -r <keep_count> [backup_dir]    Rotate backups (keep N newest)
  $0 -x <backup_file> [dest_dir]     Extract backup
  $0 -s <interval_m> <source> [dest]  Scheduled backup every N minutes
  $0 -v <backup_file>                Verify backup integrity
  $0 -h                              Show help

Examples:
  $0 -c ./my_project
  $0 -c ./my_project /tmp/backups
  $0 -l
  $0 -r 5 /tmp/backups
  $0 -x backups/file.tar.gz ./restore
  $0 -s 30 ./my_project
EOF
    exit 0
}

# Log message with timestamp
log() {
    local level="$1"
    local message="$2"
    local timestamp
    timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    echo "[$timestamp] [$level] $message" >> "$LOG_FILE"

    case "$level" in
        ERROR)   echo -e "${RED}[$level]${NC} $message" >&2 ;;
        WARNING) echo -e "${YELLOW}[$level]${NC} $message" ;;
        INFO)    echo -e "${GREEN}[$level]${NC} $message" ;;
        *)       echo "[$level] $message" ;;
    esac
}

# Check if directory exists and is readable
check_source() {
    local source="$1"
    if [ ! -e "$source" ]; then
        log "ERROR" "Source '$source' does not exist"
        return 1
    fi
    if [ ! -r "$source" ]; then
        log "ERROR" "Source '$source' is not readable"
        return 1
    fi
    return 0
}

# Ensure backup directory exists
ensure_backup_dir() {
    local dir="$1"
    if [ ! -d "$dir" ]; then
        mkdir -p "$dir" || {
            log "ERROR" "Cannot create backup directory '$dir'"
            return 1
        }
        log "INFO" "Created backup directory: $dir"
    fi
    if [ ! -w "$dir" ]; then
        log "ERROR" "Backup directory '$dir' is not writable"
        return 1
    fi
    return 0
}

# Format file size human-readable
format_size() {
    local bytes="$1"
    if [ "$bytes" -ge 1073741824 ]; then
        awk "BEGIN {printf \"%.2f GB\", $bytes/1073741824}"
    elif [ "$bytes" -ge 1048576 ]; then
        awk "BEGIN {printf \"%.2f MB\", $bytes/1048576}"
    elif [ "$bytes" -ge 1024 ]; then
        awk "BEGIN {printf \"%.2f KB\", $bytes/1024}"
    else
        echo "${bytes} B"
    fi
}

# Get all backup files sorted by time (newest first)
get_backup_files() {
    local dir="${1:-$DEFAULT_BACKUP_DIR}"
    if [ -d "$dir" ]; then
        ls -1t "$dir"/backup_*.tar.gz 2>/dev/null || true
    fi
}

#===============================================================================
# Core Functions
#===============================================================================

# 1. Create backup
create_backup() {
    local source="$1"
    local dest="${2:-$DEFAULT_BACKUP_DIR}"

    # Validate
    check_source "$source" || return 1
    ensure_backup_dir "$dest" || return 1

    # Resolve paths
    local abs_source
    abs_source=$(realpath "$source")
    local abs_dest
    abs_dest=$(realpath "$dest")

    # Generate filename
    local timestamp
    timestamp=$(date "$DATE_FORMAT")
    local dirname
    dirname=$(basename "$abs_source")
    local backup_file="${abs_dest}/backup_${dirname}_${timestamp}.tar.gz"

    log "INFO" "Creating backup of '$abs_source'..."
    log "INFO" "Destination: $backup_file"

    # Estimate size first
    local estimated_size
    estimated_size=$(du -sb "$abs_source" 2>/dev/null | cut -f1)
    log "INFO" "Source size: $(format_size "$estimated_size")"

    # Create backup
    local start_time end_time duration
    start_time=$(date +%s)

    if tar -czf "$backup_file" -C "$(dirname "$abs_source")" "$dirname" 2>>"$LOG_FILE"; then
        end_time=$(date +%s)
        duration=$((end_time - start_time))
        local file_size
        file_size=$(stat -c%s "$backup_file" 2>/dev/null || echo 0)

        log "SUCCESS" "Backup created: $(basename "$backup_file")"
        log "INFO" "  Size: $(format_size "$file_size") (compressed from $(format_size "$estimated_size"))"
        log "INFO" "  Duration: ${duration}s"
        log "INFO" "  Ratio: $(awk "BEGIN {printf \"%.1f%%\", ($file_size/$estimated_size)*100}")"

        echo "$backup_file"
    else
        log "ERROR" "Failed to create backup"
        rm -f "$backup_file"
        return 1
    fi
}

# 2. List backups
list_backups() {
    local dir="${1:-$DEFAULT_BACKUP_DIR}"

    if [ ! -d "$dir" ]; then
        log "WARNING" "Backup directory '$dir' does not exist"
        return 0
    fi

    local files
    files=$(get_backup_files "$dir")

    if [ -z "$files" ]; then
        log "WARNING" "No backups found in '$dir'"
        return 0
    fi

    local count=0
    local total_size=0

    echo
    echo -e "${CYAN}Backups in: $(realpath "$dir")${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"
    printf "  %-3s %-50s %12s %s\n" "#" "Filename" "Size" "Created"
    echo "  $(printf '%.0s-' {1..80})"

    while IFS= read -r file; do
        ((count++))
        local name
        name=$(basename "$file")
        local size
        size=$(stat -c%s "$file" 2>/dev/null || echo 0)
        local human_size
        human_size=$(format_size "$size")
        local created
        created=$(date -r "$file" '+%Y-%m-%d %H:%M:%S' 2>/dev/null || echo "N/A")
        total_size=$((total_size + size))

        printf "  %-3d %-50s %12s %s\n" "$count" "$name" "$human_size" "$created"
    done <<< "$files"

    echo "  $(printf '%.0s-' {1..80})"
    echo -e "  ${CYAN}Total: $count backup(s), $(format_size "$total_size")${NC}"
}

# 3. Rotate backups
rotate_backups() {
    local keep="$1"
    local dir="${2:-$DEFAULT_BACKUP_DIR}"

    if [ -z "$keep" ] || [ "$keep" -lt 1 ]; then
        log "ERROR" "Keep count must be at least 1"
        return 1
    fi

    if [ ! -d "$dir" ]; then
        log "WARNING" "Backup directory '$dir' does not exist"
        return 0
    fi

    local files
    files=$(get_backup_files "$dir")

    if [ -z "$files" ]; then
        log "WARNING" "No backups to rotate in '$dir'"
        return 0
    fi

    local total
    total=$(echo "$files" | wc -l)
    local to_delete=$((total - keep))

    if [ "$to_delete" -le 0 ]; then
        log "INFO" "Only $total backup(s), need to keep $keep. Nothing to delete."
        return 0
    fi

    log "INFO" "Rotating backups: keep $keep of $total (will delete $to_delete)"

    local deleted=0
    local deleted_size=0

    # Get oldest files (reverse of ls -1t)
    while IFS= read -r file; do
        if [ $((total - deleted)) -le "$keep" ]; then
            break
        fi
        local size
        size=$(stat -c%s "$file" 2>/dev/null || echo 0)
        log "DELETE" "Removing: $(basename "$file") ($(format_size "$size"))"
        rm -f "$file"
        ((deleted++))
        deleted_size=$((deleted_size + size))
    done < <(ls -1tr "$dir"/backup_*.tar.gz 2>/dev/null)

    log "SUCCESS" "Rotation complete: deleted $deleted backup(s) ($(format_size "$deleted_size"))"
}

# 4. Extract backup
extract_backup() {
    local backup_file="$1"
    local dest_dir="${2:-./restored}"

    if [ ! -f "$backup_file" ]; then
        log "ERROR" "Backup file not found: $backup_file"
        return 1
    fi

    # Verify first
    log "INFO" "Verifying backup integrity before extract..."
    if ! tar -tzf "$backup_file" &>/dev/null; then
        log "ERROR" "Backup file is corrupted or invalid: $backup_file"
        return 1
    fi

    log "INFO" "Integrity check passed"

    # Create destination
    mkdir -p "$dest_dir" || {
        log "ERROR" "Cannot create destination directory '$dest_dir'"
        return 1
    }

    local abs_dest
    abs_dest=$(realpath "$dest_dir")

    log "INFO" "Extracting to: $abs_dest"

    local start_time end_time duration
    start_time=$(date +%s)

    if tar -xzf "$backup_file" -C "$abs_dest" 2>>"$LOG_FILE"; then
        end_time=$(date +%s)
        duration=$((end_time - start_time))

        local file_count
        file_count=$(tar -tzf "$backup_file" | wc -l)

        log "SUCCESS" "Extracted $file_count items to '$abs_dest' (${duration}s)"
        ls -la "$abs_dest"
    else
        log "ERROR" "Failed to extract backup"
        return 1
    fi
}

# 5. Scheduled backup
schedule_backup() {
    local interval="$1"
    local source="$2"
    local dest="${3:-$DEFAULT_BACKUP_DIR}"

    # Validate interval
    if [ -z "$interval" ] || [ "$interval" -lt 1 ]; then
        log "ERROR" "Interval must be at least 1 minute"
        return 1
    fi
    if [ "$interval" -gt "$MAX_SCHEDULED_MINUTES" ]; then
        log "ERROR" "Interval cannot exceed $MAX_SCHEDULED_MINUTES minutes"
        return 1
    fi

    check_source "$source" || return 1
    ensure_backup_dir "$dest" || return 1

    log "INFO" "========================================"
    log "INFO" "Starting scheduled backup every ${interval} minute(s)"
    log "INFO" "Source: $(realpath "$source")"
    log "INFO" "Destination: $(realpath "$dest")"
    log "INFO" "Rotation: keeping 10 most recent backups"
    log "INFO" "PID: $$"
    log "INFO" "========================================"

    local iteration=0
    while true; do
        ((iteration++))
        log "INFO" "--- Backup iteration #$iteration ---"

        if create_backup "$source" "$dest" > /dev/null; then
            # Rotate to keep only 10
            rotate_backups 10 "$dest" || true
            log "INFO" "Next backup in ${interval} minute(s)..."
        else
            log "ERROR" "Backup iteration #$iteration failed"
        fi

        sleep $((interval * 60))
    done
}

# 6. Verify backup
verify_backup() {
    local backup_file="$1"

    if [ ! -f "$backup_file" ]; then
        log "ERROR" "Backup file not found: $backup_file"
        return 1
    fi

    local file_size
    file_size=$(stat -c%s "$backup_file" 2>/dev/null || echo 0)
    local name
    name=$(basename "$backup_file")

    echo
    echo -e "${CYAN}Verifying backup: $name${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"
    echo -e "  File size: $(format_size "$file_size")"
    echo -e "  Modified:  $(date -r "$backup_file" '+%Y-%m-%d %H:%M:%S')"

    # Check file type
    local file_type
    file_type=$(file "$backup_file" 2>/dev/null)
    echo -e "  Type:      $file_type"

    # Test integrity
    echo
    log "INFO" "Testing archive integrity..."

    if tar -tzf "$backup_file" &>/dev/null; then
        local file_count
        file_count=$(tar -tzf "$backup_file" | wc -l)
        log "SUCCESS" "Integrity check PASSED"
        echo -e "  Files:     $file_count entries"

        # Show contents (first 20 files)
        echo
        echo -e "${CYAN}Archive contents (first 20):${NC}"
        tar -tzf "$backup_file" | head -20 | sed 's/^/  /'
        if [ "$file_count" -gt 20 ]; then
            echo "  ... and $((file_count - 20)) more files"
        fi
    else
        log "ERROR" "Integrity check FAILED - file may be corrupted"
        return 1
    fi
}

#===============================================================================
# Main
#===============================================================================

main() {
    if [ $# -eq 0 ]; then
        usage
    fi

    case "${1:-}" in
        -c)
            create_backup "${2:-}" "${3:-}"
            ;;
        -l)
            list_backups "${2:-}"
            ;;
        -r)
            rotate_backups "${2:-}" "${3:-}"
            ;;
        -x)
            extract_backup "${2:-}" "${3:-}"
            ;;
        -s)
            schedule_backup "${2:-}" "${3:-}" "${4:-}"
            ;;
        -v)
            verify_backup "${2:-}"
            ;;
        -h|--help)
            usage
            ;;
        *)
            log "ERROR" "Unknown option: ${1:-}"
            usage
            ;;
    esac
}

main "$@"
