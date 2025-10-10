#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define DEVICE_PATH "/dev/clkd"

#define DEVICE_IOCTL_MAGIC 'v'

// set mode encode or decode and reset buffers
#define SET_MODE _IOW(DEVICE_IOCTL_MAGIC, 1, int)

// return current mode (encode or decode)
#define GET_MODE _IOR(DEVICE_IOCTL_MAGIC, 2, int)

enum ProcessingMode { MODE_ENCODE = 0, MODE_DECODE = 1 };

void open_close_scenario()
{
	printf("[TEST] Open/close device\n");

	int fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		perror("[FAIL] Failed to open device");
		return;
	}

	if (close(fd) < 0) {
		perror("[FAIL] Close failed");
		return;
	}

	printf("[OK] Open/close device\n\n");
}

void encode_scenario()
{
	char buf[256];
	const char *test_msg = "Hello World!";

	printf("[TEST] Encoding\n");
	int fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		perror("[FAIL] Failed to open device");
		return;
	}

	printf("Writing to device: \"%s\"\n", test_msg);
	if (write(fd, test_msg, strlen(test_msg)) < 0) {
		perror("[FAIL] Write failed");
		close(fd);
		return;
	}

	lseek(fd, 0, SEEK_SET);
	ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
	if (bytes_read < 0) {
		perror("[FAIL] Read failed");
		close(fd);
		return;
	}
	buf[bytes_read] = '\0';

	if (close(fd) < 0) {
		perror("[FAIL] Close failed");
		return;
	}

	printf("[OK] Read %zd bytes: \"%s\"\n\n", bytes_read, buf);
}

void decode_scenario()
{
	char buf[256];
	const char *test_msg = "SGVsbG8gV29ybGRkIQ==";

	printf("[TEST] Decoding\n");
	int fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		perror("[FAIL] Failed to open device");
		return;
	}

	printf("Writing to device: \"%s\"\n", test_msg);
	if (write(fd, test_msg, strlen(test_msg)) < 0) {
		perror("[FAIL] Write failed");
		close(fd);
		return;
	}

	lseek(fd, 0, SEEK_SET);
	ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
	if (bytes_read < 0) {
		perror("[FAIL] Read failed");
		close(fd);
		return;
	}
	buf[bytes_read] = '\0';

	if (close(fd) < 0) {
		perror("[FAIL] Close failed");
		return;
	}

	printf("[OK] Read %zd bytes: \"%s\"\n\n", bytes_read, buf);
}

int get_ioctl_scenario()
{
	printf("[TEST] Get ioctl\n");
	int fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		perror("[FAIL] Failed to open device");
		return -1;
	}

	int mode = -1;
	if (ioctl(fd, GET_MODE, &mode) < 0) {
		perror("[FAIL] IOCTL failed");
		close(fd);
		return -1;
	}

	if (close(fd) < 0) {
		perror("[FAIL] Close failed");
		return -1;
	}

	printf("[OK] Device processing mode %d\n\n", mode);
	return mode;
}

int set_ioctl_scenario()
{
	printf("[TEST] Set ioctl...\n");
	int fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		perror("[FAIL] Failed to open device");
		return -1;
	}

	int mode = -1;
	if (ioctl(fd, GET_MODE, &mode) < 0) {
		perror("[FAIL] IOCTL failed");
		close(fd);
		return -1;
	}

	printf("Get processing mode: \"%d\"\n", mode);

	int new_mode = (mode == MODE_ENCODE) ? MODE_DECODE : MODE_ENCODE;

	if (ioctl(fd, SET_MODE, &new_mode) < 0) {
		perror("[FAIL] IOCTL failed");
		close(fd);
		return -1;
	}

	if (close(fd) < 0) {
		perror("[FAIL] Close failed");
		return -1;
	}
	printf("[OK] Set new processing mode %d\n\n", new_mode);

	return new_mode;
}

int main(int argc, char **argv)
{
	open_close_scenario();
	const int initial_mode = get_ioctl_scenario();
	if (initial_mode == MODE_ENCODE) {
		encode_scenario();
	} else {
		decode_scenario();
	}

	const int changed_mode = set_ioctl_scenario();

	if (changed_mode == MODE_ENCODE) {
		encode_scenario();
	} else {
		decode_scenario();
	}

	return 0;
}