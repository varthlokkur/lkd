PWD := $(shell pwd)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

SRC_DIR := $(PWD)/src
BUILD_DIR := $(PWD)/build
BUILD_FILES += $(SRC_DIR)/*.ko

CLANG_FORMAT_VERS ?= 19
CLANG_FORMAT := clang-format-$(CLANG_FORMAT_VERS)
CLANG_FORMAT_FLAGS += -i
FORMAT_FILES := $(SRC_DIR)/*.c

CC := gcc
CC_FLAGS := -Wall -Wextra -Werror
CC_SRC := $(PWD)/src/pthread_app.c

INSMOD := insmod
RMMOD := rmmod

$(shell mkdir -p $(BUILD_DIR))

kbuild:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	mv $(BUILD_FILES) $(BUILD_DIR)/.

format:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(FORMAT_FILES)

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

app_us:
	$(CC) -c $(SRC_DIR)/pthread_app.c -o $(BUILD_DIR)/pthread_app.o
	$(CC) $(CC_FLAGS) $(BUILD_DIR)/pthread_app.o -o $(BUILD_DIR)/pthread_app
	@echo -e Philo compiled!$(NC)

load:
	$(INSMOD) $(PWD)/build/kthread_app.ko

unload:
	$(RMMOD) kthread_app

.PHONY: kbuild app_us clean format