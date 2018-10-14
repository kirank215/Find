#!/bin/sh
myfind=./myfind
realfind=find
tests=$1
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
echo "------##Threshold 0##------"
printf "\n"
declare -a arg=("" "src" "$1" "src $1" )
for i in "${arg[@]}"
do
    output=$($myfind $i 2>/dev/null)
    error1=$?
    output1=$($realfind $i 2>/dev/null)
    error2=$?
    if [ "$output" = "$output1" ]; then
        echo -e " filename: "$i" : ${GREEN}OK${NC}"
    else
        echo -e " filename: "$i": ${RED}KO${NC}"
        echo -e " ${RED}$output${NC} "
        echo -e " ${GREEN}$output1${NC}"
        exit
    fi
    if [ "$error1" = "$error2" ]; then 
        echo -e " filename: "$i": ${GREEN}OK :RETURN VAL${NC}"
    else
        echo -e " filename: "$i": ${RED}KO :RETURN VAL${NC}"
        echo -e " RETURN VAL : ${RED}$error1${NC}"
        echo -e " EXPECTED RET : ${GREEN}$error2${NC}"
        exit
    fi
done
printf "\n"
printf "\n"
echo "------##Threshold 1##------"
printf "\n"
declare -a arg=("-P" "-L" "-L -H" "-L -P" "-L -H $1" "-P -L src $1")
for i in "${arg[@]}"
do
    output=$($myfind $i 2>/dev/null)
    error1=$?
    output1=$($realfind $i 2>/dev/null)
    error2=$?
    if [ "$output" = "$output1" ]; then
        echo -e " filename: "$i" : ${GREEN}OK${NC}"
    else
        echo -e " filename: "$i": ${RED}KO${NC}"
        echo -e " ${RED}$output${NC} "
        echo -e " ${GREEN}$output1${NC}"
        exit
    fi
    if [ "$error1" = "$error2" ]; then 
        echo -e " filename: "$i": ${GREEN}OK :RETURN VAL${NC}"
    else
        echo -e " filename: "$i": ${RED}KO :RETURN VAL${NC}"
        echo -e " RETURN VAL : ${RED}$error1${NC}"
        echo -e " EXPECTED RET : ${GREEN}$error2${NC}"
        exit
    fi
done
printf "\n"
printf "\n"
echo "------##Threshold 2##------"
printf "\n"
declare -a arg=("-name $1" "-type d" "-type l" "-name src" )
for i in "${arg[@]}"
do
    output=$($myfind $i 2>/dev/null)
    error1=$?
    output1=$($realfind $i 2>/dev/null)
    error2=$?
    if [ "$output" = "$output1" ]; then
        echo -e " filename: "$i" : ${GREEN}OK${NC}"
    else
        echo -e " filename: "$i": ${RED}KO${NC}"
        echo -e " ${RED}$output${NC} "
        echo -e " ${GREEN}$output1${NC}"
        exit
    fi
    if [ "$error1" = "$error2" ]; then 
        echo -e " filename: "$i": ${GREEN}OK :RETURN VAL${NC}"
    else
        echo -e " filename: "$i": ${RED}KO :RETURN VAL${NC}"
        echo -e " RETURN VAL : ${RED}$error1${NC}"
        echo -e " EXPECTED RET : ${GREEN}$error2${NC}"
        exit
    fi
done
printf "\n"
printf "\n"
echo "------##Threshold 3##------"
printf "\n"
declare -a arg=("$1 -name d -o type d " "-type d -a -name $1" "-type l" 
    "-name src -a -print" "$1 -print -a -print" "$1 -type l -o -print" )
for i in "${arg[@]}"
do
    output=$($myfind $i 2>/dev/null)
    error1=$?
    output1=$($realfind $i 2>/dev/null)
    error2=$?
    if [ "$output" = "$output1" ]; then
        echo -e " filename: "$i" : ${GREEN}OK${NC}"
    else
        echo -e " filename: "$i": ${RED}KO${NC}"
        echo -e " ${RED}$output${NC} "
        echo -e " ${GREEN}$output1${NC}"
        exit
    fi
    if [ "$error1" = "$error2" ]; then 
        echo -e " filename: "$i": ${GREEN}OK :RETURN VAL${NC}"
    else
        echo -e " filename: "$i": ${RED}KO :RETURN VAL${NC}"
        echo -e " RETURN VAL : ${RED}$error1${NC}"
        echo -e " EXPECTED RET : ${GREEN}$error2${NC}"
        exit
    fi
done
