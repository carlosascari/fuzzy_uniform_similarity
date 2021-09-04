#!/bin/bash

MODULE_NAME="fuzzy"
BASE="."
SRC="$BASE"
INC="-I$BASE"
LIBS="-lm"
LIB="-L$BASE $LIBS"
TMP="./tmp"
CC="gcc"
CARGS="-O2 -std=gnu99"

# Build tmp folder
mkdir -p $TMP
rm -f -r $TMP/*

# compile fuzzy object
$CC $CARGS $INC -c -fPIC -o $TMP/fuzzy.o $SRC/fuzzy.c

# compile main
$CC $CARGS $INC -c -fPIC -o $TMP/main.o $SRC/main.c

# Linking objects
$CC $CARGS $INC $LIB -o $TMP/$MODULE_NAME $TMP/*o $LIB

# Run
cp ./$TMP/$MODULE_NAME ./
./$MODULE_NAME samples/A samples/A1
./$MODULE_NAME samples/A samples/A2
./$MODULE_NAME samples/A samples/A3
