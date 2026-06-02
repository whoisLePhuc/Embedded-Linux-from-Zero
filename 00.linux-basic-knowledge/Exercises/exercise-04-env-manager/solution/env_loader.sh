#!/bin/bash
#===============================================================================
# Script Name: env_loader.sh
# Description: Load và validate .env files
# Author: Solution
#===============================================================================

set -euo pipefail

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

# Danh sách biến bắt buộc
REQUIRED_VARS=("APP_NAME" "LOG_LEVEL" "DATA_DIR")
LOADED_COUNT=0
FAILED_COUNT=0

usage() {
    echo "Usage: $0 <env_file>"
    echo "Load and validate environment variables from a .env file"
    exit 1
}

# Kiểm tra file
validate_file() {
    local file="$1"
    if [ ! -f "$file" ]; then
        echo -e "${RED}[ERROR]${NC} File not found: $file" >&2
        return 1
    fi
    if [ ! -r "$file" ]; then
        echo -e "${RED}[ERROR]${NC} No read permission: $file" >&2
        return 1
    fi
    return 0
}

# Load file .env (chỉ xử lý dòng KEY=VALUE, bỏ comment và empty)
load_env_file() {
    local file="$1"
    local line_num=0

    while IFS= read -r line || [ -n "$line" ]; do
        ((line_num++))

        # Bỏ comment và empty lines
        [[ "$line" =~ ^[[:space:]]*# ]] && continue
        [[ "$line" =~ ^[[:space:]]*$ ]] && continue

        # Kiểm tra format
        if [[ "$line" =~ ^[a-zA-Z_][a-zA-Z0-9_]*=.*$ ]]; then
            local var_name="${line%%=*}"
            local var_value="${line#*=}"

            # Remove surrounding quotes nếu có
            var_value="${var_value%\"}"
            var_value="${var_value#\"}"
            var_value="${var_value%\'}"
            var_value="${var_value#\'}"

            # Export variable
            export "$var_name=$var_value"
            ((LOADED_COUNT++))

            # Hiển thị biến đã load
            if [ ${#var_value} -gt 50 ]; then
                printf "  ${GREEN}[LOADED]${NC} %-30s = %s...\n" "$var_name" "${var_value:0:50}"
            else
                printf "  ${GREEN}[LOADED]${NC} %-30s = %s\n" "$var_name" "$var_value"
            fi
        else
            echo -e "  ${YELLOW}[SKIP]${NC} Line $line_num: Invalid format (skipped)"
            ((FAILED_COUNT++))
        fi
    done < "$file"
}

# Validate biến bắt buộc
validate_required() {
    local missing=0

    echo
    echo -e "${CYAN}Validating required variables...${NC}"

    for var in "${REQUIRED_VARS[@]}"; do
        if [ -z "${!var+x}" ]; then
            echo -e "  ${RED}[MISSING]${NC} Required variable '$var' is NOT set!"
            ((missing++))
        else
            echo -e "  ${GREEN}[OK]${NC} Required variable '$var' = '${!var}'"
        fi
    done

    echo
    if [ "$missing" -gt 0 ]; then
        echo -e "${RED}[ERROR]${NC} $missing required variable(s) missing!"
        return 1
    else
        echo -e "${GREEN}[SUCCESS]${NC} All required variables are set"
        return 0
    fi
}

# Show summary
show_summary() {
    echo
    echo -e "${CYAN}=== Load Summary ===${NC}"
    echo -e "  ${GREEN}Loaded:${NC} $LOADED_COUNT variables"
    echo -e "  ${YELLOW}Failed:${NC} $FAILED_COUNT lines"
    echo -e "  ${CYAN}Total:${NC}  $((LOADED_COUNT + FAILED_COUNT)) lines processed"
}

#===============================================================================
# Main
#===============================================================================

if [ $# -lt 1 ]; then
    usage
fi

ENV_FILE="$1"

echo -e "${CYAN}=== Environment Loader ===${NC}"
echo "  File: $ENV_FILE"

# Validate file
validate_file "$ENV_FILE" || exit 1

# Load
echo -e "${CYAN}Loading variables...${NC}"
load_env_file "$ENV_FILE"

# Validate
validate_required || true

# Summary
show_summary

echo
echo -e "${GREEN}Done! Variables are available in current shell.${NC}"
echo "  Use 'printenv | grep APP' to verify"
