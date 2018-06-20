
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, str_len;
	socklen_t clnt_addr_sz;
	struct sockaddr_in serv_addr, clnt_addr;
	char message[BUFSIZ];

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("UDP socket creation error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	while (1) {
		memset(&message, 0, sizeof(message));  // clean buffer
		clnt_addr_sz = sizeof(clnt_addr);
		str_len = recvfrom(serv_sock, message, sizeof(message), 0, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);
		message[str_len] = 0;
		printf("Message from client : %s", message);

		memset(&message, 0, sizeof(message));  // clean buffer
		fputs("Insert message(Q to quit) : ", stdout);
                fgets(message, sizeof(message), stdin);
                if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
                        break;
		sendto(serv_sock, message, sizeof(message), 0, (struct sockaddr *)&clnt_addr, clnt_addr_sz);
	}

	close(serv_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
