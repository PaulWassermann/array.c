SHELL = cmd

CC = clang
TARGET_ARCH = x86_64-pc-windows-msvc

CFLAGS = -Wall -Werror --target=$(TARGET_ARCH)

TARGET_DIR = target

SRC_FILES = src\array.c src\utils.c src\ops\ops.c
SRC_HEADER_FILES = src\array.h src\log.h src\ops\float32_ops.h src\ops\ops.h \
                   src\utils.h

TEST_FILE = tests\tests.c
TEST_HEADER_FILES = tests\utils.h
UNIT_TEST_FILES = tests\unit_tests\test_array.h tests\unit_tests\test_utils.h \
                  tests\unit_tests\ops\test_ops.h \
                  tests\unit_tests\ops\test_float32_ops.h

FORMAT_FILES = $(SRC_FILES) $(SRC_HEADER_FILES) \
               $(TEST_HEADER_FILES) $(UNIT_TEST_FILES) \
               tests\builder.c  

.PHONY: check-format
check-format:
	clang-format --Werror --dry-run --style=file --verbose $(FORMAT_FILES)

.PHONY: clean
clean:
	@DEL "$(TEST_FILE)"
	@RMDIR /S /Q "$(TARGET_DIR)"

.PHONY: code-analysis
code-analysis:
	clang --analyzer-output text --analyze $(SRC_FILES)

.PHONY: compile
compile: target-folder
	$(CC) $(CFLAGS) -o $(TARGET_DIR)\main.exe src\main.c $(SRC_FILES)

.PHONY: format
format:
	clang-format --Werror -i --style=file --verbose $(FORMAT_FILES)

.PHONY: target-folder
target-folder:
	@IF NOT EXIST "$(TARGET_DIR)" MKDIR "$(TARGET_DIR)"

.PHONY: test
test: target-folder
	$(TARGET_DIR)\builder.exe tests -t tests\template.txt -o $(TEST_FILE)
	$(CC) $(CFLAGS) -o $(TARGET_DIR)\tests.exe $(TEST_FILE) $(SRC_FILES)
	$(TARGET_DIR)\tests.exe

.PHONY: test-builder
test-builder: target-folder
	$(CC) $(CFLAGS) -D LOG_LEVEL=ERROR_LEVEL -o $(TARGET_DIR)\builder.exe tests\builder.c src\utils.c
