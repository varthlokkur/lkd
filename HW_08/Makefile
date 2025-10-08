PWD := $(shell pwd)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

SRC_DIR := $(PWD)/src
BUILD_DIR := $(PWD)/build
BUILD_FILES += $(SRC_DIR)/*.ko

CLANG_FORMAT_VERS ?= 19
CLANG_FORMAT := clang-format-$(CLANG_FORMAT_VERS)
CLANG_FORMAT_FLAGS += -i
FORMAT_FILES := $(SRC_DIR)/*.c

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

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

load:
	$(INSMOD) $(BUILD_DIR)/clkd.ko

unload:
	$(RMMOD) clkd

app:
	$(CC) -c $(SRC_DIR)/device_test_app.c -o $(BUILD_DIR)/device_test_app.o
	$(CC) $(CC_FLAGS) $(BUILD_DIR)/device_test_app.o -o $(BUILD_DIR)/device_test_app
	@echo -e Test app compiled!$(NC)


.PHONY: make clean format