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

$(shell mkdir -p $(BUILD_DIR))

kbuild:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	mv $(BUILD_FILES) $(BUILD_DIR)/.

format:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(FORMAT_FILES)

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

load:
	$(INSMOD) $(BUILD_DIR)/ex_tasklets.ko
	$(INSMOD) $(BUILD_DIR)/ex_workqueue.ko

unload:
	$(RMMOD) ex_tasklets
	$(RMMOD) ex_workqueue

.PHONY: kbuild clean format