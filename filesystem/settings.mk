# Libraries
LIBS=static-utils commons

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static-utils

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=./config/config-inicial.config ./instrucciones/instrucciones1

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="./logs/memcheck.log"
HELGRIND_FLAGS=--log-file="./logs/helgrind.log"

# Source files (*.c) to be excluded from tests compilation
TEST_EXCLUDE=src/main.c
