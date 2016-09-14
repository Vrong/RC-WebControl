#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "tcp/TcpServer.hpp"
#include "tcp/TcpSocket.hpp"
#include "misc/Clock.hpp"

int DEBUG_GAMEPAD = 0;

#define CONTROL_PORT 9998;
#define CMD_GAMEPAD "GAMEPAD___"
#define CMD_TYPE_AXE "TYP_AXE___"
#define CMD_TYPE_BUTTON "TYP_BUTTON"
#define TYPE_AXE 1
#define TYPE_BUTTON 2


using namespace std;

void executeCommand(int type, int index, float value);

int main(int argc, char** argv)
{
  int port = CONTROL_PORT;

  if(argc < 2)
  {
    cout << "No valid argument : <port>" << endl;
    return -1;
  }

  if(strcmp(argv[1], "test") == 0)
  {
    DEBUG_GAMEPAD = 1;
  }
  else
  {
    port = atoi(argv[1]);
    cout << "Local control TCP server set on port " << port << endl;
  }

  if(DEBUG)
  {

  }

  TcpServer server(port);

  cout << "TCP Server running..." << endl;

  while(true)
  {
    cout << "Waiting for client" << endl;
    TcpSocket client = server.acceptClient();
    cout << "Connected to client" << endl;

    char rcv[11];
    std::vector<uchar> buffer;
    rcv[10] = '\0';
    while(true)
    {
      //memset(rcv, '\0', sizeof(rcv));

      if(!client.recv(rcv, 10))
      {
        cout << "Connection lost" << endl;
        break;
      }


      if(!strcmp(rcv, CMD_GAMEPAD))
      {
        int index = -1, type = -1;
        float value = -1f;

        if(!client.recv(rcv, 10))
        {
          cout << "Connection lost" << endl;
          break;
        }
        if(!strcmp(rcv, CMD_TYPE_AXE))
          type = TYPE_AXE;
        else
          type = TYPE_BUTTON;
        else
        {
          cout << "Invalid gamepad request : " << rcv << endl;
          break;
        }

        rcv[4] = '\0';
        if(!client.recv(rcv, 4))
        {
          cout << "Connection lost" << endl;
          break;
        }
        index = atoi(rcv);

        if(!client.recv(rcv, 4))
        {
          cout << "Connection lost" << endl;
          break;
        }
        value = atof(rcv);
        if(index >= 0 && value >= 0)
        {
          cout << "Received command : " << type << " | " << index << " | " << value << endl;
          executeCommand(type, index, value);
        }
        else
          cout << "Invalid resolution" << endl;
      }
      else
      {
        cout << "received " << rcv << endl;
      }
    }
  }
  return 0;
}


void executeCommand(int type, int index, float value)
{

}
