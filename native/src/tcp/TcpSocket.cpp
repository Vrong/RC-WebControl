#include "TcpSocket.hpp"


TcpSocket::TcpSocket(int socket, struct sockaddr_in address) : sock(socket), addr(address)
{ }

int TcpSocket::send(char buffer[], int len)
{
	return write(sock,buffer,len);
}

int TcpSocket::recv(char buffer[], int len)
{
 return read(sock,buffer,len);
}
