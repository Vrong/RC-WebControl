#ifndef TCP_TARGET_H
#define TCP_TARGET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


class TcpSocket
{
private:
	int sock, n;
	socklen_t len;
	struct sockaddr_in addr;

public:
	TcpSocket(int socket, struct sockaddr_in address);

	int send(char buffer[], int len);

	int recv(char buffer[], int len);

};


#endif
