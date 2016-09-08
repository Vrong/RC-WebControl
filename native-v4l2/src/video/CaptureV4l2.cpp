#include "CaptureV4l2.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <sys/ioctl.h>
//#include <stdio.h>

#include <unistd.h>

using namespace cv;


Capture::Capture() : Capture(0)
{

}

Capture::Capture(int camera) : m_camera(camera)
{
	m_width = m_height = 0;
	int fd;
	fd = open("/dev/video" + camera, O_RDWR);
	if (fd == -1)
	{
	    perror("Opening Video device");
	    return 1;
	}

	struct v4l2_capability caps = {0};
	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps))
	{
	    perror("Querying Capabilites");
	    return 1;
	}


	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;
	if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
	{
	    perror("Setting Pixel Format");
	    return 1;
	}


	struct v4l2_requestbuffers req = {0};
	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
	{
	    perror("Requesting Buffer");
	    return 1;
	}

	struct v4l2_buffer buf = {0};
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = bufferindex;
	if(-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
	{
	    perror("Querying Buffer");
	    return 1;
	}

	buffer = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

}

Capture::Capture(int camera, int width, int height) : Capture(camera)
{


}


static int xioctl(int fd, int request, void *arg)
{
    int r;
    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);
    return r;
}

void Capture::setSize(int width, int height)
{
	m_width = width;
	m_height = height;

	if(m_width > 0 && m_height > 0)
	{
		fmt.fmt.pix.width = m_width;
		fmt.fmt.pix.height = m_height;
	}
}



void Capture::updateImage()
{

	if(-1 == xioctl(fd, VIDIOC_STREAMON, &buf.type))
	{
	    perror("Start Capture");
	    return 1;
	}

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	struct timeval tv = {0};
	tv.tv_sec = 2;
	int r = select(fd+1, &fds, NULL, NULL, &tv);
	if(-1 == r)
	{
	    perror("Waiting for Frame");
	    return 1;
	}

	if(-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
	{
	    perror("Retrieving Frame");
	    return 1;
	}

}

Mat Capture::getMat()
{

}


std::vector<uchar> Capture::encodeImage(std::string ext)
{
	buf.clear();

  std::vector<int> p;

	return buf;
}

std::vector<uchar> Capture::getJpeg()
{

}

std::vector<uchar> Capture::getPng()
{

}
