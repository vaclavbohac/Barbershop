#!/bin/bash

# Vaclav Bohac (c) 2011

BIN_DIR=bin

cd $BIN_DIR
for file in `ls`
do
	if [ -x "$file" ]; then
		./$file
	fi
done
