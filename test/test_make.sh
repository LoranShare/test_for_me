#!/bin/bash
## @author    Gevorgyan Ovik
## @brief     Build tests

# @todo добавить критерий на конкретную ошибку сборки, т.к. тест проходит при
# при любых ошибках сборки

# @note Проверку можно вынести в функцию, но мало времени
# @note можно добавить проверки на наличие тулзов и их версий
# @note результат make и вызова тестов можно в логи сохранить
test_num=0;

PATH_TO_MAKEFILE="$(dirname -- "${BASH_SOURCE[0]}")"
PATH_TO_MAKEFILE="$(cd -- "$PATH_TO_MAKEFILE" && pwd)"

if [[ -z "$PATH_TO_MAKEFILE" ]] ; then
  exit 1
fi

PATH_TO_MAKEFILE="$PATH_TO_MAKEFILE"/../

# Проверяем, что сборка не возможна при ошибочных параметрах сборки
make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE > /dev/null 2>&1
if [ "$?" -eq 0 ]; then
	echo "Test $test_num - Failed"
	exit 1;
fi

echo "Test $test_num - Passed"
test_num=$((test_num + 1))

make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE MEM_SIZE=0 CHUNK_SIZE=0 > /dev/null 2>&1
if [ "$?" -eq 0 ]; then
	echo "Test $test_num - Failed"
	exit 1;
fi

echo "Test $test_num - Passed"
test_num=$((test_num + 1))

make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE MEM_SIZE=10 CHUNK_SIZE=0 > /dev/null 2>&1
if [ "$?" -eq 0 ]; then
	echo "Test $test_num - Failed"
	exit 1;
fi

echo "Test $test_num - Passed"
test_num=$((test_num + 1))


make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE MEM_SIZE=0 CHUNK_SIZE=10 > /dev/null 2>&1
if [ "$?" -eq 0 ]; then
	echo "Test $test_num - Failed"
	exit 1;
fi

echo "Test $test_num - Passed"
test_num=$((test_num + 1))

make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE MEM_SIZE=10 CHUNK_SIZE=1 > /dev/null 2>&1
if [ "$?" -ne 0 ]; then
	echo "Test $test_num - Failed"
	exit 1;
fi

echo "Test $test_num - Passed"
test_num=$((test_num + 1))

make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE MEM_SIZE=10 CHUNK_SIZE=11 > /dev/null 2>&1
if [ "$?" -eq 0 ]; then
	echo "Test $test_num - Failed"
	exit 1;
fi

echo "Test $test_num - Passed"
test_num=$((test_num + 1))

make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE MEM_SIZE=20 CHUNK_SIZE=10 > /dev/null 2>&1
if [ "$?" -ne 0 ]; then
	echo "Test $test_num - Failed"
	exit 1;
fi

echo "Test $test_num - Passed"
test_num=$((test_num + 1))

make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1 && make -C $PATH_TO_MAKEFILE MEM_SIZE=10 CHUNK_SIZE=10 > /dev/null 2>&1
if [ "$?" -ne 0 ]; then
	echo "Test $test_num - Failed"
	exit 1;
fi

echo "Test $test_num - Passed"

make -C $PATH_TO_MAKEFILE clean > /dev/null 2>&1
