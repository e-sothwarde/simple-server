#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>

#define IP "192.168.64.2"
#define PORT 3005
#define BUF_SIZE 1000
#define BACKLOG 10

// initialises listening socked, binds, listens and returns sockfd
int listener_init(void) { 	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &(addr.sin_addr));

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
	listen(sockfd, BACKLOG);

	return sockfd;
}

struct request_line {
	char method[10];
	char target[50];
	char version[8];
};

struct status_line {
	char version[8];
	char status[3];
	char [20];
};

struct field_line {
};

int respond(int code) {
};

int get(struct request_line rl) {
	// look for target resource
	char target_dir[] = "/home/user/Programming/http-server";
	int target_len = strlen(target_dir) + strlen(rl.target);
	char target[target_len];
	memset(target, 0, target_len);
	strcat(target, target_dir);
	strcat(target, rl.target);
	printf("%s\n", target);

	FILE *target_file = fopen(target, "r");
	if (target_file == NULL) {
		printf("404\n");
		respond(404);
		exit(1);
	}

	printf("Resource Found\n");
}

int main(void) {
	int listener_fd = listener_init();

	// accept an incoming connection
	struct sockaddr conn_addr;
	socklen_t caddr_size = sizeof(conn_addr);
	int connected_fd = accept(listener_fd, &conn_addr, &caddr_size);
	FILE *connected_file = fdopen(connected_fd, "r");

	struct request_line rl;
	fscanf(connected_file, "%s %s %s\n", rl.method, rl.target, rl.version);

	if (!strcmp(rl.method, "GET")) {
		get(rl);
	}

	close(connected_fd);
	close(listener_fd);
}
