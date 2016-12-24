#include "Capture.hpp"
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

void Capture::setJpegQuality(int quality)
{
	if(quality > 0 && quality <= 100)
	{
		m_quality = quality;
	}
}

void Capture::setBW(bool bw)
{
	m_blackandwhite = bw;
}

void Capture::setEqualization(bool equ)
{
	m_equalization = equ;
}


void Capture::updateImage()
{
	prec_image.release();
	prec_image = cur_image.clone();
	cap >> cur_image;

	applyFilters(cur_image, cur_image);

	// resize image if demanded
	if(m_width > 0 && m_height > 0)
	{
		Size size(m_width,m_height);
		resize(cur_image, cur_image, size);
	}
}

void Capture::applyFilters(cv::Mat &src, cv::Mat &dst)
{

		// convert to grayscale if demanded
		if(m_blackandwhite)
			cvtColor(src, dst, CV_BGR2GRAY);

		// equalize if demanded
		if(m_equalization)
		{
			// different equalization method used if grayscale or not
			if(m_blackandwhite)
			{
		  	equalizeHist(src, dst);
			}
			else // color equalization
			{
				Mat equ_image;
			 	std::vector<Mat> channels;
				cvtColor(src, equ_image, CV_BGR2YCrCb); //change the color image from BGR to YCrCb format
				split(equ_image, channels); //split the image into channels
				equalizeHist(channels[0], channels[0]); //equalize histogram on the 1st channel (Y)
				merge(channels, equ_image); //merge 3 channels including the modified 1st channel into one image
				cvtColor(equ_image, dst, CV_YCrCb2BGR); //change the color image from YCrCb to BGR format (to display image properly)
				equ_image.release();
			}
		}
}


void Capture::calculateIFrame()
{
	if(prec_image.channels() == cur_image.channels() &&
				prec_image.depth() == cur_image.depth() &&
				prec_image.cols == cur_image.cols &&
				prec_image.rows == cur_image.rows) //if prec and cur avec same characteristics we can do a substraction
			absdiff(prec_image, cur_image, iframe);
	else // or we just give a black image because properties just changed
	{
		iframe = cur_image.clone();
		iframe.setTo(Scalar(0)); //may depends of channels count
	}
}

Mat Capture::getImageMat()
{
	return cur_image;
}

Mat Capture::getIFrameMat()
{
	return iframe;
}

void Capture::saveImage(char dest[])
{
	imwrite(dest, cur_image, getJpegParams());
}

void Capture::saveIFrame(char dest[])
{
	imwrite(dest, iframe, getJpegParams());
}


std::vector<int> Capture::getJpegParams()
{
	  std::vector<int> p;
	  p.push_back(CV_IMWRITE_JPEG_QUALITY);
	  p.push_back(m_quality);
		return p;
}

std::vector<uchar> Capture::encodeImage(std::string ext, int source)
{
	buf.clear();

	if(source == Type::FRAME)
		imencode(ext, cur_image, buf, getJpegParams());
	else if(source == Type::IFRAME)
		imencode(ext, iframe, buf, getJpegParams());
	return buf;
}

std::vector<uchar> Capture::getJpeg()
{
	return getJpeg(Type::FRAME);
}

std::vector<uchar> Capture::getPng()
{
	return getPng(Type::FRAME);
}
std::vector<uchar> Capture::getJpeg(int source)
{
	return encodeImage(".jpg", source);
}

std::vector<uchar> Capture::getPng(int source)
{
	return encodeImage(".png", source);
}

void Capture::show()
{
	show(Type::FRAME);
}

void Capture::show(int source)
{
	std::string txt = "Capture ";

	std::ostringstream oss;
	oss << "Camera " << m_camera;

	//namedWindow(oss.str().c_str(), WINDOW_AUTOSIZE);
	if(source == Type::FRAME)
		imshow(oss.str().c_str(), cur_image);
	else if(source == Type::IFRAME)
		imshow(oss.str().c_str(), iframe);
	cv::waitKey(1);
}
