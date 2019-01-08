#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

static void send_bit(const char *filename, bool bit)
{
	char dummy;
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open failed");
		exit(EXIT_FAILURE);
	}

	if (bit) {
		if (read(fd, &dummy, sizeof(dummy)) < 0) {
			perror("read failed");
			exit(EXIT_FAILURE);
		}
	}

	close(fd);
}

static void send_message(const char *filename, const char *msg)
{
	bool bit;
	int i, j;

	for (i = 0; i < strlen(msg); i++) {
		for (j = 0; j < 8; j++) {
			bit = msg[i] & (1 << j);
			send_bit(filename, bit);
		}
	}
}

int main(int argc, char **argv)
{
	send_message(argv[1], argv[2]);
	exit(EXIT_SUCCESS);
}
