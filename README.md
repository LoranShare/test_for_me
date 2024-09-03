# Задание

Разработайте блочный аллокатор памяти на C или C++. Аллокатор должен выделять и освобождать по одному блоку фиксированного размера из статического пула. Размер блока и количество блоков фиксированы в процессе выполнения программы, но должны быть настраиваемыми до сборки проекта. Модуль должен быть адаптирован для работы на Embedded-платформах различной разрядности в условиях многозадачного окружения. Модуль должен иметь набор юнит-тестов.

Условия, требующие уточнения, трактуйте на своё усмотрение. Оставьте об этом комментарии в файлах исходного кода. Используйте кодировку UTF-8.

# Проект

```
├── source      # Исходники модуля блочного аллокатора
│   ├── inc
│   └── src
├── test        # Исходники тестов
├── Makefile    # Makefile для сборки
├── README.md
├── run_test.sh # Скрипт для запуска тестов

```

## Требования
* Тестовый фраймворк - CUnit-2.0-1.
* Компилятор - gcc 13.2.0.
* Сборка - GNU Make 4.3.

## Запуск
Для сборки проекта и запуска тестов необходимо вызвать скрипт из корня репозитория:

```
$  ./run_test.sh
Test 0 - Passed
Test 1 - Passed
Test 2 - Passed
Test 3 - Passed
Test 4 - Passed
Test 5 - Passed
Test 6 - Passed
Test 7 - Passed


     CUnit - A unit testing framework for C - Version 2.1-3
     http://cunit.sourceforge.net/


Suite: common_suilt
  Test: allocating one chunk of memory ...passed
  Test: allocate all memory ...passed
  Test: freeing up addresses not in the pool ...passed
  Test: test for freeing unaligned addresses ...passed
  Test: freeing a memory block twice ...passed

Run Summary:    Type  Total    Ran Passed Failed Inactive
              suites      1      1    n/a      0        0
               tests      5      5      5      0        0
             asserts    356    356    356      0      n/a

Elapsed time =    0.000 seconds
```

## To do
Не реализовал доступ для многозадачного окружения. В общих чертах понятно, что нужен примитив синхронизации (мьютекс), который так же должен учитывать возможность прерывания. Скорее всего он будет платформозависим и он необходим во всех операциях, где меняется освобождается, выделяется память.

Тесты в целом можно улучшить, добавить бенчмарк и т.п.