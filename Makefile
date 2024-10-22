CC = clang
CFLAGS = -Wall -Werror
TARGET_ARCH = x86_64-pc-windows-msvc

SRC_FILES = src\array.c src\utils.c
TEST_FILES = tests\tests.c tests\utils.c

.PHONY: compile
compile:
	@IF NOT EXIST "target" MKDIR "target"
	$(CC) $(CFLAGS) --target=$(TARGET_ARCH) -o target\main.exe src\main.c $(SRC_FILES)

.PHONY: format
format:
	clang-format --Werror -i --style=file $(SRC_FILES) $(TEST_FILES)

.PHONY: test
test:
	$(CC) $(CFLAGS) --target=$(TARGET_ARCH) -o target\tests.exe $(TEST_FILES) $(SRC_FILES)
