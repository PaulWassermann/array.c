SHELL = cmd

CC = clang
TARGET_ARCH = x86_64-pc-windows-msvc

CFLAGS = -Wall -Werror --target=$(TARGET_ARCH)

SRC_FILES = src\array.c src\utils.c
TEST_FILE = tests\tests.c

FORMAT_FILES = $(SRC_FILES) src\array.h src\utils.h \
               tests\builder.c tests\utils.h \
               tests\unit_tests\test_array.h tests\unit_tests\test_utils.h 

.PHONY: check-format
check-format:
	clang-format --Werror --dry-run --style=file --verbose $(FORMAT_FILES)

.PHONY: code-analysis
code-analysis:
	clang --analyzer-output text --analyze $(SRC_FILES)

.PHONY: compile
compile: target
	$(CC) $(CFLAGS) -o target\main.exe src\main.c $(SRC_FILES)

.PHONY: format
format:
	clang-format --Werror -i --style=file --verbose $(FORMAT_FILES)

.PHONY: target-folder
target-folder:
	@IF NOT EXIST "target" MKDIR "target"

.PHONY: test
test: target-folder test-builder
	target\builder.exe tests -t tests\template.txt -o $(TEST_FILE)
	$(CC) $(CFLAGS) -o target\tests.exe $(TEST_FILE) $(SRC_FILES)

.PHONY: test-builder
test-builder:
	$(CC) $(CFLAGS) -o target\builder.exe tests\builder.c src\utils.c
