#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/file.h>

#define LOCK_FILE "/home/debian/Desktop/svn_project/programming/flock/flocks"

int acquire_lock() {
	int lock_fd = open(LOCK_FILE, O_RDONLY);
	if (lock_fd == -1) {
		perror("Unable to open lock file");
		exit(EXIT_FAILURE);
	}
	/*LOCK_EX : only one process can get lock(exclusive)
	 *LOCK_NB : return when locked, not blocking
	 */
	if (flock(lock_fd, LOCK_EX | LOCK_NB) == -1) {
		close(lock_fd);
		return -1;
	}

	return lock_fd;
}

int main() {
	pid_t pid, sid;
	int lock_fd, fd;
	int retval;
	fd_set rfds;
	struct timeval tv;

	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}

	umask(0);

	lock_fd = acquire_lock();
	while (1) {
		if (lock_fd == -1) {
			printf("Another instance of the daemon is already running.\n");
			break;
		}

		tv.tv_sec = 2;
		tv.tv_usec = 0;

		retval = select(0, NULL, NULL, NULL, &tv);
		if (retval == -1) {
			perror("select() error");
			break;
		}
		else if (retval == 0) {
			continue;
		}
	}

	if (lock_fd != -1) {
		flock(lock_fd, LOCK_UN);
		close(lock_fd);
	}
	printf("Daemon stopped\n");
	exit(EXIT_SUCCESS);
}

