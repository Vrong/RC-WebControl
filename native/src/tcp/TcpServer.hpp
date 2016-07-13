#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "TcpSocket.hpp"



class TcpServer
{
private:
	int sockfd, sock_cli, m_port;
	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;

public:
	TcpServer(int port);

	TcpSocket acceptClient();

};


#endif