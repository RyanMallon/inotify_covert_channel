#include <sys/inotify.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	struct inotify_event events[16];
	ssize_t count;
	size_t num_events;
	int i, bit, ifd, wfd;
	char byte;

	ifd = inotify_init();
	if (ifd < 0) {
		perror("inotify_init failed");
		exit(EXIT_FAILURE);
	}

	if (inotify_add_watch(ifd, argv[1], IN_OPEN | IN_CLOSE_NOWRITE | IN_ACCESS) < 0) {
		perror("inotify_add_watch failed");
		exit(EXIT_FAILURE);
	}

	bit = 0;
	byte = 0;
	while (1) {
		count = read(ifd, events, sizeof(events));
		if (count < 0) {
			perror("read error");
			exit(EXIT_FAILURE);
		}

		num_events = count / sizeof(events[0]);
		for (i = 0; i < num_events; i++) {

			if (events[i].mask & IN_ACCESS)
				byte |= (1 << bit);

			if (events[i].mask & IN_CLOSE) {
				bit++;
				if (bit == 8) {
					printf("Byte: %c\n", byte);
					byte = 0;
					bit = 0;
				}
			}
		}
	}

	exit(EXIT_SUCCESS);
}
