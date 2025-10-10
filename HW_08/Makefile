PWD := $(shell pwd)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

SRC_DIR := $(PWD)/src
SRC_TEST_DIR := $(PWD)/tests
BUILD_DIR := $(PWD)/build
BUILD_FILES += *.ko
TEST_SCRIPTS_FILES += $(SRC_TEST_DIR)/*.sh

CLANG_FORMAT_VERS ?= 19
CLANG_FORMAT := clang-format-$(CLANG_FORMAT_VERS)
CLANG_FORMAT_FLAGS += -i
FORMAT_FILES := $(SRC_DIR)/*.c
FORMAT_TEST_FILES := $(SRC_TEST_DIR)/*.c

INSMOD = insmod
RMMOD = rmmod

CC := gcc
CC_FLAGS := -Wall -Wextra -Werror

$(shell mkdir -p $(BUILD_DIR))

make:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	mv $(BUILD_FILES) $(BUILD_DIR)/.

format:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(FORMAT_FILES)
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(FORMAT_TEST_FILES)

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

load:
	$(INSMOD) $(BUILD_DIR)/clkd.ko

unload:
	$(RMMOD) clkd

tests:
	$(CC) -c $(SRC_TEST_DIR)/device_test_app.c -o $(BUILD_DIR)/device_test_app.o
	$(CC) $(CC_FLAGS) $(BUILD_DIR)/device_test_app.o -o $(BUILD_DIR)/device_test_app
	cp $(TEST_SCRIPTS_FILES) $(BUILD_DIR)/.
	@echo -e Test app compiled!$(NC)


.PHONY: make clean format tests