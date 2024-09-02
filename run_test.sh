#!/bin/bash
#

MEM_SIZE=8000
CHUNK_SIZE=80
PATH_TO_MAKEFILE=.

./test/test_make.sh

# Проверяем, что сборка не возможна при ошибочных параметрах сборки
make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE MEM_SIZE=$MEM_SIZE CHUNK_SIZE=$CHUNK_SIZE > /dev/null 2>&1
if [ "$?" -ne 0 ]; then
	echo "Building test - Failed"
	exit 1;
fi

./build/cunit_test
