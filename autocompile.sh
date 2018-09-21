#!/bin/bash

readonly PROJECT_ROOT=$( cd "$( dirname "$0" )" && pwd )
readonly FILE_PATH="$1"

readonly FILE_DIR=$(dirname "$FILE_PATH")

readonly EXECUTABLE="$FILE_DIR/program"

readonly COMPILE_COMMAND="g++ -Wall -std=c11 $FILE_PATH -o $EXECUTABLE"

readonly GIT_STATUS_COMMAND="git status"
readonly PRINT_LINE="echo \\\"==================================================\\\""

readonly entr_command="
tput reset;
echo \\\"Compiling...\\\";
$PRINT_LINE;
$COMPILE_COMMAND;
echo; $PRINT_LINE;
echo \\\"Running GIT Status...\\\";
$PRINT_LINE;
$GIT_STATUS_COMMAND;
echo;
date;"

while true; do
  echo "$FILE_PATH" |
  entr -d bash -c "$entr_command"
done
