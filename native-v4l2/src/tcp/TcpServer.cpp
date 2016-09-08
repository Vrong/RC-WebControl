
#include "TcpServer.hpp"
#include "TcpSocket.hpp"
#include <iostream>

using namespace std;

TcpServer::TcpServer(int port) : m_port(port)
{ 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
  		cout << "ERROR opening socket" << endl;

  	bzero((char *) &serv_addr, sizeof(serv_addr));

  	// paramètrage du serveur
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	//réservation du port
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
		cout << "ERROR on binding" << endl;

	//écoute du port
	listen(sockfd,5);
}

TcpSocket TcpServer::acceptClient()
{

    clilen = sizeof(cli_addr);

	sock_cli = accept(sockfd, 
           (struct sockaddr *) &cli_addr, 
           &clilen);

	if (sock_cli < 0) 
	{
    	cout << "ERROR on accept" << endl;
    	TcpSocket client(-1, cli_addr);
    	return client;
	}

	TcpSocket client(sock_cli, cli_addr);

	return client;
}
