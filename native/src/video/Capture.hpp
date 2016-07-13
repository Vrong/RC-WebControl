#ifndef CAPTURE_H
#define CAPTURE_H

#include "opencv2/opencv.hpp"

#include <unistd.h>

using namespace cv;

class Capture
{
private:
	int m_camera;
	Mat image;
	std::vector<uchar> buf;

	int m_height, m_width;

	VideoCapture cap;

	std::vector<uchar> encodeImage(string ext);

public:
	Capture();
	Capture(int camera);
	Capture(int camera, int width, int height);

	void setSize(int width, int height);

	void updateImage();

	Mat getMat();
	std::vector<uchar> getJpeg();
	std::vector<uchar> getPng();

	void show();

};


#endif
