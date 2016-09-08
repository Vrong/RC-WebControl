#ifndef CAPTURE_H
#define CAPTURE_H


#include <unistd.h>
#include <string>
#include <sys/ioctl.h>

using namespace cv;

class Capture
{
private:
	int m_camera;
	Mat image;
	std::vector<uchar> buf;
	struct v4l2_format fmt = {0};

	int m_height, m_width;
	uint8_t *buffer;

	VideoCapture cap;

	std::vector<uchar> encodeImage(std::string ext);
	static int xioctl(int fd, int request, void *arg);

public:
	Capture();
	Capture(int camera);
	Capture(int camera, int width, int height);

	void setSize(int width, int height);

	void updateImage();

	Mat getMat();
	std::vector<uchar> getJpeg();
	std::vector<uchar> getPng();


};


#endif
