#!/bin/bash
set -e

# Check if source file is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <source_file.c>"
    echo "Example: $0 examples/two_sum.c"
    exit 1
fi

SOURCE_FILE="$1"

# Validate source file exists
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: Source file '$SOURCE_FILE' not found!"
    exit 1
fi

# Extract executable name (remove .c extension)
EXECUTABLE="${SOURCE_FILE%.c}"

# Get all .c files in containers/
CONTAINER_SRCS=$(find containers -name "*.c" | tr '\n' ' ')

# Compiler flags
CFLAGS="-Wall -Wextra -std=c11 -g -fsanitize=address,undefined"
INCLUDE_PATH="-Icontainers"

# Build command
echo "Building $SOURCE_FILE..."
echo "gcc $CFLAGS $INCLUDE_PATH $SOURCE_FILE $CONTAINER_SRCS -o $EXECUTABLE"
gcc $CFLAGS $INCLUDE_PATH "$SOURCE_FILE" $CONTAINER_SRCS -o "$EXECUTABLE"

echo "Build successful! Executable: $EXECUTABLE"

# Runs and deletes file
echo ""
echo "Running $EXECUTABLE..."
./"$EXECUTABLE"
rm "$EXECUTABLE"