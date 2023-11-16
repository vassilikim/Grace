#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

set -e

#make

echo -e "${CYAN}"
echo "   \$\$\$\$\$\$\\"                                         
echo "  \$\$  __\$\$\\"                                        
echo "  \$\$ |  \\_|  \$\$\$\$\$\$\\  \$\$\$\$\$\$\\   \$\$\$\$\$\$\$\\  \$\$\$\$\$\$\\"  
echo "  \$\$ |\$\$\$\$\\ \$\$  __\$\$\\ \\____\$\$\\ \$\$  _____|\$\$  __\$\$\\" 
echo "  \$\$ |\\_\$\$ |\$\$ |  \\__|\$\$\$\$\$\$\$ |\$\$ |      \$\$\$\$\$\$\$\$ |"
echo "  \$\$ |  \$\$ |\$\$ |     \$\$  __\$\$ |\$\$ |      \$\$   ____|"
echo "  \\\$\$\$\$\$\$  |\$\$ |     \\$\$\$\$\$\$\$ |\\\$\$\$\$\$\$\$\\ \\\$\$\$\$\$\$\$\\" 
echo "   \______/ \__|      \_______| \_______| \_______|"
echo -e "${NC}"

./grace <$1
llc temp.ll
echo -e "- ${YELLOW}Assemble Generation${NC}: ${GREEN}PASSED${NC}"
clang -no-pie temp.s lib.a
echo -e "- ${YELLOW}Output Executable${NC}: ${GREEN}PASSED${NC}"
echo -e "- Compilation completed ${GREEN}SUCCESSFULLY${NC}."

echo ""
echo "Program Execution:" 
echo "*************************************************" 
./a.out
echo "*************************************************" 
echo "" 
