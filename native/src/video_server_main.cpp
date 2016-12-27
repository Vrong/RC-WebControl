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
#include "video/Capture.hpp"
#include "misc/Clock.hpp"

int DEBUG_CAMERA = 0;

#define CMD_GET_IMAGE "GET_IMAGE_"
#define CMD_SET_RESOLUTION "SET_RESOLU"

using namespace std;


int main(int argc, char** argv)
{
  int port = 9999;

  if(argc < 2)
  {
    cout << "No valid argument : <port>" << endl;
    return -1;
  }

  if(strcmp(argv[1], "videotest") == 0)
  {
    DEBUG_CAMERA = 1;
  }
  else
  {
    port = atoi(argv[1]);
    cout << "Local native video TCP server set on port " << port << endl;
  }


  //Init capture on opencv camera 0
  //Capture capture(0, 200, 150);
  Capture capture(0);
  capture.updateImage();


  if(DEBUG_CAMERA)
  {
    Timer timer;
    Timer fpstimer;
    int fps = 0, fpscpt = 0;
    while(1)
    {
      timer.restart();
      capture.updateImage();
      uint64_t capturetime = (timer.elapsed_ms());
      uint64_t avant = timer.elapsed_us();
      capture.getJpeg();
      uint64_t compression = (timer.elapsed_us() - avant);
      cout << "Temps compression : " << (timer.elapsed_us() - avant) << endl << endl;
      capture.show();

      fpscpt ++;
      if(fpstimer.elapsed_s() >= 1)
      {
        fps = fpscpt;
        fpscpt = 0;
        fpstimer.restart();
      }

      cout << "Temps Capture :     " << capturetime << endl
           << "Temps compression : " << compression << endl
           << "FPS :               " << fps << endl << endl;
    }
  }

  TcpServer server(port);

  cout << "TCP Server running..." << endl;

  while(true)
  {
    cout << "Waiting for client" << endl;
    TcpSocket client = server.acceptClient();
    cout << "Video server connected to client" << endl;

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

      if(!strcmp(rcv, CMD_GET_IMAGE))
      {
        capture.updateImage();
        buffer = capture.getJpeg();
        client.send((char*)buffer.data(), buffer.size()); //sending image
        //cout << "image sent" << endl;
      }
      else if(!strcmp(rcv, CMD_SET_RESOLUTION))
      {
        rcv[4] = '\0';
        int w, h;
        if(!client.recv(rcv, 4))
        {
          cout << "Connection lost" << endl;
          break;
        }
        w = atoi(rcv);

        if(!client.recv(rcv, 4))
        {
          cout << "Connection lost" << endl;
          break;
        }
        h = atoi(rcv);
        if(h > 0 && w > 0)
        {
          capture.setSize(w, h);
          cout << "Resolution changed to " << w << "x" << h << endl;
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
