#include "CaptureOpencv.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <sstream>
//#include <stdio.h>

#include <unistd.h>

using namespace cv;


Capture::Capture() : Capture(0)
{

}

Capture::Capture(int camera) : m_camera(camera)
{
	m_width = m_height = 0;
	if(!cap.open(camera))
        return;

	//cap.set(CV_CAP_PROP_FPS, 15); //bizarre
	//cap.set(CV_CAP_PROP_BRIGHTNESS, 0.5); //0-1
	//cap.set(CV_CAP_PROP_CONTRAST, 1); //0-1
	//cap.set(CV_CAP_PROP_SATURATION, 1); //0-1
	//cap.set(CV_CAP_PROP_HUE, 1);
	//cap.set(CV_CAP_PROP_GAIN, 0.1); //not supported
	//cap.set(CV_CAP_PROP_EXPOSURE, 1); //not supported
	//cap.set(CV_CAP_PROP_HUE, 1);
}

Capture::Capture(int camera, int width, int height) : Capture(camera)
{
	setSize(width, height);
}

void Capture::setSize(int width, int height)
{
	m_width = width;
	m_height = height;

	if(m_width > 0 && m_height > 0)
	{
		cap.set(CV_CAP_PROP_FRAME_WIDTH,width);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,height);
	}
}



void Capture::updateImage()
{
	cap >> image;

	if(m_width > 0 && m_height > 0)
	{
		Size size(m_width,m_height);
		resize(image, image, size);
	}
}

Mat Capture::getMat()
{
	return image;
}


std::vector<uchar> Capture::encodeImage(std::string ext)
{
	buf.clear();

  std::vector<int> p;
  p.push_back(CV_IMWRITE_JPEG_QUALITY);
  p.push_back(95);

	imencode(ext, image, buf, p);
	return buf;
}

std::vector<uchar> Capture::getJpeg()
{
	return encodeImage(".jpg");
}

std::vector<uchar> Capture::getPng()
{
	return encodeImage(".png");
}


void Capture::show()
{
	std::string txt = "Capture ";

	std::ostringstream oss;
	oss << "Camera " << m_camera;

	//namedWindow(oss.str().c_str(), WINDOW_AUTOSIZE);
	imshow(oss.str().c_str(), image);
	cv::waitKey(1);
}
