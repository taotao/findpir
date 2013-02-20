#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main (int argc, char *argv[])
{
	int fd = (-1);
	unsigned char *addr;
	unsigned char *temp;
	long count;

	fd = open ("/dev/mem", O_RDWR|O_SYNC);
	if (fd < 0) {
		perror ("open");
		goto main_end;
	}

	addr = (unsigned char *)mmap (NULL, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0xF0000);
	if (addr == MAP_FAILED) {
		perror ("mmap");
		goto main_end;
	}

	for (temp = addr, count = 0; temp < (addr+0x10000); temp += 16, count += 16) {
		if (strncmp ((const char *)temp, "$PIR", 4) == 0) {
			break;
		}
	}
	if (temp >= addr+0x10000) {
		printf ("out of range\n");
		goto main_end;
	}

	printf ("Addr: %08X\n", (unsigned int)(0xF0000 + count));

main_end:

	if (addr != MAP_FAILED) {
		munmap (addr, 0x10000);
	}

	if (fd != (-1)) {
		close (fd);
	}

	return 0;
}

