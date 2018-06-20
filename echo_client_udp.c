
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock, str_len;
	struct sockaddr_in serv_addr, from_addr;
	socklen_t addr_sz;
	char message[BUFSIZ];

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	while (1) {
		memset(&message, 0, sizeof(message));  // clean buffer
		fputs("Insert message(Q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		sendto(sock, message, sizeof(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

		memset(&message, 0, sizeof(message));  // clean buffer
		addr_sz = sizeof(from_addr);
		str_len = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *)&from_addr, &addr_sz);
		message[str_len] = 0;
		printf("Message from server : %s", message);
	}
	close(sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
