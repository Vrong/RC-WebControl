#ifndef CAPTURE_H
#define CAPTURE_H

#include "opencv2/opencv.hpp"

#define FRAME 0
#define IFRAME 1

#include <unistd.h>
#include <string>

using namespace cv;

class Capture
{
private:
	int m_camera;
	int m_quality = 95;
	Mat cur_image, prec_image, iframe;
	std::vector<uchar> buf;

	int m_height, m_width;

	VideoCapture cap;

	std::vector<uchar> encodeImage(std::string ext, int source);

public:
	Capture();
	Capture(int camera);
	Capture(int camera, int width, int height);

	void setSize(int width, int height);
	void setJpegQuality(int quality);

	void updateImage();
	void calculateIFrame();

	Mat getMat();
	std::vector<uchar> getJpeg();
	std::vector<uchar> getPng();
	std::vector<uchar> getJpeg(int source);
	std::vector<uchar> getPng(int source);

	void show();

};


#endif
