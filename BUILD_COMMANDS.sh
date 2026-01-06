#!/bin/bash

# ╔════════════════════════════════════════════════════════════╗
# ║   ☠ ARMAGEDDON Framework v3.0.0 - Build Script ☠           ║
# ║   ESP32-S3-N16R8 Pentest Firmware                          ║
# ╚════════════════════════════════════════════════════════════╝

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default values
TARGET="esp32s3"
PORT="/dev/ttyUSB0"
BAUD="921600"
ACTION="build"

# Print header
print_header() {
    echo -e "${BLUE}"
    echo "╔════════════════════════════════════════════════════════════╗"
    echo "║   ☠ ARMAGEDDON Framework v3.0.0 Build Helper ☠            ║"
    echo "║   ESP32-S3-N16R8 Pentest Firmware                          ║"
    echo "╚════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
}

# Print help
print_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -t, --target TARGET       Target chip (default: esp32s3)"
    echo "  -p, --port PORT           Serial port (default: /dev/ttyUSB0)"
    echo "  -b, --baud BAUD           Baud rate (default: 921600)"
    echo "  -a, --action ACTION       build|clean|flash|monitor|all (default: build)"
    echo "  -h, --help                Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                                  # Build only"
    echo "  $0 -a all                          # Build, flash, and monitor"
    echo "  $0 -a flash -p /dev/ttyUSB0        # Flash only"
    echo "  $0 -a monitor                      # Monitor serial output"
    echo "  $0 -a clean                        # Clean build artifacts"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -t|--target)
            TARGET="$2"
            shift 2
            ;;
        -p|--port)
            PORT="$2"
            shift 2
            ;;
        -b|--baud)
            BAUD="$2"
            shift 2
            ;;
        -a|--action)
            ACTION="$2"
            shift 2
            ;;
        -h|--help)
            print_help
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            print_help
            exit 1
            ;;
    esac
done

# Main script
print_header

echo -e "${YELLOW}Configuration:${NC}"
echo "  Target: $TARGET"
echo "  Port: $PORT"
echo "  Baud: $BAUD"
echo "  Action: $ACTION"
echo ""

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ] || [ ! -d "main" ]; then
    echo -e "${RED}Error: CMakeLists.txt or main/ directory not found!${NC}"
    echo "Please run this script from the project root directory."
    exit 1
fi

# Set target
if [ "$ACTION" != "monitor" ]; then
    echo -e "${YELLOW}[1/4] Setting target to $TARGET...${NC}"
    idf.py set-target $TARGET
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Target set successfully${NC}"
    else
        echo -e "${RED}✗ Failed to set target${NC}"
        exit 1
    fi
fi

# Clean if requested
if [ "$ACTION" = "clean" ]; then
    echo -e "${YELLOW}Cleaning build artifacts...${NC}"
    idf.py fullclean
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Clean complete${NC}"
    else
        echo -e "${RED}✗ Clean failed${NC}"
        exit 1
    fi
    exit 0
fi

# Build
if [ "$ACTION" = "build" ] || [ "$ACTION" = "all" ]; then
    echo -e "${YELLOW}[2/4] Building project...${NC}"
    idf.py build
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Build successful${NC}"
        
        # Show size information
        echo ""
        echo -e "${BLUE}Binary size:${NC}"
        idf.py size 2>/dev/null || true
    else
        echo -e "${RED}✗ Build failed${NC}"
        exit 1
    fi
fi

# Flash
if [ "$ACTION" = "flash" ] || [ "$ACTION" = "all" ]; then
    echo -e "${YELLOW}[3/4] Flashing to $PORT (baud: $BAUD)...${NC}"
    
    # Check if port exists
    if [ ! -e "$PORT" ]; then
        echo -e "${RED}✗ Serial port not found: $PORT${NC}"
        echo "Available ports:"
        ls -la /dev/tty* 2>/dev/null | grep -E "USB|ACM" || echo "  (none found)"
        exit 1
    fi
    
    idf.py -p $PORT -b $BAUD flash
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Flash successful${NC}"
    else
        echo -e "${RED}✗ Flash failed${NC}"
        exit 1
    fi
fi

# Monitor
if [ "$ACTION" = "monitor" ] || [ "$ACTION" = "all" ]; then
    echo -e "${YELLOW}[4/4] Starting serial monitor on $PORT (baud: $BAUD)...${NC}"
    echo -e "${BLUE}Press Ctrl+] to exit the monitor${NC}"
    echo ""
    
    idf.py -p $PORT -b $BAUD monitor
fi

echo ""
echo -e "${GREEN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║  ✓ Operation complete!                                    ║${NC}"
echo -e "${GREEN}║                                                            ║${NC}"
echo -e "${GREEN}║  Next steps:                                              ║${NC}"
echo -e "${GREEN}║  1. Connect to WiFi: HackESP32 / redteam123              ║${NC}"
echo -e "${GREEN}║  2. Open: http://192.168.4.1 in your browser             ║${NC}"
echo -e "${GREEN}║  3. Start using BadUSB, WiFi attacks, and logging        ║${NC}"
echo -e "${GREEN}║                                                            ║${NC}"
echo -e "${GREEN}║  Ready for engagement! 💀🖤🔥                            ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════════════════════════╝${NC}"
