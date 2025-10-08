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
#define GET_VERSION _IOR(DEVICE_IOCTL_MAGIC, 0, int)

void open_close_scenario()
{
	printf("[TEST] Opening device...\n");
	int fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) 
	{
		perror("[FAIL] Failed to open device");
		return;
	}
	printf("[OK] Device opened\n\n");

	printf("[TEST] Closing device...\n");
	if (close(fd) < 0) 
	{
		perror("[FAIL] Close failed");
		return;
	}
	printf("[OK] Device closed\n");
}

void read_write_scenario()
{
	char buf[256];
	const char *test_msg = "0xFFFFFFFF";

	printf("[TEST] Opening device...\n");
	int fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) 
	{
		perror("[FAIL] Failed to open device");
		return;
	}
	printf("[OK] Device opened\n\n");

	printf("[TEST] Writing to device: \"%s\"\n", test_msg);
	if (write(fd, test_msg, strlen(test_msg)) < 0) 
	{
		perror("Write failed");
		close(fd);
		return;
	}
	printf("[OK] Write successful\n\n");

	printf("[TEST] Reading from device...\n");
	lseek(fd, 0, SEEK_SET); // Сбрасываем позицию
	ssize_t bytes_read = read(fd, buf, sizeof(buf)-1);
	if (bytes_read < 0) 
	{
		perror("Read failed");
		close(fd);
		return;
	}
	buf[bytes_read] = '\0';
	printf("[OK] Read %zd bytes: \"%s\"\n\n", bytes_read, buf);


	printf("[TEST] Closing device...\n");
	if (close(fd) < 0) 
	{
		perror("[FAIL] Close failed");
		return;
	}
	printf("[OK] Device closed\n");
}

void ioctl_scenario()
{
	printf("[TEST] Opening device...\n");
	int fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) 
	{
		perror("[FAIL] Failed to open device");
		return;
	}
	printf("[OK] Device opened\n\n");

	int version = -1;
	printf("[TEST] Sending IOCTL command: \"%d\"\n", version);
	if (ioctl(fd, GET_VERSION, &version) < 0) 
	{
		perror("[FAIL] IOCTL failed");
		close(fd);
		return;
	}
	printf("[TEST] Get IOCTL result: \"%d\"\n", version);

	printf("[TEST] Closing device...\n");
	if (close(fd) < 0) 
	{
		perror("[FAIL] Close failed");
		return;
	}
	printf("[OK] Device closed\n");

	
}

int main(int argc, char **argv)
{
	open_close_scenario();
	read_write_scenario();
	ioctl_scenario();

	return 0;
}